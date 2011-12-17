#ifndef MOVEMENTRESPONSEMESSAGE_HPP_INCLUDED
#define MOVEMENTRESPONSEMESSAGE_HPP_INCLUDED

/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "MessageContainer.hpp"
#include "string.hpp"

namespace Enigma
{
    class DllExport MovementResponseMessage : public MessageContainer
    {
        private:

        protected:

        public:

        MovementResponseMessage(Message& message)
            :MessageContainer(message){};

        MovementResponseMessage(Enigma::f32 x1,Enigma::f32 y1, Enigma::f32 z1,Enigma::f32 x2,Enigma::f32 y2, Enigma::f32 z2,Enigma::s32 combatantId);
        MovementResponseMessage();
        ~MovementResponseMessage();

        static const int GetMessageType(){return 9;}
        static const int GetMessageLength(){return 9;}

        void PrintMessage();

        Enigma::f32 GetX1();
        Enigma::f32 GetY1();
        Enigma::f32 GetZ1();
        Enigma::f32 GetX2();
        Enigma::f32 GetY2();
        Enigma::f32 GetZ2();

        void SetX1(Enigma::f32 value);
        void SetY1(Enigma::f32 value);
        void SetZ1(Enigma::f32 value);
        void SetX2(Enigma::f32 value);
        void SetY2(Enigma::f32 value);
        void SetZ2(Enigma::f32 value);

        Enigma::s32 GetCombatantId();
        void SetCombatantId(Enigma::s32 value);

    };
};

#endif // MOVEMENTRESPONSEMESSAGE_HPP_INCLUDED
