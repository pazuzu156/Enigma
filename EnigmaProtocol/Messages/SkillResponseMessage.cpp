/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "SkillResponseMessage.hpp"

#include "boost/lexical_cast.hpp"

namespace Enigma
{
    SkillResponseMessage::SkillResponseMessage()
    {
        this->ResizeMessage(SkillResponseMessage::GetMessageLength(),0);
        this->SetType(SkillResponseMessage::GetMessageType());

		//this->SetChannel(CHANNEL_AUTHENTICATION);
		//this->SetChannel(CHANNEL_MOVEMENT);
		this->SetChannel(CHANNEL_COMBAT);
		//this->SetChannel(CHANNEL_CHAT);
		//this->SetChannel(CHANNEL_VOICE_CHAT);
    }

    SkillResponseMessage::~SkillResponseMessage()
    {

    }

    Enigma::s32 SkillResponseMessage::GetSkillId()
    {
        return this->GetInt(0);
    }

    void SkillResponseMessage::SetSkillId(Enigma::s32 value)
    {
        this->SetInt(0,value);
    }

    void SkillResponseMessage::SetSkillId(std::string value)
    {
       this->SetSkillId(boost::lexical_cast<Enigma::s32>(value));
    }


    Enigma::s32 SkillResponseMessage::GetTargetId()
    {
        return this->GetInt(1);
    }

    void SkillResponseMessage::SetTargetId(Enigma::s32 value)
    {
        this->SetInt(1,value);
    }

    void SkillResponseMessage::SetTargetId(std::string value)
    {
       this->SetTargetId(boost::lexical_cast<Enigma::s32>(value));
    }


    Enigma::s32 SkillResponseMessage::GetSkillLevel()
    {
        return this->GetInt(2);
    }

    void SkillResponseMessage::SetSkillLevel(Enigma::s32 value)
    {
        this->SetInt(2,value);
    }

    void SkillResponseMessage::SetSkillLevel(std::string value)
    {
       this->SetSkillLevel(boost::lexical_cast<Enigma::s32>(value));
    }
};