#ifndef LOGINREQUESTMESSAGE_HPP_INCLUDED
#define LOGINREQUESTMESSAGE_HPP_INCLUDED

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
    class DllExport LoginRequestMessage : public MessageContainer
    {
        private:

        void PopulateCredentials();
        std::string mUsername;
        std::string mPassword;

        protected:

        public:

        LoginRequestMessage(Message& message)
            :MessageContainer(message){};

        LoginRequestMessage(const std::string& username,const std::string& password);
        LoginRequestMessage();
        ~LoginRequestMessage();

        const std::string& GetUsername();
        const std::string& GetPassword();

        void SetCredential(const std::string& username,const std::string& password);

        const Enigma::c8* Hash(const std::string& value);

        const Enigma::c8* NullSafeHash(const std::string& value);

        static const int GetMessageType(){return 0;}
        static const int GetMessageLength(){return 0;}

    };
};

#endif // LOGINREQUESTMESSAGE_HPP_INCLUDED
