/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "OpenALAudioDevice.hpp"

#include "boost_foreach.hpp"

#include <boost/chrono.hpp>

#include "SpeexDecoder.hpp"

namespace Enigma
{
	typedef std::pair<boost::uuids::uuid,SoundSource> SoundSourcePair;

    OpenALAudioDevice::OpenALAudioDevice(ClientTransmissionManager& clientTransmissionManager)
		: mClientTransmissionManager(clientTransmissionManager)
    {
        this->Preinit();
    }

    OpenALAudioDevice::~OpenALAudioDevice()
    {
        this->Unload();
        if(this->mCaptureDevice!=NULL)
        {
            alcCaptureCloseDevice(this->mCaptureDevice);
        }
        alcDestroyContext(this->mContext);
        alcCloseDevice(this->mDevice);
        //alutExit();
    }

    void OpenALAudioDevice::Preinit()
    {
        this->mIsLoaded=false;

        this->mDevice=NULL;
        this->mContext=NULL;

        this->mCaptureDevice=NULL;
        this->mDefaultCaptureDevice=NULL;
        this->mAvailableSamples=0;

        this->mBitsPerSample = VOICE_CHAT_RESOLUTION;
        this->mChannels = 1;
        this->mBlockAlign = this->mChannels * this->mBitsPerSample / 8;
    }

    void OpenALAudioDevice::Init(int argc, Enigma::c8** argv)
    {
        //this->mSoundSources.resize(100);

        //alutInit(&argc, argv);
        //this->CheckForOpenALError(true);

		std::cout << "---Available Audio Devices---" << std::endl;
		char* devices = (char*)alcGetString(NULL, ALC_DEVICE_SPECIFIER);
		while(devices && *devices !=NULL)
		{	
			std::cout << devices << std::endl;
			devices += strlen(devices) + 1; //next device
		}
		std::cout <<"-----------------------------" <<  std::endl;

        if(this->mDevice==NULL)
        {
            #ifdef PLATFORM_MAC
                this->mDevice=alcOpenDevice((const ALCbyte*)"Generic Software");
            #else
                this->mDevice=alcOpenDevice("Generic Software");
            #endif
            this->CheckForOpenALError(true,"alcOpenDevice");
			std::cout << "Trying generic software audio device." << std::endl;

			if(this->mDevice!=NULL)
			{
				std::cout << "Using generic software audio device." << std::endl;
			}
		}

        if(this->mDevice==NULL)
        {
			this->mDevice=alcOpenDevice(NULL);
			this->CheckForOpenALError(true,"alcOpenDevice");

			if(this->mDevice!=NULL)
			{
				std::cout << "Using {Default} audio device." << std::endl;
			}
		}

        if(this->mDevice==NULL)
        {
            #ifdef PLATFORM_MAC
                this->mDevice=alcOpenDevice((const ALCbyte*)"Generic Hardware");
            #else
                this->mDevice=alcOpenDevice("Generic Hardware");
            #endif
            this->CheckForOpenALError(true,"alcOpenDevice");
			std::cout << "Trying generic hardware audio device." << std::endl;

			if(this->mDevice!=NULL)
			{
				std::cout << "Using generic hardware audio device." << std::endl;
			}
		}

        if(this->mDevice==NULL)
        {
            #ifdef PLATFORM_MAC
                this->mDevice=alcOpenDevice((const ALCbyte*)"DirectSound Default");
            #else
                this->mDevice=alcOpenDevice("DirectSound Default");
            #endif
            this->CheckForOpenALError(true,"alcOpenDevice");
			std::cout << "Trying DirectSound default audio device." << std::endl;

			if(this->mDevice!=NULL)
			{
				std::cout << "Using DirectSound default audio device." << std::endl;
			}
		}

        if(this->mDevice==NULL)
        {
            #ifdef PLATFORM_MAC
                this->mDevice=alcOpenDevice((const ALCbyte*)"No Output");
            #else
                this->mDevice=alcOpenDevice("No Output");
            #endif
            this->CheckForOpenALError(true,"alcOpenDevice");
			std::cout << "Failed to load any audio device defaulting to no output!" << std::endl;

			if(this->mDevice!=NULL)
			{
				std::cout << "Using no audio device." << std::endl;
			}
        }

        if(this->mDevice==NULL)
        {
            throw EnigmaException("Failed to open sound device!");
        }

		int attrlist[] = 
		{ 
		#ifdef ALC_SOURCES
          	ALC_SOURCES, NUMBER_OF_AUDIO_SOURCES,
		#endif
		#ifdef ALC_MONO_SOURCES
          	ALC_MONO_SOURCES, NUMBER_OF_AUDIO_SOURCES,
		#endif
		#ifdef ALC_STEREO_SOURCES 
			ALC_STEREO_SOURCES, 0,
		#endif
		#ifdef ALC_BUFFERS
			ALC_BUFFERS, (NUMBER_OF_AUDIO_SOURCES * NUMBER_OF_AUDIO_BUFFERS),
		#endif
		#ifdef ALC_FREQUENCY
          	ALC_FREQUENCY, VOICE_CHAT_FREQUENCY,
		#endif
		#ifdef ALC_RESOLUTION
			ALC_RESOLUTION, VOICE_CHAT_RESOLUTION,
		#endif
		#ifdef ALC_BUFFERSIZE
			ALC_BUFFERSIZE, VOICE_CHAT_BUFFER_SIZE,
		#endif
		#ifdef ALC_SYNC
			ALC_SYNC, AL_FALSE,
		#endif
          	ALC_INVALID 
		};

        this->mContext=alcCreateContext(this->mDevice,attrlist);
        this->CheckForOpenALError(true,"alcCreateContext");

        if(this->mContext==NULL)
        {
			throw EnigmaException("Failed to open sound context!");
        }

        if(alcMakeContextCurrent(this->mContext))
        {
            std::cout << "Default audio context set." << std::endl;
            #ifdef ENIGMA_PLATFORM_WINDOWS
            this->mDefaultCaptureDevice = const_cast<ALCbyte*>(alcGetString(NULL, ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER));
            #else
            this->mDefaultCaptureDevice = (ALCbyte*)(alcGetString(NULL, ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER));
            #endif
            this->CheckForOpenALError(true,"alcGetString");
            if(this->mDefaultCaptureDevice==NULL)
            {
                std::cout << "Failed to get default capture device." << std::endl;
                return;
            }

			ALCuint frequency = VOICE_CHAT_FREQUENCY;
			ALCsizei bufferSize = VOICE_CHAT_BUFFER_SIZE;

            #ifdef ENIGMA_PLATFORM_WINDOWS
            this->mCaptureDevice = alcCaptureOpenDevice(this->mDefaultCaptureDevice, frequency, AL_FORMAT_MONO16, bufferSize);
            #else
            this->mCaptureDevice = alcCaptureOpenDevice((const ALCchar*)this->mDefaultCaptureDevice, frequency, AL_FORMAT_MONO16, bufferSize);
            #endif
            this->CheckForOpenALError(true,"alcCaptureOpenDevice");

            if(this->mCaptureDevice==NULL)
            {
                std::cout << "Failed to get capture device." << std::endl;
            }
			else
			{
				std::cout << "Obtained capture device with frquency of " << frequency << " and a buffer size of " << bufferSize << "." << std::endl;
			}
        }
        else
        {
            std::cout << "Failed to set default audio context." << std::endl;
        }
        this->CheckForOpenALError(true,"alcMakeContextCurrent");
    }

