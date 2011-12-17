#ifndef CHATRESPONSEMESSAGE_HPP_INCLUDED
#define CHATRESPONSEMESSAGE_HPP_INCLUDED

/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "string.hpp"

#include "MessageContainer.hpp"
#include "ChatType.hpp"

namespace Enigma
{
    class DllExport ChatResponseMessage : public MessageContainer
    {
        private:

        std::string mChatMessage;
        std::string mChatTarget;
        Enigma::s32 mChatType;

        protected:

        public:

        ChatResponseMessage(Message& message)
            :MessageContainer(message){};

        ChatResponseMessage();
        ~ChatResponseMessage();

        static const int GetMessageType(){return 13;}
        static const int GetMessageLength(){return 0;}

        Enigma::s32 GetChatType();
        void SetChatType(Enigma::s32 value);

        virtual void UpdateMessageData();
        virtual void GetMessageData();

        const std::string& GetChatMessage() const;
        void SetChatMessage(const std::string& value);

        const std::string& GetChatTarget() const;
        void SetChatTarget(const std::string& value);
    };
};

#endif // CHATRESPONSEMESSAGE_HPP_INCLUDED
