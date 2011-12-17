/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "MovementRequestMessage.hpp"

namespace Enigma
{

    MovementRequestMessage::MovementRequestMessage(Enigma::f32 x1,Enigma::f32 y1, Enigma::f32 z1,Enigma::f32 x2,Enigma::f32 y2, Enigma::f32 z2)
    {
        this->ResizeMessage(MovementRequestMessage::GetMessageLength(),0);
        this->SetType(MovementRequestMessage::GetMessageType());

        this->SetX1(x1);
        this->SetY1(y1);
        this->SetZ1(z1);
        this->SetX2(x2);
        this->SetY2(y2);
        this->SetZ2(z2);

		//this->SetChannel(CHANNEL_AUTHENTICATION);
		this->SetChannel(CHANNEL_MOVEMENT);
		//this->SetChannel(CHANNEL_COMBAT);
		//this->SetChannel(CHANNEL_CHAT);
		//this->SetChannel(CHANNEL_VOICE_CHAT);	
	}

    MovementRequestMessage::MovementRequestMessage()
    {
        this->ResizeMessage(MovementRequestMessage::GetMessageLength(),0);
        this->SetType(MovementRequestMessage::GetMessageType());

        this->SetX1(0.0f);
        this->SetY1(0.0f);
        this->SetZ1(0.0f);
        this->SetX2(0.0f);
        this->SetY2(0.0f);
        this->SetZ2(0.0f);

		//this->SetChannel(CHANNEL_AUTHENTICATION);
		this->SetChannel(CHANNEL_MOVEMENT);
		//this->SetChannel(CHANNEL_COMBAT);
		//this->SetChannel(CHANNEL_CHAT);
		//this->SetChannel(CHANNEL_VOICE_CHAT);	
	}

    MovementRequestMessage::~MovementRequestMessage()
    {

    }

    void MovementRequestMessage::PrintMessage()
    {
        std::cout << "MovementRequestMessage: " <<
        "\n\tType: " << this->GetMessageUnion().signed32BitFloatPointer[0] <<
        "\n\tLength: " << this->GetMessageUnion().signed32BitFloatPointer[1] <<
        "\n\tX1: " << this->GetMessageUnion().signed32BitFloatPointer[2] <<
        "\n\tY1: " << this->GetMessageUnion().signed32BitFloatPointer[3] <<
        "\n\tZ1: " << this->GetMessageUnion().signed32BitFloatPointer[4] <<
        "\n\tX2: " << this->GetMessageUnion().signed32BitFloatPointer[5] <<
        "\n\tY2: " << this->GetMessageUnion().signed32BitFloatPointer[6] <<
        "\n\tZ2: " << this->GetMessageUnion().signed32BitFloatPointer[7] <<
        std::endl;
    }

    Enigma::f32 MovementRequestMessage::GetX1()
    {
        return this->GetFloat(0);
    }

    Enigma::f32 MovementRequestMessage::GetY1()
    {
        return this->GetFloat(1);
    }

    Enigma::f32 MovementRequestMessage::GetZ1()
    {
        return this->GetFloat(2);
    }

    Enigma::f32 MovementRequestMessage::GetX2()
    {
        return this->GetFloat(3);
    }

    Enigma::f32 MovementRequestMessage::GetY2()
    {
        return this->GetFloat(4);
    }

    Enigma::f32 MovementRequestMessage::GetZ2()
    {
        return this->GetFloat(5);
    }

    void MovementRequestMessage::SetX1(Enigma::f32 value)
    {
        this->SetFloat(0,value);
    }

    void MovementRequestMessage::SetY1(Enigma::f32 value)
    {
        this->SetFloat(1,value);
    }

    void MovementRequestMessage::SetZ1(Enigma::f32 value)
    {
        this->SetFloat(2,value);
    }

    void MovementRequestMessage::SetX2(Enigma::f32 value)
    {
        this->SetFloat(3,value);
    }

    void MovementRequestMessage::SetY2(Enigma::f32 value)
    {
        this->SetFloat(4,value);
    }

    void MovementRequestMessage::SetZ2(Enigma::f32 value)
    {
        this->SetFloat(5,value);
    }

    std::ostream& operator<< (std::ostream& os,const MovementRequestMessage& value)
    {
        MovementRequestMessage messageContainer=const_cast<MovementRequestMessage&>(value);
        return os <<
        messageContainer.GetMessageUnion().signed32BitFloatPointer[0] <<
        messageContainer.GetMessageUnion().signed32BitFloatPointer[1] <<
        messageContainer.GetMessageUnion().signed32BitFloatPointer[2] <<
        messageContainer.GetMessageUnion().signed32BitFloatPointer[3] <<
        messageContainer.GetMessageUnion().signed32BitFloatPointer[4] <<
        messageContainer.GetMessageUnion().signed32BitFloatPointer[5] <<
        messageContainer.GetMessageUnion().signed32BitFloatPointer[6] <<
        messageContainer.GetMessageUnion().signed32BitFloatPointer[7] <<
        std::endl;
    }

};
