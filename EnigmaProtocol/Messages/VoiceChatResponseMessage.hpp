#ifndef VOICECHATRESPONSEMESSAGE_HPP_INCLUDED
#define VOICECHATRESPONSEMESSAGE_HPP_INCLUDED

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

#include "TypeDefs.hpp"

namespace Enigma
{
    class DllExport VoiceChatResponseMessage : public MessageContainer
    {
        private:

        protected:

        public:

        VoiceChatResponseMessage(Message& message)
            :MessageContainer(message){};

        VoiceChatResponseMessage();
        ~VoiceChatResponseMessage();

        static const int GetMessageType(){return 15;}
        static const int GetMessageLength(){return 2;}

        Enigma::s32 GetCombatantId();
        void SetCombatantId(Enigma::s32 value);

        Enigma::s32 GetVoiceChannel();
        void SetVoiceChannel(Enigma::s32 value);

        Enigma::u8* GetAudioData(size_t& size);
        void SetAudioData(Enigma::u8* value,size_t size);

		bool GetIsEndOfStream();

        s64 GetAudioCheckSum();
    };
};

#endif // VOICECHATRESPONSEMESSAGE_HPP_INCLUDED
