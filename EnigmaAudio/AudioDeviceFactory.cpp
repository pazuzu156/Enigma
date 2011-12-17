/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "AudioDeviceFactory.hpp"

#include "NullAudioDevice.hpp"

#ifndef PLATFORM_MAC
    #include "OpenALAudioDevice.hpp"
#endif

#include "SpeexEncoder.hpp"
#include "SpeexDecoder.hpp"

namespace Enigma
{
    IAudioDevice* AudioDeviceFactory::CreateDevice(AudioDeviceType type, AudioEncoderType encoder)
    {
        IAudioDevice* device=NULL;

        switch(type)
        {
            case OpenALAudioDeviceType:
                #ifndef PLATFORM_MAC
                    device = new OpenALAudioDevice(mClientTransmissionManager);
				#else
					device = new NullAudioDevice();
                #endif
            break;
            case NullAudioDeviceType:
                device = new NullAudioDevice();
            break;
        }

        if(device==NULL && type!=NullAudioDeviceType)
        {
            device = new NullAudioDevice();
        }

		switch(encoder)
		{
			case SpeexAudioEncoder:
				device->SetEncoderAndDecoder(new SpeexDecoder(),new SpeexEncoder());
			break;
			case NullAudioEncoder:
				device->SetEncoderAndDecoder(NULL,NULL);
			break;
		}

        return device;
    }

    IAudioDevice* AudioDeviceFactory::CreateDevice(std::string type)
    {
        if(strcmp(type.c_str(),"Null")==0)
        {
            return this->CreateDevice(NullAudioDeviceType,NullAudioEncoder);
        }
        else
        if(strcmp(type.c_str(),"OpenAL")==0)
        {
            return this->CreateDevice(OpenALAudioDeviceType,SpeexAudioEncoder);
        }

        return NULL;
    }

    IAudioDevice* AudioDeviceFactory::CreateDevice()
    {
        return this->CreateDevice(OpenALAudioDeviceType,SpeexAudioEncoder);
    }
};
