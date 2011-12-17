/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "MovementManagerBase.hpp"

namespace Enigma
{

    MovementManagerBase::MovementManagerBase()
    {

    }

    MovementManagerBase::~MovementManagerBase()
    {

    }

    MessageContainer* MovementManagerBase::CreateMovementMessage(Message& message)
    {
        MessageContainer* messageContainer=NULL;

        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==MovementRequestMessage::GetMessageType())
        {
            messageContainer = new MovementRequestMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==MovementResponseMessage::GetMessageType())
        {
            messageContainer = new MovementResponseMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==ChangeMapRequestMessage::GetMessageType())
        {
            messageContainer = new ChangeMapRequestMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==ChangeMapResponseMessage::GetMessageType())
        {
            messageContainer = new ChangeMapResponseMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==PlayerOnMapUpdateRequestMessage::GetMessageType())
        {
            messageContainer = new PlayerOnMapUpdateRequestMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==PlayerOnMapUpdateResponseMessage::GetMessageType())
        {
            messageContainer = new PlayerOnMapUpdateResponseMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==NpcOnMapUpdateRequestMessage::GetMessageType())
        {
            messageContainer = new NpcOnMapUpdateRequestMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==NpcOnMapUpdateResponseMessage::GetMessageType())
        {
            messageContainer = new NpcOnMapUpdateResponseMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==MonsterOnMapUpdateRequestMessage::GetMessageType())
        {
            messageContainer = new MonsterOnMapUpdateRequestMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==MonsterOnMapUpdateResponseMessage::GetMessageType())
        {
            messageContainer = new MonsterOnMapUpdateResponseMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==ItemOnMapUpdateRequestMessage::GetMessageType())
        {
            messageContainer = new ItemOnMapUpdateRequestMessage(message);
        }
        else
        if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==ItemOnMapUpdateResponseMessage::GetMessageType())
        {
            messageContainer = new ItemOnMapUpdateResponseMessage(message);
        }

        return messageContainer;
    }

};
