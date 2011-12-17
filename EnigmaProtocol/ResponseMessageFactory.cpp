/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "ResponseMessageFactory.hpp"

#include "Messages/ResponseMessages.hpp"

namespace Enigma
{

    ResponseMessageFactory::ResponseMessageFactory()
    {

    }

    ResponseMessageFactory::~ResponseMessageFactory()
    {

    }

    MessageContainer* ResponseMessageFactory::CreateMessage(Message& message)
    {
        MessageContainer* messageContainer=NULL;

        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==MovementResponseMessage::GetMessageType())
        {
            messageContainer = new MovementResponseMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==PlayerOnMapUpdateResponseMessage::GetMessageType())
        {
            messageContainer = new PlayerOnMapUpdateResponseMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==ChangeMapResponseMessage::GetMessageType())
        {
            messageContainer = new ChangeMapResponseMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==NpcOnMapUpdateResponseMessage::GetMessageType())
        {
            messageContainer = new NpcOnMapUpdateResponseMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==MonsterOnMapUpdateResponseMessage::GetMessageType())
        {
            messageContainer = new MonsterOnMapUpdateResponseMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==ItemOnMapUpdateResponseMessage::GetMessageType())
        {
            messageContainer = new ItemOnMapUpdateResponseMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==LoginResponseMessage::GetMessageType())
        {
            messageContainer = new LoginResponseMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==CharacterCreationResponseMessage::GetMessageType())
        {
            messageContainer = new CharacterCreationResponseMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==CharacterListResponseMessage::GetMessageType())
        {
            messageContainer = new CharacterListResponseMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==CharacterSelectionResponseMessage::GetMessageType())
        {
            messageContainer = new CharacterSelectionResponseMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==ChatResponseMessage::GetMessageType())
        {
            messageContainer = new ChatResponseMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==VoiceChatResponseMessage::GetMessageType())
        {
            messageContainer = new VoiceChatResponseMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==InviteResponseMessage::GetMessageType())
        {
            messageContainer = new InviteResponseMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==KickResponseMessage::GetMessageType())
        {
            messageContainer = new KickResponseMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==PlayerListResponseMessage::GetMessageType())
        {
            messageContainer = new PlayerListResponseMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==CreatePlayerOrganizationResponseMessage::GetMessageType())
        {
            messageContainer = new CreatePlayerOrganizationResponseMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==ModifyPlayerOrganizationRankResponseMessage::GetMessageType())
        {
            messageContainer = new ModifyPlayerOrganizationRankResponseMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==SetPlayerRankResponseMessage::GetMessageType())
        {
            messageContainer = new SetPlayerRankResponseMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==NpcChatResponseMessage::GetMessageType())
        {
            messageContainer = new NpcChatResponseMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==ItemTransferResponseMessage::GetMessageType())
        {
            messageContainer = new ItemTransferResponseMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==SkillResponseMessage::GetMessageType())
        {
            messageContainer = new SkillResponseMessage(message);
        }

        return messageContainer;
    }

};
