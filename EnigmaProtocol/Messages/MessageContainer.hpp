#ifndef MESSAGECONTAINER_HPP_INCLUDED
#define MESSAGECONTAINER_HPP_INCLUDED

/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "iostream.hpp"

#include "boost_foreach.hpp"

#include "EnigmaException.hpp"
#include "../Message.hpp"
#include "MessageStatus.hpp"
#include "Channels.hpp"

namespace Enigma
{
    class DllExport MessageContainer
    {
        private:

        Message mMessage;
        bool mReliable;
        bool mOwnsUnion;
        bool mOwnsMessageString;
		
		size_t mChannel;

        bool mIsDirty;
		
        protected:

        //Set a new inner message.
        void SetInnerMessage(const std::string& message);

        //Set a new inner message.
        void SetInnerMessage(const Enigma::c8* message);

        //Set a new inner message.
        void SetInnerMessage(size_t size,const Enigma::c8* message);

        //Resize message.
        void ResizeMessage(size_t size,int type);

        static u64 GetSimpleChecksum(Enigma::u8* data,size_t length);
        static Enigma::u32 GetFletcherChecksum(Enigma::u16* data,size_t length);
        static Enigma::u32 GetAdlerChecksum(Enigma::u8* data,size_t length);

        public:

        MessageContainer();
        MessageContainer(Message message);
        MessageContainer(Enigma::c8* message);
        MessageContainer(std::string& message);

        ~MessageContainer();

        static const int GetMessageType(){return -1;}
        static const int GetMessageLength(){return 3;}

        void PrintMessage();

		virtual void UpdateMessageData(){};
		virtual void GetMessageData(){};

        Message& GetMessageUnion();
        void SetMessageUnion(Message message);

        Enigma::s32 GetType();
        void SetType(Enigma::s32 value);

        Enigma::s32 GetLength();
        void SetLength(Enigma::s32 value);

        Enigma::s32 GetStatus();
        void SetStatus(Enigma::s32 value);

        bool GetReliable() const;
        void SetReliable(bool value);

        size_t GetChannel() const;
        void SetChannel(size_t value);

        //Return message minus the length & type.
        Enigma::c8* GetInnerMessage();

        //deallocates inner message.
        void Clear(const Enigma::c8* callerName);

        void SkipAutomaticCleanup();

        Enigma::f32 GetFloat(size_t index);
        void SetFloat(size_t index,Enigma::f32 value);

        Enigma::s32 GetInt(size_t index);
        void SetInt(size_t index,Enigma::s32 value);

        Enigma::c8* GetString(size_t index);
        void SetString(size_t index,std::string& value);
        void SetString(size_t index,const std::string& value);
        void SetString(size_t index,const Enigma::c8* value);

        Enigma::u8* GetBytes(size_t index,size_t& size);
        void SetBytes(size_t index,size_t length,const Enigma::u8* value);

        u64 GetChecksum();
    };
};

#endif // MESSAGECONTAINER_HPP_INCLUDED
