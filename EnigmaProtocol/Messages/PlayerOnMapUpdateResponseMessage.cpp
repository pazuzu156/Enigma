/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "PlayerOnMapUpdateResponseMessage.hpp"

namespace Enigma
{

    PlayerOnMapUpdateResponseMessage::PlayerOnMapUpdateResponseMessage()
    {
        this->SetName("");
        this->SetType(PlayerOnMapUpdateResponseMessage::GetMessageType());

        this->SetId(0);
        this->SetIsLeavingMap(false);
        this->SetGender(0);
        this->SetHealth(0.0f);
        this->SetModelId(0);

		//this->SetChannel(CHANNEL_AUTHENTICATION);
		this->SetChannel(CHANNEL_MOVEMENT);
		//this->SetChannel(CHANNEL_COMBAT);
		//this->SetChannel(CHANNEL_CHAT);
		//this->SetChannel(CHANNEL_VOICE_CHAT);	
	}

    PlayerOnMapUpdateResponseMessage::~PlayerOnMapUpdateResponseMessage()
    {

    }

    Enigma::s32 PlayerOnMapUpdateResponseMessage::GetId()
    {
        return this->GetInt(0);
    }

    void PlayerOnMapUpdateResponseMessage::SetId(Enigma::s32 value)
    {
        this->SetInt(0,value);
    }

    bool PlayerOnMapUpdateResponseMessage::GetIsLeavingMap()
    {
        return this->GetInt(1)==1;
    }

    void PlayerOnMapUpdateResponseMessage::SetIsLeavingMap(bool value)
    {
        if(value)
        {
            this->SetInt(1,1);
        }
        else
        {
            this->SetInt(1,0);
        }
    }

    bool PlayerOnMapUpdateResponseMessage::GetIsEnteringMap()
    {
        return this->GetInt(1)==2;
    }

    void PlayerOnMapUpdateResponseMessage::SetIsEnteringMap(bool value)
    {
        if(value)
        {
            this->SetInt(1,2);
        }
        else
        {
            this->SetInt(1,0);
        }
    }

    Enigma::s32 PlayerOnMapUpdateResponseMessage::GetGender()
    {
        return this->GetInt(2);
    }

    void PlayerOnMapUpdateResponseMessage::SetGender(Enigma::s32 value)
    {
        this->SetInt(2,value);
    }

    Enigma::f32 PlayerOnMapUpdateResponseMessage::GetHealth()
    {
        return this->GetFloat(3);
    }

    void PlayerOnMapUpdateResponseMessage::SetHealth(Enigma::f32 value)
    {
        this->SetFloat(3,value);
    }

    Enigma::s32 PlayerOnMapUpdateResponseMessage::GetModelId()
    {
        return this->GetInt(4);
    }

    void PlayerOnMapUpdateResponseMessage::SetModelId(Enigma::s32 value)
    {
        this->SetInt(4,value);
    }

    Enigma::f32 PlayerOnMapUpdateResponseMessage::GetX()
    {
        return this->GetFloat(5);
    }

    void PlayerOnMapUpdateResponseMessage::SetX(Enigma::f32 value)
    {
        this->SetFloat(5,value);
    }

    Enigma::f32 PlayerOnMapUpdateResponseMessage::GetY()
    {
        return this->GetFloat(6);
    }

    void PlayerOnMapUpdateResponseMessage::SetY(Enigma::f32 value)
    {
        this->SetFloat(6,value);
    }

    Enigma::f32 PlayerOnMapUpdateResponseMessage::GetZ()
    {
        return this->GetFloat(7);
    }

    void PlayerOnMapUpdateResponseMessage::SetZ(Enigma::f32 value)
    {
        this->SetFloat(7,value);
    }

    const std::string& PlayerOnMapUpdateResponseMessage::GetName()
    {
        this->mName = this->GetString(8);
        return this->mName;
    }

    void PlayerOnMapUpdateResponseMessage::SetName(const std::string& value)
    {
        this->mName=value;
        this->ResizeMessage(this->mName.length()+PlayerOnMapUpdateResponseMessage::GetMessageLength(),0);
        this->SetString(8,this->mName);
    }
};
