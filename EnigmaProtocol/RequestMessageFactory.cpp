/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "RequestMessageFactory.hpp"

#include "Messages/RequestMessages.hpp"

namespace Enigma
{

    RequestMessageFactory::RequestMessageFactory()
    {

    }

    RequestMessageFactory::~RequestMessageFactory()
    {

    }

    MessageContainer* RequestMessageFactory::CreateMessage(Message& message)
    {
        MessageContainer* messageContainer=NULL;

        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==MovementRequestMessage::GetMessageType())
        {
            messageContainer = new MovementRequestMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==PlayerOnMapUpdateRequestMessage::GetMessageType())
        {
            messageContainer = new PlayerOnMapUpdateRequestMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==ChangeMapRequestMessage::GetMessageType())
        {
            messageContainer = new ChangeMapRequestMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==NpcOnMapUpdateRequestMessage::GetMessageType())
        {
            messageContainer = new NpcOnMapUpdateRequestMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==MonsterOnMapUpdateRequestMessage::GetMessageType())
        {
            messageContainer = new MonsterOnMapUpdateRequestMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==ItemOnMapUpdateRequestMessage::GetMessageType())
        {
            messageContainer = new ItemOnMapUpdateRequestMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==LoginRequestMessage::GetMessageType())
        {
            messageContainer = new LoginRequestMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==CharacterCreationRequestMessage::GetMessageType())
        {
            messageContainer = new CharacterCreationRequestMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==CharacterListRequestMessage::GetMessageType())
        {
            messageContainer = new CharacterListRequestMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==CharacterSelectionRequestMessage::GetMessageType())
        {
            messageContainer = new CharacterSelectionRequestMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==ChatRequestMessage::GetMessageType())
        {
            messageContainer = new ChatRequestMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==VoiceChatRequestMessage::GetMessageType())
        {
            messageContainer = new VoiceChatRequestMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==InviteRequestMessage::GetMessageType())
        {
            messageContainer = new InviteRequestMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==KickRequestMessage::GetMessageType())
        {
            messageContainer = new KickRequestMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==PlayerListRequestMessage::GetMessageType())
        {
            messageContainer = new PlayerListRequestMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==CreatePlayerOrganizationRequestMessage::GetMessageType())
        {
            messageContainer = new CreatePlayerOrganizationRequestMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==ModifyPlayerOrganizationRankRequestMessage::GetMessageType())
        {
            messageContainer = new ModifyPlayerOrganizationRankRequestMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==SetPlayerRankRequestMessage::GetMessageType())
        {
            messageContainer = new SetPlayerRankRequestMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==RollRequestMessage::GetMessageType())
        {
            messageContainer = new RollRequestMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==ServerTimeRequestMessage::GetMessageType())
        {
            messageContainer = new ServerTimeRequestMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==NpcChatRequestMessage::GetMessageType())
        {
            messageContainer = new NpcChatRequestMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==ItemTransferRequestMessage::GetMessageType())
        {
            messageContainer = new ItemTransferRequestMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==SkillRequestMessage::GetMessageType())
        {
            messageContainer = new SkillRequestMessage(message);
        }

        return messageContainer;
    }

};
