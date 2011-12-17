#ifndef INVITEREQUESTMESSAGE_HPP_INCLUDED
#define INVITEREQUESTMESSAGE_HPP_INCLUDED

/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "MessageContainer.hpp"
#include "InviteTypes.hpp"

namespace Enigma
{
    class DllExport InviteRequestMessage : public MessageContainer
    {
        private:

        std::string mTarget;

        protected:

        public:

        InviteRequestMessage(Message& message)
            :MessageContainer(message){};

        InviteRequestMessage();
        ~InviteRequestMessage();

        static const int GetMessageType(){return 18;}
        static const int GetMessageLength(){return 2;}

        Enigma::s32 GetInviteType();
        void SetInviteType(Enigma::s32 value);

        Enigma::s32 GetInviteId();
        void SetInviteId(Enigma::s32 value);

        const std::string& GetTarget();
        void SetTarget(const std::string& value);
        void SetTarget(std::string& value);
    };
};

#endif // INVITEREQUESTMESSAGE_HPP_INCLUDED
