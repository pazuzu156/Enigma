/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "ModifyPlayerOrganizationRankResponseMessage.hpp"

namespace Enigma
{

    ModifyPlayerOrganizationRankResponseMessage::ModifyPlayerOrganizationRankResponseMessage()
    {
        this->SetName("");
        this->SetType(ModifyPlayerOrganizationRankResponseMessage::GetMessageType());

		//this->SetChannel(CHANNEL_AUTHENTICATION);
		//this->SetChannel(CHANNEL_MOVEMENT);
		//this->SetChannel(CHANNEL_COMBAT);
		this->SetChannel(CHANNEL_CHAT);
		//this->SetChannel(CHANNEL_VOICE_CHAT);	
	}

    ModifyPlayerOrganizationRankResponseMessage::~ModifyPlayerOrganizationRankResponseMessage()
    {

    }

    Enigma::s32 ModifyPlayerOrganizationRankResponseMessage::GetOrganizationType()
    {
        return this->GetInt(0);
    }

    void ModifyPlayerOrganizationRankResponseMessage::SetOrganizationType(Enigma::s32 value)
    {
        this->SetInt(0,value);
    }

    Enigma::s32 ModifyPlayerOrganizationRankResponseMessage::GetPermissions()
    {
        return this->GetInt(1);
    }

    void ModifyPlayerOrganizationRankResponseMessage::SetPermissions(Enigma::s32 value)
    {
        this->SetInt(1,value);
    }

    const std::string& ModifyPlayerOrganizationRankResponseMessage::GetName()
    {
        this->mName = this->GetString(2);
        return this->mName;
    }

    void ModifyPlayerOrganizationRankResponseMessage::SetName(const std::string& value)
    {
        this->mName=value;
        this->ResizeMessage(this->mName.length()+ModifyPlayerOrganizationRankResponseMessage::GetMessageLength(),0);
        this->SetString(2,this->mName);
    }

    void ModifyPlayerOrganizationRankResponseMessage::SetName(std::string& value)
    {
        this->mName=value;
        this->ResizeMessage(this->mName.length()+ModifyPlayerOrganizationRankResponseMessage::GetMessageLength(),0);
        this->SetString(2,this->mName);
    }
};
