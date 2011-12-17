/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "VoiceChatRequestMessage.hpp"

namespace Enigma
{
    VoiceChatRequestMessage::VoiceChatRequestMessage()
    {
        size_t size;
        size = VoiceChatRequestMessage::GetMessageLength();
        this->ResizeMessage(size,0);
        this->SetType(VoiceChatRequestMessage::GetMessageType());
        //this->SetReliable(false);

		//this->SetChannel(CHANNEL_AUTHENTICATION);
		//this->SetChannel(CHANNEL_MOVEMENT);
		//this->SetChannel(CHANNEL_COMBAT);
		//this->SetChannel(CHANNEL_CHAT);
		this->SetChannel(CHANNEL_VOICE_CHAT);

		this->SetCombatantId(0);
		this->SetVoiceChannel(CHAT_TYPE_MAP);
		//this->SetVoiceChannel(CHAT_TYPE_GUILD);
		//this->SetVoiceChannel(CHAT_TYPE_PARTY);
		//this->SetVoiceChannel(CHAT_TYPE_BROADCAST);
    }

    VoiceChatRequestMessage::~VoiceChatRequestMessage()
    {

    }

    Enigma::s32 VoiceChatRequestMessage::GetCombatantId()
    {
        return this->GetInt(0);
    }

    void VoiceChatRequestMessage::SetCombatantId(Enigma::s32 value)
    {
        this->SetInt(0,value);
    }

    Enigma::s32 VoiceChatRequestMessage::GetVoiceChannel()
	{
		return this->GetInt(1);
	}

    void VoiceChatRequestMessage::SetVoiceChannel(Enigma::s32 value)
	{
		this->SetInt(1,value);
	}

    Enigma::u8* VoiceChatRequestMessage::GetAudioData(size_t& size)
    {
        return this->GetBytes(2,size);
    }

    void VoiceChatRequestMessage::SetAudioData(Enigma::u8* value,size_t size)
    {
		//store combatId and voiceChannel because resize will not copy them.
		Enigma::s32 combatId=this->GetCombatantId();
		Enigma::s32 voiceChannel=this->GetVoiceChannel();

		//set bytes which resizes the message buffer. Note only base message container values are copied.
		this->SetBytes(2,size,value);

		//restore previously stored message values to the new buffer.
		this->SetCombatantId(0);
		this->SetVoiceChannel(CHAT_TYPE_MAP);        
    }

    s64 VoiceChatRequestMessage::GetAudioCheckSum()
    {
        s64 checksum=0;
        Enigma::u8* target=NULL;
		size_t size=0;

        target = this->GetBytes(2,size);

        for(int i=0;i<size;i++)
        {
            checksum+=target[i];
        }

        return checksum;
    }
};
