#ifndef KICKREQUESTMESSAGE_HPP_INCLUDED
#define KICKREQUESTMESSAGE_HPP_INCLUDED

/*
Copyright � 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "MessageContainer.hpp"
#include "KickTypes.hpp"

namespace Enigma
{
    class DllExport KickRequestMessage : public MessageContainer
    {
        private:

        std::string mName;

        protected:

        public:

        KickRequestMessage(Message& message)
            :MessageContainer(message){};

        KickRequestMessage();
        ~KickRequestMessage();

        static const int GetMessageType(){return 20;}
        static const int GetMessageLength(){return 1;}

        Enigma::s32 GetKickType();
        void SetKickType(Enigma::s32 value);

        const std::string& GetName();
        void SetName(const std::string& value);
        void SetName(std::string& value);
    };
};

#endif // KICKREQUESTMESSAGE_HPP_INCLUDED
