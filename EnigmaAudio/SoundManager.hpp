#ifndef SOUNDMANAGER_HPP_INCLUDED
#define SOUNDMANAGER_HPP_INCLUDED

/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "EnigmaException.hpp"

#include "AudioDeviceFactory.hpp"

#include "Entities\Entity.hpp"

#include "IAudioEventListener.hpp"

#include "ClientTransmissionManager.hpp"
#include "ClientConfigurationManager.hpp"

#include "ThreadedManager.hpp"

namespace Enigma
{
	class DllExport SoundManager : public IAudioEventListener, public ThreadedManager
    {
        private:

		ClientTransmissionManager& mClientTransmissionManager;

		bool mIsRecording;
		bool mHasCaptured;

        //bool mIsLoaded; //provided by ThreadedManager
        AudioDeviceFactory mAudioDeviceFactory;
        IAudioDevice* mAudioDevice;

		boost::posix_time::ptime mLastPoll;

		int margc; 
		Enigma::c8** margv;

        protected:

        void ReallyStartRecording();
        void ReallyStopRecording();

        public:

        SoundManager(ClientTransmissionManager& clientTransmissionManager);
        ~SoundManager();

        void Preinit();
        void Init(int argc, Enigma::c8** argv);
        void Load();
        void Unload();

		void Capture();
        void Poll();

        //playback
        void StreamAtLocation(const Enigma::Entity& entity,Enigma::u8* buffer,int priority,size_t length);

        void StreamVoiceAtLocation(const Enigma::Entity& entity,Enigma::u8* buffer,size_t length);

        void PlayAtLocation(const Enigma::Entity& entity,const std::string& filename,int priority);
        void PlayAtLocation(const Enigma::Entity& entity,Enigma::c8* filename,int priority);
        void PlayAtLocation(const Enigma::Entity& entity,const Enigma::c8* filename,int priority);

        void UpdateListenerValues(const Enigma::Entity& entity);

        //recording
        void StartRecording();
        void StopRecording();

		virtual void onVoiceReceived(Enigma::s32 characterId, const Enigma::Character& character, Enigma::u8* buffer,size_t length);
		virtual void onRecorderStateChanged(bool isRecording);
		virtual void onPlayFile(const Enigma::Entity& entity, size_t audioId, const std::string& filename);
    };
};

#endif // SOUNDMANAGER_HPP_INCLUDED
