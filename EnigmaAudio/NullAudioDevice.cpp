/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "NullAudioDevice.hpp"

namespace Enigma
{
    NullAudioDevice::NullAudioDevice()
    {
        std::cout << "NullAudioDevice::NullAudioDevice()" << std::endl;
    }

    NullAudioDevice::~NullAudioDevice()
    {
        std::cout << "NullAudioDevice::~NullAudioDevice()" << std::endl;
    }

    void NullAudioDevice::Preinit()
    {
        std::cout << "NullAudioDevice::Preinit()" << std::endl;
    }

    void NullAudioDevice::Init(int argc, Enigma::c8** argv)
    {
        std::cout << "NullAudioDevice::Init(int, Enigma::c8**)" << std::endl;
    }

    void NullAudioDevice::Load()
    {
        std::cout << "NullAudioDevice::Load()" << std::endl;
    }

    void NullAudioDevice::Unload()
    {
        std::cout << "NullAudioDevice::Unload()" << std::endl;
    }

    void NullAudioDevice::Poll()
    {
        std::cout << "NullAudioDevice::Poll()" << std::endl;
    }

	void NullAudioDevice::Capture()
	{
		std::cout << "NullAudioDevice::Capture()" << std::endl;
	}

    //playback
    void NullAudioDevice::StreamAtLocation(const Enigma::Entity& entity,Enigma::u8* buffer,int priority,size_t encodedSize)
    {
        std::cout << "NullAudioDevice::StreamAtLocation(const Enigma::Entity& entity, Enigma::u8*,int,size_t)" << std::endl;
    }

    void NullAudioDevice::StreamAtLocation(const Enigma::Entity& entity,unsigned int buffer,int priority)
    {
        std::cout << "NullAudioDevice::StreamAtLocation(const Enigma::Entity& entity,unsigned int,int)" << std::endl;
    }

    void NullAudioDevice::StreamVoiceAtLocation(const Enigma::Entity& entity,Enigma::u8* encodedBuffer,size_t encodedSize)
    {
        std::cout << "NullAudioDevice::StreamVoiceAtLocation(const Enigma::Entity& entity, Enigma::u8*,size_t)" << std::endl;
    }

    void NullAudioDevice::StreamVoiceAtLocation(const Enigma::Entity& entity,unsigned int buffer)
    {
        std::cout << "NullAudioDevice::StreamVoiceAtLocation(const Enigma::Entity& entity,unsigned int)" << std::endl;
    }

    void NullAudioDevice::PlayAtLocation(const Enigma::Entity& entity,const std::string& filename,int priority)
    {
        std::cout << "NullAudioDevice::PlayAtLocation(const Enigma::Entity& entity,std::string&,int)" << std::endl;
    }

    void NullAudioDevice::PlayAtLocation(const Enigma::Entity& entity,Enigma::c8* filename,int priority)
    {
        std::cout << "NullAudioDevice::PlayAtLocation(const Enigma::Entity& entity,Enigma::c8*,int)" << std::endl;
    }

    void NullAudioDevice::PlayAtLocation(const Enigma::Entity& entity,const Enigma::c8* filename,int priority)
    {
        std::cout << "NullAudioDevice::PlayAtLocation(const Enigma::Entity& entity,const Enigma::c8*,int)" << std::endl;
    }

    void NullAudioDevice::UpdateListenerValues(const Enigma::Entity& entity)
    {
        std::cout << "NullAudioDevice::UpdateListenerValues(const Enigma::Entity& entity)" << std::endl;
    }

    //recording
    void NullAudioDevice::StartRecording()
    {
        std::cout << "NullAudioDevice::StartRecording()" << std::endl;
    }

    void NullAudioDevice::StopRecording()
    {
        std::cout << "NullAudioDevice::StopRecording()" << std::endl;
    }
};