    void OpenALAudioDevice::Load()
    {
        if(!this->mIsLoaded)
        {
            this->mIsLoaded=true;
            if(this->mDevice!=NULL && this->mContext!=NULL)
            {
                this->GenerateSources();
            }
        }
    }

    void OpenALAudioDevice::Unload()
    {
        if(this->mIsLoaded)
        {
            this->mIsLoaded=false;
            //this->mSoundSources.clear();
        }
    }

    void OpenALAudioDevice::Poll()
    {
		BOOST_FOREACH(SoundSourcePair source,this->mAllocatedSources)
		{
			source.second.Poll();
		}
    }

	void OpenALAudioDevice::Capture()
	{
        if(this->mIsLoaded)
        {
			while(this->CaptureAudio())
			{
				//empty captured samples before moving on.
			}
        }
	}

    void OpenALAudioDevice::GenerateSources()
    {
        ALuint source;
        std::cout << "please wait detecting audio sources." << std::endl;
        while(alGetError()==AL_NO_ERROR && this->mSoundSources.size()<NUMBER_OF_AUDIO_SOURCES)
        {
            alGenSources(1,&source);
			//this->CheckForOpenALError(true,"alGenSources");
            if(alIsSource(source))
            {
				//this->CheckForOpenALError(true,"alIsSource");
                this->mSoundSources.push(SoundSource(source));
            }
            else
            {
                std::cout << "An invalid source was generated!" << std::endl;
            }
        }

        std::cout << this->mSoundSources.size() << " audio sources detected." << std::endl;

        int numberOfChatSources=0;
        SoundSource voiceChatSoundSource;
        numberOfChatSources=(this->mSoundSources.size() / 7) * 3; //sorry thinking about Job.
        if(numberOfChatSources<=0)
        {
            numberOfChatSources=1;
        }

        for(int i=0;i<numberOfChatSources;i++)
        {
            voiceChatSoundSource = this->mSoundSources.top();
			voiceChatSoundSource.SetAudioDecoder(new SpeexDecoder()); //sources are kept until application end so leaks shouldn't be an issue.
            this->mVoiceChatSources.push(voiceChatSoundSource);
            this->mSoundSources.pop();
        }

        std::cout << this->mSoundSources.size() << " audio sources used for sounds." << std::endl;
        std::cout << this->mVoiceChatSources.size() << " audio sources used for voice chat." << std::endl;
    }

