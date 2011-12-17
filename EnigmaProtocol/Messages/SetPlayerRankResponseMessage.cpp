/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "SetPlayerRankResponseMessage.hpp"

namespace Enigma
{
    SetPlayerRankResponseMessage::SetPlayerRankResponseMessage()
    {
        this->ResizeMessage(SetPlayerRankResponseMessage::GetMessageLength(),0);
        this->SetType(SetPlayerRankResponseMessage::GetMessageType());

		//this->SetChannel(CHANNEL_AUTHENTICATION);
		//this->SetChannel(CHANNEL_MOVEMENT);
		//this->SetChannel(CHANNEL_COMBAT);
		this->SetChannel(CHANNEL_CHAT);
		//this->SetChannel(CHANNEL_VOICE_CHAT);
    }

    SetPlayerRankResponseMessage::~SetPlayerRankResponseMessage()
    {

    }

    Enigma::s32 SetPlayerRankResponseMessage::GetOrganizationType()
    {
        return mOrganizationType;
    }

    void SetPlayerRankResponseMessage::SetOrganizationType(Enigma::s32 value)
    {
        this->mOrganizationType=value;
    }

    std::string& SetPlayerRankResponseMessage::GetRankName()
    {
        return this->mRankName;
    }

    void SetPlayerRankResponseMessage::SetRankName(std::string& value)
    {
        this->mRankName=value;
    }

    void SetPlayerRankResponseMessage::SetRankName(const std::string& value)
    {
        this->mRankName=value;
    }

    std::string& SetPlayerRankResponseMessage::GetPlayerName()
    {
        return this->mPlayerName;
    }

    void SetPlayerRankResponseMessage::SetPlayerName(std::string& value)
    {
       this->mPlayerName=value;
    }

    void SetPlayerRankResponseMessage::SetPlayerName(const std::string& value)
    {
       this->mPlayerName=value;
    }

    void SetPlayerRankResponseMessage::Update()
    {
        std::string temp;
        temp.append(this->mRankName);
        temp.append("|");
        temp.append(this->mPlayerName);

        this->ResizeMessage(SetPlayerRankResponseMessage::GetMessageLength()+temp.length()+1,0);

        this->SetInt(0,this->mOrganizationType);
        this->SetString(1,temp);
    }

    void SetPlayerRankResponseMessage::Populate()
    {
        Enigma::c8* temp;
        Enigma::c8* temp2;
        this->mOrganizationType = this->GetInt(0);
        temp=this->GetString(1);

        temp2=strtok(temp,"|");
        if(temp2==NULL)
        {
            temp2="";
        }
        this->mRankName=temp2;

        temp2=strtok(NULL,"|");
        if(temp2==NULL)
        {
            temp2="";
        }
        this->mPlayerName = temp2;
    }
};
