#ifndef IAUDIODEVICE_HPP_INCLUDED
#define IAUDIODEVICE_HPP_INCLUDED

/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "std_map.hpp"
#include "iostream.hpp"
#include "string.hpp"

#include "boost_foreach.hpp"

#include "Entities\Entity.hpp"

#include "IAudioDecoder.hpp"
#include "IAudioEncoder.hpp"

#include "ClientTransmissionManager.hpp"
#include "ClientConfigurationManager.hpp"

namespace Enigma
{
    class DllExport IAudioDevice
    {
        private:

        protected:

		IAudioDecoder* mAudioDecoder;
		IAudioEncoder* mAudioEncoder;

        public:

        virtual ~IAudioDevice(){};

        virtual void Preinit()=0;
        virtual void Init(int argc, Enigma::c8** argv)=0;
        virtual void Load()=0;
        virtual void Unload()=0;

        virtual void Poll()=0;
		virtual void Capture()=0;

		virtual void SetEncoderAndDecoder(IAudioDecoder* audioDecoder, IAudioEncoder* audioEncoder)
		{
			mAudioDecoder=audioDecoder;
			mAudioEncoder=audioEncoder;
		}

        //playback
		virtual void StreamAtLocation(const Enigma::Entity& entity,Enigma::u8* buffer,int priority,size_t encodedSize)=0;

        virtual void StreamVoiceAtLocation(const Enigma::Entity& entity,Enigma::u8* encodedBuffer,size_t encodedSize)=0;

        virtual void PlayAtLocation(const Enigma::Entity& entity,const std::string& filename,int priority)=0;
        virtual void PlayAtLocation(const Enigma::Entity& entity,Enigma::c8* filename,int priority)=0;
        virtual void PlayAtLocation(const Enigma::Entity& entity,const Enigma::c8* filename,int priority)=0;

        virtual void UpdateListenerValues(const Enigma::Entity& entity)=0;

        //recording
        virtual void StartRecording()=0;
        virtual void StopRecording()=0;
    };
};

#endif // IAUDIODEVICE_HPP_INCLUDED
