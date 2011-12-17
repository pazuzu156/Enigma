/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "NpcChatRequestMessage.hpp"

namespace Enigma
{

    NpcChatRequestMessage::NpcChatRequestMessage()
    {
        size_t size;
        size = NpcChatRequestMessage::GetMessageLength();
        this->ResizeMessage(size,0);
        this->SetType(NpcChatRequestMessage::GetMessageType());

        this->mChatResponse="";
        this->mTargetNpcId=0;

		//this->SetChannel(CHANNEL_AUTHENTICATION);
		//this->SetChannel(CHANNEL_MOVEMENT);
		//this->SetChannel(CHANNEL_COMBAT);
		this->SetChannel(CHANNEL_CHAT);
		//this->SetChannel(CHANNEL_VOICE_CHAT);	
	}

    NpcChatRequestMessage::~NpcChatRequestMessage()
    {

    }

    Enigma::s32 NpcChatRequestMessage::GetTargetNpcId()
    {
        return this->mTargetNpcId;
    }

    void NpcChatRequestMessage::SetTargetNpcId(Enigma::s32 value)
    {
        this->mTargetNpcId=value;
    }

    const std::string& NpcChatRequestMessage::GetChatResponse() const
    {
        return this->mChatResponse;
    }

    void NpcChatRequestMessage::SetChatResponse(std::string& value)
    {
        this->mChatResponse=value;
    }

    void NpcChatRequestMessage::SetChatResponse(const std::string& value)
    {
        this->mChatResponse=value;
    }

    void NpcChatRequestMessage::UpdateMessageData()
    {
        this->SetInt(0,this->mTargetNpcId);

		std::string temp="";
        temp.append(this->mChatResponse);
        this->ResizeMessage(temp.length()+1,0);
        this->SetString(1,temp);
    }

    void NpcChatRequestMessage::GetMessageData()
    {
        this->mTargetNpcId = this->GetInt(0);
        this->mChatResponse = this->GetString(1);
    }

};
