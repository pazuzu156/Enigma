/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "KickRequestMessage.hpp"

namespace Enigma
{

    KickRequestMessage::KickRequestMessage()
    {
        this->SetName("");
        this->SetType(KickRequestMessage::GetMessageType());

		//this->SetChannel(CHANNEL_AUTHENTICATION);
		//this->SetChannel(CHANNEL_MOVEMENT);
		//this->SetChannel(CHANNEL_COMBAT);
		this->SetChannel(CHANNEL_CHAT);
		//this->SetChannel(CHANNEL_VOICE_CHAT);	
	}

    KickRequestMessage::~KickRequestMessage()
    {

    }

    Enigma::s32 KickRequestMessage::GetKickType()
    {
        return this->GetInt(0);
    }

    void KickRequestMessage::SetKickType(Enigma::s32 value)
    {
        this->SetInt(0,value);
    }

    const std::string& KickRequestMessage::GetName()
    {
        this->mName = this->GetString(1);
        return this->mName;
    }

    void KickRequestMessage::SetName(const std::string& value)
    {
        this->mName=value;
        this->ResizeMessage(this->mName.length()+KickRequestMessage::GetMessageLength(),0);
        this->SetString(1,this->mName);
    }

    void KickRequestMessage::SetName(std::string& value)
    {
        this->mName=value;
        this->ResizeMessage(this->mName.length()+KickRequestMessage::GetMessageLength(),0);
        this->SetString(1,this->mName);
    }
};
