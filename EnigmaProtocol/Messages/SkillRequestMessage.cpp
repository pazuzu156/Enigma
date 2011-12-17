/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "SkillRequestMessage.hpp"

#include "boost/lexical_cast.hpp"

namespace Enigma
{
    SkillRequestMessage::SkillRequestMessage()
    {
        this->ResizeMessage(SkillRequestMessage::GetMessageLength(),0);
        this->SetType(SkillRequestMessage::GetMessageType());
		//init
		this->SetSkillId(0);
		this->SetSkillLevel(0);
		this->SetTargetCharacterId(0);
		this->SetTargetNpcId(0);
		this->SetTargetMonsterId(0);
		this->SetTargetPreferenceType(TARGETPREFERENCETYPE_NONE);

		//this->SetChannel(CHANNEL_AUTHENTICATION);
		//this->SetChannel(CHANNEL_MOVEMENT);
		this->SetChannel(CHANNEL_COMBAT);
		//this->SetChannel(CHANNEL_CHAT);
		//this->SetChannel(CHANNEL_VOICE_CHAT);
    }

    SkillRequestMessage::~SkillRequestMessage()
    {

    }

    Enigma::s32 SkillRequestMessage::GetSkillId()
    {
        return this->GetInt(0);
    }

    void SkillRequestMessage::SetSkillId(Enigma::s32 value)
    {
        this->SetInt(0,value);
    }

    void SkillRequestMessage::SetSkillId(std::string value)
    {
       this->SetSkillId(boost::lexical_cast<Enigma::s32>(value));
    }

    Enigma::s32 SkillRequestMessage::GetSkillLevel()
    {
        return this->GetInt(1);
    }

    void SkillRequestMessage::SetSkillLevel(Enigma::s32 value)
    {
        this->SetInt(1,value);
    }

    void SkillRequestMessage::SetSkillLevel(std::string value)
    {
       this->SetSkillLevel(boost::lexical_cast<Enigma::s32>(value));
    }


    Enigma::s32 SkillRequestMessage::GetTargetNpcId()
    {
        return this->GetInt(2);
    }

    void SkillRequestMessage::SetTargetNpcId(Enigma::s32 value)
    {
        this->SetInt(2,value);
    }

    void SkillRequestMessage::SetTargetNpcId(std::string value)
    {
       this->SetTargetNpcId(boost::lexical_cast<Enigma::s32>(value));
    }


	Enigma::s32 SkillRequestMessage::GetTargetMonsterId()
    {
        return this->GetInt(3);
    }

    void SkillRequestMessage::SetTargetMonsterId(Enigma::s32 value)
    {
        this->SetInt(3,value);
    }

    void SkillRequestMessage::SetTargetMonsterId(std::string value)
    {
       this->SetTargetMonsterId(boost::lexical_cast<Enigma::s32>(value));
    }


	Enigma::s32 SkillRequestMessage::GetTargetCharacterId()
    {
        return this->GetInt(4);
    }

    void SkillRequestMessage::SetTargetCharacterId(Enigma::s32 value)
    {
        this->SetInt(4,value);
    }

    void SkillRequestMessage::SetTargetCharacterId(std::string value)
    {
       this->SetTargetCharacterId(boost::lexical_cast<Enigma::s32>(value));
    }


	Enigma::s32 SkillRequestMessage::GetTargetPreferenceType()
	{
		return this->GetInt(5);
	}

    void SkillRequestMessage::SetTargetPreferenceType(Enigma::s32 value)
	{
		this->SetInt(5,value);
	}

    void SkillRequestMessage::SetTargetPreferenceType(std::string value)
	{
		this->SetTargetPreferenceType(boost::lexical_cast<Enigma::s32>(value));
	}

	Enigma::vector3d<Enigma::s32> SkillRequestMessage::GetTargetLocation()
	{
		if(this->GetTargetPreferenceType()!=TARGETPREFERENCETYPE_AREA)
		{
			throw EnigmaException("Location only valid on area targeted skills.");
		}

		Enigma::vector3d<Enigma::s32> result;

		result.X = this->GetTargetCharacterId();
		result.Y = this->GetTargetNpcId();
		result.Z = this->GetTargetMonsterId();

		return result;
	}

	void SkillRequestMessage::SetTargetLocation(Enigma::vector3d<Enigma::s32> value)
	{
		if(this->GetTargetPreferenceType()!=TARGETPREFERENCETYPE_AREA)
		{
			throw EnigmaException("Location only valid on area targeted skills.");
		}

		this->SetTargetCharacterId(value.X);
		this->SetTargetNpcId(value.Y);
		this->SetTargetMonsterId(value.Z);
	}
};