/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "SoundManager.hpp"

namespace Enigma
{

    SoundManager::SoundManager(ClientTransmissionManager& clientTransmissionManager)
		: mClientTransmissionManager(clientTransmissionManager),
		mAudioDeviceFactory(clientTransmissionManager)
    {
        this->Preinit();
    }

    SoundManager::~SoundManager()
    {
        //this->Unload();
    }

    void SoundManager::Preinit()
    {
        this->mAudioDevice=NULL;
		this->mIsRecording=false;
		this->mHasCaptured=false;

		this->mLastPoll = boost::posix_time::microsec_clock::local_time();
    }

    void SoundManager::Init(int argc, Enigma::c8** argv)
    {
		this->margc=argc;
		this->margv=argv;
    }

    void SoundManager::Load()
    {
		//this->mClientTransmissionManager.GetClientSessionManager().RegisterAudioEventListener(this);

		this->mAudioDevice = this->mAudioDeviceFactory.CreateDevice();

		if(this->mAudioDevice!=NULL)
		{    
			this->mAudioDevice->Init(this->margc,this->margv);

			this->mAudioDevice->Load();

			//set default listener.
			this->UpdateListenerValues(Character());
		}
    }

    void SoundManager::Unload()
    {
		if(this->mAudioDevice!=NULL)
		{
			this->mAudioDevice->Unload();
		}
    }

	void SoundManager::Capture()
	{
		if(this->mIsRecording || !this->mHasCaptured)
		{
			this->mAudioDevice->Capture();
			this->mHasCaptured=true;
		}
	}

    void SoundManager::Poll()
    {
		//boost::posix_time::time_duration diff = boost::posix_time::microsec_clock::local_time() - this->mLastPoll;

		this->mClientTransmissionManager.GetClientSessionManager().DoAudioEvents();

		this->Capture();

		this->mAudioDevice->Poll();

		this->mClientTransmissionManager.GetClientSessionManager().Wait();

		/*while(diff.total_milliseconds() < 20)
		{
			boost::this_thread::sleep(boost::posix_time::milliseconds(1));
			diff = boost::posix_time::microsec_clock::local_time() - this->mLastPoll;
		}

		if(diff.total_milliseconds() > 21)
		{
			std::cout << "Actual time difference: " << diff.total_milliseconds() << std::endl;
		}

		this->mLastPoll = boost::posix_time::microsec_clock::local_time();*/
    }

    void SoundManager::StreamAtLocation(const Enigma::Entity& entity,Enigma::u8* buffer,int priority,size_t length)
    {
        if(this->mAudioDevice!=NULL)
        {
            this->mAudioDevice->StreamAtLocation(entity,buffer,priority,length);
        }
    }

    void SoundManager::StreamVoiceAtLocation(const Enigma::Entity& entity,Enigma::u8* buffer,size_t length)
    {
		if(this->mAudioDevice!=NULL)
        {
            this->mAudioDevice->StreamVoiceAtLocation(entity,buffer,length);
        }
    }

    void SoundManager::PlayAtLocation(const Enigma::Entity& entity,const std::string& filename,int priority)
    {
        if(this->GetIsLoaded() && this->mAudioDevice!=NULL)
        {
            this->mAudioDevice->PlayAtLocation(entity,filename,priority);
        }
    }

    void SoundManager::PlayAtLocation(const Enigma::Entity& entity,Enigma::c8* filename,int priority)
    {
        if(this->GetIsLoaded() && this->mAudioDevice!=NULL)
        {
            this->mAudioDevice->PlayAtLocation(entity,filename,priority);
        }
    }

    void SoundManager::PlayAtLocation(const Enigma::Entity& entity,const Enigma::c8* filename,int priority)
    {
        if(this->GetIsLoaded() && this->mAudioDevice!=NULL)
        {
            this->mAudioDevice->PlayAtLocation(entity,filename,priority);
        }
    }

    void SoundManager::UpdateListenerValues(const Enigma::Entity& entity)
    {
        if(this->GetIsLoaded() && this->mAudioDevice!=NULL)
        {
            this->mAudioDevice->UpdateListenerValues(entity);
        }
    }

    void SoundManager::ReallyStartRecording()
    {
        if(this->GetIsLoaded() && this->mAudioDevice!=NULL)
        {
            this->mAudioDevice->StartRecording();
			this->mIsRecording=true;
			this->mHasCaptured=false;
        }
    }

    void SoundManager::ReallyStopRecording()
    {
        if(this->GetIsLoaded() && this->mAudioDevice!=NULL)
        {
            this->mAudioDevice->StopRecording();
			this->mIsRecording=false;
			this->mHasCaptured=false;
        }
    }

    void SoundManager::StartRecording()
    {
		EnigmaEvent enigmaEvent;
		enigmaEvent.EventType = EnigmaEvent::EVENT_AUDIO_RECORDER_STATE_CHANGED;
		enigmaEvent.Index = (size_t)true;
		this->mClientTransmissionManager.GetClientSessionManager().PushAudioEvent(enigmaEvent);
    }

    void SoundManager::StopRecording()
    {
		EnigmaEvent enigmaEvent;
		enigmaEvent.EventType = EnigmaEvent::EVENT_AUDIO_RECORDER_STATE_CHANGED;
		enigmaEvent.Index = (size_t)false;
		this->mClientTransmissionManager.GetClientSessionManager().PushAudioEvent(enigmaEvent);
    }

	void SoundManager::onVoiceReceived(Enigma::s32 characterId, const Enigma::Character& character, Enigma::u8* buffer,size_t length)
	{
        if(this->GetIsLoaded() && this->mAudioDevice!=NULL)
        {
			this->StreamVoiceAtLocation(character,buffer,length);
        }		
	}

	void SoundManager::onRecorderStateChanged(bool isRecording)
	{
		if(this->mIsRecording==isRecording)
		{
			return;
		}

		if(isRecording)
		{
			this->ReallyStartRecording();
		}
		else
		{
			this->ReallyStopRecording();
		}
	}

	void SoundManager::onPlayFile(const Enigma::Entity& entity, size_t audioId, const std::string& filename)
	{
        if(this->GetIsLoaded() && this->mAudioDevice!=NULL)
        {
			if(audioId>0)
			{
				std::cout << "SoundManager does not currently support audioId." << std::endl;
			}
			else
			{
				if(!filename.empty())
				{
					this->PlayAtLocation(entity,filename,0);
				}
				else
				{
					std::cout << "SoundManager can not play a sound file with no filename and no audio id." << std::endl;
				}
			}
        }	
	}
};
