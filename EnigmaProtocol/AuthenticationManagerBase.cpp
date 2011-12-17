/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "AuthenticationManagerBase.hpp"

namespace Enigma
{

    AuthenticationManagerBase::AuthenticationManagerBase()
    {

    }

    AuthenticationManagerBase::~AuthenticationManagerBase()
    {

    }

    MessageContainer* AuthenticationManagerBase::CreateAuthenticationMessage(Message& message)
    {
        MessageContainer* messageContainer=NULL;

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

        return messageContainer;
    }

};
