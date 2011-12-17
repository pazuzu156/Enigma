/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "MovementResponseMessage.hpp"

namespace Enigma
{

    MovementResponseMessage::MovementResponseMessage(Enigma::f32 x1,Enigma::f32 y1, Enigma::f32 z1,Enigma::f32 x2,Enigma::f32 y2, Enigma::f32 z2,Enigma::s32 combatantId)
    {
        this->ResizeMessage(MovementResponseMessage::GetMessageLength(),0);
        this->SetType(MovementResponseMessage::GetMessageType());

        this->SetX1(x1);
        this->SetY1(y1);
        this->SetZ1(z1);
        this->SetX2(x2);
        this->SetY2(y2);
        this->SetZ2(z2);

        this->SetCombatantId(combatantId);

		//this->SetChannel(CHANNEL_AUTHENTICATION);
		this->SetChannel(CHANNEL_MOVEMENT);
		//this->SetChannel(CHANNEL_COMBAT);
		//this->SetChannel(CHANNEL_CHAT);
		//this->SetChannel(CHANNEL_VOICE_CHAT);	
	}

    MovementResponseMessage::MovementResponseMessage()
    {
        this->ResizeMessage(MovementResponseMessage::GetMessageLength(),0);
        this->SetType(MovementResponseMessage::GetMessageType());

        this->SetX1(0.0f);
        this->SetY1(0.0f);
        this->SetZ1(0.0f);
        this->SetX2(0.0f);
        this->SetY2(0.0f);
        this->SetZ2(0.0f);

        this->SetCombatantId(0);

		//this->SetChannel(CHANNEL_AUTHENTICATION);
		this->SetChannel(CHANNEL_MOVEMENT);
		//this->SetChannel(CHANNEL_COMBAT);
		//this->SetChannel(CHANNEL_CHAT);
		//this->SetChannel(CHANNEL_VOICE_CHAT);	
    }

    MovementResponseMessage::~MovementResponseMessage()
    {

    }

    void MovementResponseMessage::PrintMessage()
    {
        std::cout << "MovementResponseMessage: " <<
        "\n\tType: " << this->GetMessageUnion().signed32BitFloatPointer[0] <<
        "\n\tLength: " << this->GetMessageUnion().signed32BitFloatPointer[1] <<
        "\n\tX1: " << this->GetMessageUnion().signed32BitFloatPointer[2] <<
        "\n\tY1: " << this->GetMessageUnion().signed32BitFloatPointer[3] <<
        "\n\tZ1: " << this->GetMessageUnion().signed32BitFloatPointer[4] <<
        "\n\tX2: " << this->GetMessageUnion().signed32BitFloatPointer[5] <<
        "\n\tY2: " << this->GetMessageUnion().signed32BitFloatPointer[6] <<
        "\n\tZ2: " << this->GetMessageUnion().signed32BitFloatPointer[7] <<
        "\n\tCombatantId: " << this->GetMessageUnion().signed32BitIntegerPointer[8] <<
        std::endl;
    }

    Enigma::f32 MovementResponseMessage::GetX1()
    {
        //return this->GetMessage()->fourth[2];
        return this->GetFloat(0);
    }

    Enigma::f32 MovementResponseMessage::GetY1()
    {
        //return this->GetMessage()->fourth[3];
        return this->GetFloat(1);
    }

    Enigma::f32 MovementResponseMessage::GetZ1()
    {
        //return this->GetMessage()->fourth[4];
        return this->GetFloat(2);
    }

    Enigma::f32 MovementResponseMessage::GetX2()
    {
        //return this->GetMessage()->fourth[5];
        return this->GetFloat(3);
    }

    Enigma::f32 MovementResponseMessage::GetY2()
    {
        //return this->GetMessage()->fourth[6];
        return this->GetFloat(4);
    }

    Enigma::f32 MovementResponseMessage::GetZ2()
    {
        //return this->GetMessage()->fourth[7];
        return this->GetFloat(5);
    }

    void MovementResponseMessage::SetX1(Enigma::f32 value)
    {
        //this->GetMessage()->fourth[2]=value;
        this->SetFloat(0,value);
    }

    void MovementResponseMessage::SetY1(Enigma::f32 value)
    {
        //this->GetMessage()->fourth[3]=value;
        this->SetFloat(1,value);
    }

    void MovementResponseMessage::SetZ1(Enigma::f32 value)
    {
        //this->GetMessage()->fourth[4]=value;
        this->SetFloat(2,value);
    }

    void MovementResponseMessage::SetX2(Enigma::f32 value)
    {
        //this->GetMessage()->fourth[5]=value;
        this->SetFloat(3,value);
    }

    void MovementResponseMessage::SetY2(Enigma::f32 value)
    {
        //this->GetMessage()->fourth[6]=value;
        this->SetFloat(4,value);
    }

    void MovementResponseMessage::SetZ2(Enigma::f32 value)
    {
        //this->GetMessage()->fourth[7]=value;
        this->SetFloat(5,value);
    }

    Enigma::s32 MovementResponseMessage::GetCombatantId()
    {
        //return this->GetMessage()->second[8];
        return this->GetInt(6);
    }

    void MovementResponseMessage::SetCombatantId(Enigma::s32 value)
    {
        //this->GetMessage()->second[8]=value;
        this->SetInt(6,value);
    }

    std::ostream& operator<< (std::ostream& os,const MovementResponseMessage& value)
    {
        MovementResponseMessage messageContainer=const_cast<MovementResponseMessage&>(value);
        return os <<
        messageContainer.GetMessageUnion().signed32BitFloatPointer[0] <<
        messageContainer.GetMessageUnion().signed32BitFloatPointer[1] <<
        messageContainer.GetMessageUnion().signed32BitFloatPointer[2] <<
        messageContainer.GetMessageUnion().signed32BitFloatPointer[3] <<
        messageContainer.GetMessageUnion().signed32BitFloatPointer[4] <<
        messageContainer.GetMessageUnion().signed32BitFloatPointer[5] <<
        messageContainer.GetMessageUnion().signed32BitFloatPointer[6] <<
        messageContainer.GetMessageUnion().signed32BitFloatPointer[7] <<
        messageContainer.GetMessageUnion().signed32BitIntegerPointer[8] <<
        std::endl;
    }
};
