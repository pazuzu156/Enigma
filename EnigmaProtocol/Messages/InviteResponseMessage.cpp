/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "InviteResponseMessage.hpp"

namespace Enigma
{

    InviteResponseMessage::InviteResponseMessage()
    {
        this->SetName("");
        this->SetType(InviteResponseMessage::GetMessageType());

		//this->SetChannel(CHANNEL_AUTHENTICATION);
		//this->SetChannel(CHANNEL_MOVEMENT);
		//this->SetChannel(CHANNEL_COMBAT);
		this->SetChannel(CHANNEL_CHAT);
		//this->SetChannel(CHANNEL_VOICE_CHAT);	
	}

    InviteResponseMessage::~InviteResponseMessage()
    {

    }

    Enigma::s32 InviteResponseMessage::GetInviteType()
    {
        return this->GetInt(0);
    }

    void InviteResponseMessage::SetInviteType(Enigma::s32 value)
    {
        this->SetInt(0,value);
    }

    Enigma::s32 InviteResponseMessage::GetInviteId()
    {
        return this->GetInt(1);
    }

    void InviteResponseMessage::SetInviteId(Enigma::s32 value)
    {
        this->SetInt(1,value);
    }

    const std::string& InviteResponseMessage::GetName()
    {
        this->mName = this->GetString(2);
        return this->mName;
    }

    void InviteResponseMessage::SetName(const std::string& value)
    {
        this->mName=value;
        this->ResizeMessage(this->mName.length()+InviteResponseMessage::GetMessageLength(),0);
        this->SetString(2,this->mName);
    }

    void InviteResponseMessage::SetName(std::string& value)
    {
        this->mName=value;
        this->ResizeMessage(this->mName.length()+InviteResponseMessage::GetMessageLength(),0);
        this->SetString(2,this->mName);
    }
};
