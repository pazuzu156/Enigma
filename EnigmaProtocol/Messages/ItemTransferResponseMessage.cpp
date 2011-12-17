/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "ItemTransferResponseMessage.hpp"

namespace Enigma
{
    ItemTransferResponseMessage::ItemTransferResponseMessage()
    {
        size_t size;
        size = ItemTransferResponseMessage::GetMessageLength();
        this->ResizeMessage(size,0);
        this->SetType(ItemTransferResponseMessage::GetMessageType());

		//this->SetChannel(CHANNEL_AUTHENTICATION);
		//this->SetChannel(CHANNEL_MOVEMENT);
		this->SetChannel(CHANNEL_COMBAT);
		//this->SetChannel(CHANNEL_CHAT);
		//this->SetChannel(CHANNEL_VOICE_CHAT);	
	}

    ItemTransferResponseMessage::~ItemTransferResponseMessage()
    {

    }

    Enigma::s32 ItemTransferResponseMessage::GetSourceLocation()
    {
        return this->GetInt(0);
    }

    void ItemTransferResponseMessage::SetSourceLocation(Enigma::s32 value)
    {
        this->SetInt(0,value);
    }

    Enigma::s32 ItemTransferResponseMessage::GetSourceId()
    {
        return this->GetInt(1);
    }

    void ItemTransferResponseMessage::SetSourceId(Enigma::s32 value)
    {
        this->SetInt(1,value);
    }

    Enigma::s32 ItemTransferResponseMessage::GetDestinationLocation()
    {
        return this->GetInt(2);
    }

    void ItemTransferResponseMessage::SetDestinationLocation(Enigma::s32 value)
    {
        this->SetInt(2,value);
    }

    Enigma::s32 ItemTransferResponseMessage::GetDestinationId()
    {
        return this->GetInt(3);
    }

    void ItemTransferResponseMessage::SetDestinationId(Enigma::s32 value)
    {
        this->SetInt(3,value);
    }

    Enigma::s32 ItemTransferResponseMessage::GetStackSize()
    {
        return this->GetInt(4);
    }

    void ItemTransferResponseMessage::SetStackSize(Enigma::s32 value)
    {
        this->SetInt(4,value);
    }

    void ItemTransferResponseMessage::SetStackSize(size_t value)
    {
        this->SetInt(4,value);
    }

};
