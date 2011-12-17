#ifndef NULLAUDIODEVICE_HPP_INCLUDED
#define NULLAUDIODEVICE_HPP_INCLUDED

/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "IAudioDevice.hpp"

namespace Enigma
{
    class DllExport NullAudioDevice : public IAudioDevice
    {
        private:

        protected:

        public:

        NullAudioDevice();
        ~NullAudioDevice();

        virtual void Preinit();
        virtual void Init(int argc, Enigma::c8** argv);
        virtual void Load();
        virtual void Unload();

        virtual void Poll();
		virtual void Capture();

        //playback
        virtual void StreamAtLocation(const Enigma::Entity& entity, Enigma::u8* buffer,int priority,size_t encodedSize);
        virtual void StreamAtLocation(const Enigma::Entity& entity,unsigned int buffer,int priority);

        virtual void StreamVoiceAtLocation(const Enigma::Entity& entity,Enigma::u8* encodedBuffer,size_t encodedSize);
        virtual void StreamVoiceAtLocation(const Enigma::Entity& entity,unsigned int buffer);

        virtual void PlayAtLocation(const Enigma::Entity& entity,const std::string& filename,int priority);
        virtual void PlayAtLocation(const Enigma::Entity& entity,Enigma::c8* filename,int priority);
        virtual void PlayAtLocation(const Enigma::Entity& entity,const Enigma::c8* filename,int priority);

        virtual void UpdateListenerValues(const Enigma::Entity& entity);

        //recording
        virtual void StartRecording();
        virtual void StopRecording();

    };
};

#endif // NULLAUDIODEVICE_HPP_INCLUDED