    void OpenALAudioDevice::CheckForOpenALError(bool nothrow,const Enigma::c8* functionName)
    {
        ALenum error=alGetError();
        switch(error)
        {
            case AL_INVALID_NAME:
                if(!nothrow)
                {
                    throw EnigmaException("AL_INVALID_NAME");
                }
                else
                {
                    std::cout << functionName << " caused a " << "AL_INVALID_NAME" << " error." << std::endl;
                }
            break;
            case AL_INVALID_ENUM:
                if(!nothrow)
                {
                    throw EnigmaException("AL_INVALID_ENUM");
                }
                else
                {
                    std::cout << functionName << " caused a " << "AL_INVALID_ENUM" << std::endl;
                }
            break;
            case AL_INVALID_VALUE:
                if(!nothrow)
                {
                    throw EnigmaException("AL_INVALID_VALUE");
                }
                else
                {
                    std::cout << functionName << " caused a " << "AL_INVALID_VALUE" << std::endl;
                }
            break;
            case AL_INVALID_OPERATION:
                if(!nothrow)
                {
                    throw EnigmaException("AL_INVALID_OPERATION");
                }
                else
                {
                    std::cout << functionName << " caused a " << "AL_INVALID_OPERATION" << std::endl;
                }
            break;
            case AL_OUT_OF_MEMORY:
                if(!nothrow)
                {
                    throw EnigmaException("AL_OUT_OF_MEMORY");
                }
                else
                {
                    std::cout << "AL_OUT_OF_MEMORY" << std::endl;
                }
            break;
            case AL_NO_ERROR:
                //do nothing its fine.
            break;
            default:
                //this means the api has changed.
                throw EnigmaException("unknown openal exception.");
            break;
        };
    }

    //playback
    void OpenALAudioDevice::StreamAtLocation(const Enigma::Entity& entity,Enigma::u8* buffer,int priority,size_t encodedSize)
	{
		if(this->mDevice!=NULL && this->mContext!=NULL)
		{
			if(this->mAllocatedSources.find(entity.GetUniqueId())==this->mAllocatedSources.end())
			{
				if(!this->mVoiceChatSources.empty())
				{
					SoundSource soundSource=this->mVoiceChatSources.top();

					this->mVoiceChatSources.pop();

					this->mAllocatedSources[entity.GetUniqueId()] = soundSource;

					//call self because this time the container will have an entry for this entity.
					this->StreamAtLocation(entity,buffer,priority,encodedSize);
				}
				else
				{
					BOOST_FOREACH(SoundSourcePair source,this->mAllocatedSources)
					{
						if(!source.second.GetIsBusy() && source.second.GetIsOutOfBuffers())
						{
							source.first = entity.GetUniqueId();
							source.second.SetTimestamp(0); //reset so jitter buffer doesn't mess up.
							break;
						}
					}
					//call self because this time the container will have an entry for this entity.
					this->StreamAtLocation(entity,buffer,priority,encodedSize);
				}
			}
			else
			{
				
				this->mAllocatedSources[entity.GetUniqueId()].SetPriority(priority);
				this->mAllocatedSources[entity.GetUniqueId()].UpdateSourcePosition(entity);
				this->mAllocatedSources[entity.GetUniqueId()].IncrementTimestamp();
				this->mAllocatedSources[entity.GetUniqueId()].QueueBuffer(buffer,encodedSize);
			}
		}
    }

    void OpenALAudioDevice::StreamVoiceAtLocation(const Enigma::Entity& entity,Enigma::u8* buffer,size_t encodedSize)
    {
        this->StreamAtLocation(entity,buffer,0,encodedSize);
    }

    void OpenALAudioDevice::PlayAtLocation(const Enigma::Entity& entity,const std::string& filename,int priority)
    {
        if(this->mDevice!=NULL && this->mContext!=NULL)
        {
            SoundSource soundSource=this->mSoundSources.top();

            this->mSoundSources.pop();

            soundSource.SetPriority(priority);
            soundSource.Stop();
            soundSource.UpdateSourcePosition(entity);
            soundSource.Play(filename);

            this->mSoundSources.push(soundSource);
        }
    }

