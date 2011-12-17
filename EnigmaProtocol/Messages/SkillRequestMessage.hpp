#ifndef SKILLREQUESTMESSAGE_HPP_INCLUDED
#define SKILLREQUESTMESSAGE_HPP_INCLUDED

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

#include "TargetPreferenceTypes.hpp"

namespace Enigma
{
    class DllExport SkillRequestMessage : public MessageContainer
    {
        private:

        protected:

        public:

        SkillRequestMessage(Message& message)
            :MessageContainer(message){};

        SkillRequestMessage();
        ~SkillRequestMessage();

        static const int GetMessageType(){return 42;}
        static const int GetMessageLength(){return 5;}

        Enigma::s32 GetSkillId();
        void SetSkillId(Enigma::s32 value);
        void SetSkillId(std::string value);

        Enigma::s32 GetSkillLevel();
        void SetSkillLevel(Enigma::s32 value);
        void SetSkillLevel(std::string value);

		Enigma::s32 GetTargetNpcId();
        void SetTargetNpcId(Enigma::s32 value);
        void SetTargetNpcId(std::string value);

		Enigma::s32 GetTargetMonsterId();
        void SetTargetMonsterId(Enigma::s32 value);
        void SetTargetMonsterId(std::string value);

		Enigma::s32 GetTargetCharacterId();
        void SetTargetCharacterId(Enigma::s32 value);
        void SetTargetCharacterId(std::string value);

		Enigma::s32 GetTargetPreferenceType();
        void SetTargetPreferenceType(Enigma::s32 value);
        void SetTargetPreferenceType(std::string value);

		Enigma::vector3d<Enigma::s32> GetTargetLocation();
		void SetTargetLocation(Enigma::vector3d<Enigma::s32> value);
    };
};


#endif // SKILLREQUESTMESSAGE_HPP_INCLUDED