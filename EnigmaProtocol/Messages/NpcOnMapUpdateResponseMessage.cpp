/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "NpcOnMapUpdateResponseMessage.hpp"

namespace Enigma
{

    NpcOnMapUpdateResponseMessage::NpcOnMapUpdateResponseMessage()
    {
        this->SetName("");
        this->SetType(NpcOnMapUpdateResponseMessage::GetMessageType());

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

    NpcOnMapUpdateResponseMessage::~NpcOnMapUpdateResponseMessage()
    {

    }

    Enigma::s32 NpcOnMapUpdateResponseMessage::GetId()
    {
        return this->GetInt(0);
    }

    void NpcOnMapUpdateResponseMessage::SetId(Enigma::s32 value)
    {
        this->SetInt(0,value);
    }

    bool NpcOnMapUpdateResponseMessage::GetIsLeavingMap()
    {
        return this->GetInt(1)==1;
    }

    void NpcOnMapUpdateResponseMessage::SetIsLeavingMap(bool value)
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

    bool NpcOnMapUpdateResponseMessage::GetIsEnteringMap()
    {
        return this->GetInt(1)==2;
    }

    void NpcOnMapUpdateResponseMessage::SetIsEnteringMap(bool value)
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

    Enigma::s32 NpcOnMapUpdateResponseMessage::GetGender()
    {
        return this->GetInt(2);
    }

    void NpcOnMapUpdateResponseMessage::SetGender(Enigma::s32 value)
    {
        this->SetInt(2,value);
    }

    Enigma::f32 NpcOnMapUpdateResponseMessage::GetHealth()
    {
        return this->GetFloat(3);
    }

    void NpcOnMapUpdateResponseMessage::SetHealth(Enigma::f32 value)
    {
        this->SetFloat(3,value);
    }

    Enigma::s32 NpcOnMapUpdateResponseMessage::GetModelId()
    {
        return this->GetInt(4);
    }

    void NpcOnMapUpdateResponseMessage::SetModelId(Enigma::s32 value)
    {
        this->SetInt(4,value);
    }

    Enigma::f32 NpcOnMapUpdateResponseMessage::GetX()
    {
        return this->GetFloat(5);
    }

    void NpcOnMapUpdateResponseMessage::SetX(Enigma::f32 value)
    {
        this->SetFloat(5,value);
    }

    Enigma::f32 NpcOnMapUpdateResponseMessage::GetY()
    {
        return this->GetFloat(6);
    }

    void NpcOnMapUpdateResponseMessage::SetY(Enigma::f32 value)
    {
        this->SetFloat(6,value);
    }

    Enigma::f32 NpcOnMapUpdateResponseMessage::GetZ()
    {
        return this->GetFloat(7);
    }

    void NpcOnMapUpdateResponseMessage::SetZ(Enigma::f32 value)
    {
        this->SetFloat(7,value);
    }

    const std::string& NpcOnMapUpdateResponseMessage::GetName()
    {
        this->mName = this->GetString(8);
        return this->mName;
    }

    void NpcOnMapUpdateResponseMessage::SetName(const std::string& value)
    {
        this->mName=value;
        this->ResizeMessage(this->mName.length()+NpcOnMapUpdateResponseMessage::GetMessageLength(),0);
        this->SetString(8,this->mName);
    }
};
