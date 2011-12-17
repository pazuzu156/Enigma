#ifndef NPCONMAPUPDATERESPONSEMESSAGE_HPP_INCLUDED
#define NPCONMAPUPDATERESPONSEMESSAGE_HPP_INCLUDED

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
    class DllExport NpcOnMapUpdateResponseMessage : public MessageContainer
    {
        private:

        std::string mName;

        protected:

        public:

        NpcOnMapUpdateResponseMessage(Message& message)
            :MessageContainer(message){};

        NpcOnMapUpdateResponseMessage();
        ~NpcOnMapUpdateResponseMessage();

        static const int GetMessageType(){return 33;}
        static const int GetMessageLength(){return 8;}

        Enigma::s32 GetId();
        void SetId(Enigma::s32 value);

        bool GetIsLeavingMap();
        void SetIsLeavingMap(bool value);

        bool GetIsEnteringMap();
        void SetIsEnteringMap(bool value);

        Enigma::s32 GetGender();
        void SetGender(Enigma::s32 value);

        Enigma::f32 GetHealth();
        void SetHealth(Enigma::f32 value);

        Enigma::s32 GetModelId();
        void SetModelId(Enigma::s32 value);

        Enigma::f32 GetX();
        void SetX(Enigma::f32 value);

        Enigma::f32 GetY();
        void SetY(Enigma::f32 value);

        Enigma::f32 GetZ();
        void SetZ(Enigma::f32 value);

        const std::string& GetName();
        void SetName(const std::string& value);

    };
};

#endif // NPCONMAPUPDATERESPONSEMESSAGE_HPP_INCLUDED
