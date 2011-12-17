/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "ChatRequestMessage.hpp"

namespace Enigma
{

    ChatRequestMessage::ChatRequestMessage()
    {
        size_t size;
        size = ChatRequestMessage::GetMessageLength();
        this->ResizeMessage(size,0);
        this->SetType(ChatRequestMessage::GetMessageType());
        this->mChatMessage="";
        this->mChatTarget="";
        this->SetChatType(CHAT_TYPE_MAP);

		//this->SetChannel(CHANNEL_AUTHENTICATION);
		//this->SetChannel(CHANNEL_MOVEMENT);
		//this->SetChannel(CHANNEL_COMBAT);
		this->SetChannel(CHANNEL_CHAT);
		//this->SetChannel(CHANNEL_VOICE_CHAT);	
	}

    ChatRequestMessage::~ChatRequestMessage()
    {

    }

    Enigma::s32 ChatRequestMessage::GetChatType()
    {
        return this->GetInt(0);
    }

    void ChatRequestMessage::SetChatType(Enigma::s32 value)
    {
        this->SetInt(0,value);
        this->mChatType=value;
    }

    const std::string& ChatRequestMessage::GetChatMessage() const
    {
        return this->mChatMessage;
    }

    void ChatRequestMessage::SetChatMessage(const std::string& value)
    {
        this->mChatMessage=value;
    }

    const std::string& ChatRequestMessage::GetChatTarget() const
    {
        return this->mChatTarget;
    }

    void ChatRequestMessage::SetChatTarget(const std::string& value)
    {
        this->mChatTarget=value;
    }

    void ChatRequestMessage::UpdateMessageData()
    {
		std::string temp;
        temp.append(this->mChatMessage);
        temp.append("~");
        temp.append(this->mChatTarget);
        this->ResizeMessage(temp.length()+1,0);
        this->SetString(1,temp);
        this->SetChatType(this->mChatType);

		//std::cout << "UpdateChatMessageData(): " << temp << std::endl;
    }

    void ChatRequestMessage::GetMessageData()
    {
		std::string temp=this->GetString(1);
        size_t split=temp.find("~");
        this->mChatMessage = temp.substr(0,split);
        this->mChatTarget = temp.substr(split+1,(temp.size()-this->mChatTarget.size()));

        //std::cout << "GetChatMessageData(): " << temp << std::endl;
    }

};
