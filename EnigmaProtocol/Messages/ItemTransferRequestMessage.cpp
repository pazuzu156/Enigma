/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "ItemTransferRequestMessage.hpp"

namespace Enigma
{
    ItemTransferRequestMessage::ItemTransferRequestMessage()
    {
        size_t size;
        size = ItemTransferRequestMessage::GetMessageLength();
        this->ResizeMessage(size,0);
        this->SetType(ItemTransferRequestMessage::GetMessageType());

		//this->SetChannel(CHANNEL_AUTHENTICATION);
		//this->SetChannel(CHANNEL_MOVEMENT);
		this->SetChannel(CHANNEL_COMBAT);
		//this->SetChannel(CHANNEL_CHAT);
		//this->SetChannel(CHANNEL_VOICE_CHAT);	
	}

    ItemTransferRequestMessage::~ItemTransferRequestMessage()
    {

    }

    Enigma::s32 ItemTransferRequestMessage::GetSourceLocation()
    {
        return this->GetInt(0);
    }

    void ItemTransferRequestMessage::SetSourceLocation(Enigma::s32 value)
    {
        this->SetInt(0,value);
    }

    Enigma::s32 ItemTransferRequestMessage::GetSourceId()
    {
        return this->GetInt(1);
    }

    void ItemTransferRequestMessage::SetSourceId(Enigma::s32 value)
    {
        this->SetInt(1,value);
    }

    Enigma::s32 ItemTransferRequestMessage::GetDestinationLocation()
    {
        return this->GetInt(2);
    }

    void ItemTransferRequestMessage::SetDestinationLocation(Enigma::s32 value)
    {
        this->SetInt(2,value);
    }

    Enigma::s32 ItemTransferRequestMessage::GetDestinationId()
    {
        return this->GetInt(3);
    }

    void ItemTransferRequestMessage::SetDestinationId(Enigma::s32 value)
    {
        this->SetInt(3,value);
    }

    Enigma::s32 ItemTransferRequestMessage::GetStackSize()
    {
        return this->GetInt(4);
    }

    void ItemTransferRequestMessage::SetStackSize(Enigma::s32 value)
    {
        this->SetInt(4,value);
    }

    void ItemTransferRequestMessage::SetStackSize(size_t value)
    {
        this->SetInt(4,value);
    }

};