    void OpenALAudioDevice::PlayAtLocation(const Enigma::Entity& entity,Enigma::c8* filename,int priority)
    {
        if(this->mDevice!=NULL && this->mContext!=NULL)
        {
            SoundSource soundSource=this->mSoundSources.top();

            this->mSoundSources.pop();

            soundSource.SetPriority(priority);
            soundSource.Stop();
            soundSource.UpdateSourcePosition(entity);
            soundSource.Play(filename);

            this->mSoundSources.push(soundSource);
        }
    }

    void OpenALAudioDevice::PlayAtLocation(const Enigma::Entity& entity,const Enigma::c8* filename,int priority)
    {
        if(this->mDevice!=NULL && this->mContext!=NULL)
        {
            SoundSource soundSource=this->mSoundSources.top();

            this->mSoundSources.pop();

            soundSource.SetPriority(priority);
            soundSource.Stop();
            soundSource.UpdateSourcePosition(entity);
            soundSource.Play(filename);

            this->mSoundSources.push(soundSource);
        }
    }

    void OpenALAudioDevice::UpdateListenerValues(const Enigma::Entity& entity)
    {
        if(this->mDevice!=NULL && this->mContext!=NULL)
        {
            ALfloat listenerPosition[3];
            ALfloat listenerVelocity[3];
            // Orientation of the listener. (first 3 elements are "at", second 3 are "up")
            ALfloat listenerOrientation[6];

            //velocity
            listenerVelocity[0] = entity.GetVelocityX(); 
            listenerVelocity[1] = entity.GetVelocityY();
            listenerVelocity[2] = entity.GetVelocityZ();
            //position
            listenerPosition[0] = entity.GetX(); 
            listenerPosition[1] = entity.GetY();
            listenerPosition[2] = -entity.GetZ();
            //at
            listenerOrientation[0] = entity.GetTargetX() - entity.GetX();
            listenerOrientation[1] = entity.GetTargetY() - entity.GetY();
            listenerOrientation[2] = -(entity.GetTargetZ() - entity.GetZ());
            //up
			listenerOrientation[3] = 0.0f; 
            listenerOrientation[4] = 1.0f;
            listenerOrientation[5] = 0.0f; //listenerOrientation[5] = -Entity::UpVectorZ;

            alListenerfv(AL_POSITION,listenerPosition);
            this->CheckForOpenALError(true,"alListenerfv");

            alListenerfv(AL_VELOCITY,listenerVelocity);
            this->CheckForOpenALError(true,"alListenerfv");

            alListenerfv(AL_ORIENTATION,listenerOrientation);
            this->CheckForOpenALError(true,"alListenerfv");
        }
    }

    //recording
    void OpenALAudioDevice::StartRecording()
    {
        if (this->mDevice!=NULL && this->mContext!=NULL && this->mCaptureDevice!=NULL)
        {
            alcCaptureStart(this->mCaptureDevice);
            this->CheckForOpenALError(true,"alcCaptureStart");
            std::cout << "Started recording." << std::endl;
        }
    }

    void OpenALAudioDevice::StopRecording()
    {
        if (this->mDevice!=NULL && this->mContext!=NULL && this->mCaptureDevice!=NULL)
        {
            alcCaptureStop(this->mCaptureDevice);
            this->CheckForOpenALError(true,"alcCaptureStop");
            std::cout << "Stopped recording." << std::endl;
        }

		//send encoded data to server.
		Enigma::u32 EndStream=0;
		this->mClientTransmissionManager.GetClientSessionManager().SendVoiceData((Enigma::u8*)&EndStream,sizeof(Enigma::u32));
    }

	bool OpenALAudioDevice::CaptureAudio()
	{
        if(this->mDevice!=NULL && this->mContext!=NULL && this->mCaptureDevice!=NULL)
        {
			Enigma::u8 buffer[VOICE_CHAT_BUFFER_SIZE];

            alcGetIntegerv(this->mCaptureDevice, ALC_CAPTURE_SAMPLES, 1, &mAvailableSamples);
            this->CheckForOpenALError(true,"alcGetIntegerv");

            if (this->mAvailableSamples > (VOICE_CHAT_BUFFER_SIZE / this->mBlockAlign))
            {
                alcCaptureSamples(this->mCaptureDevice, buffer, VOICE_CHAT_BUFFER_SIZE / this->mBlockAlign);
                this->CheckForOpenALError(true,"alcCaptureSamples");

				if(this->mAudioEncoder!=NULL)
				{
					Enigma::u8* encodedBuffer=NULL;
					size_t encodedSize=0;

					//encode voice data.
					encodedBuffer = this->mAudioEncoder->Encode(buffer,VOICE_CHAT_BUFFER_SIZE,encodedSize);

					//send encoded data to server.
					this->mClientTransmissionManager.GetClientSessionManager().SendVoiceData(encodedBuffer,encodedSize);

					//clean up buffer.
					delete[] encodedBuffer;
				}
				
				return true;
            }
		}

		return false;
	}
};
