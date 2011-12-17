/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "MessageContainer.hpp"

namespace Enigma
{

    MessageContainer::MessageContainer()
    {
        this->mMessage.signed32BitIntegerPointer=NULL;
        this->mReliable=true;
        this->mOwnsUnion=false;
        this->mOwnsMessageString=false;
        this->mIsDirty=false;

		this->mChannel=CHANNEL_NONE; //this should always be changed.
    }

    MessageContainer::MessageContainer(Message message)
    {
        this->mMessage=message;
        this->mReliable=true;
        this->mOwnsUnion=false;
        this->mOwnsMessageString=false;
        this->mIsDirty=false;
		
		this->mChannel=CHANNEL_NONE; //this should always be changed.
    }

    MessageContainer::MessageContainer(Enigma::c8* message)
    {
        this->mMessage.signed32BitIntegerPointer=NULL;
        this->mReliable=true;
        this->mOwnsUnion=false;
        this->mOwnsMessageString=false;
        this->mIsDirty=false;

		this->mChannel=CHANNEL_NONE; //this should always be changed.

        this->SetInnerMessage(message);
    }

    MessageContainer::MessageContainer(std::string& message)
    {
        this->mMessage.signed32BitIntegerPointer=NULL;
        this->mReliable=true;
        this->mOwnsUnion=false;
        this->mOwnsMessageString=false;
        this->mIsDirty=false;

		this->mChannel=CHANNEL_NONE; //this should always be changed.

        this->SetInnerMessage(message);
    }

    MessageContainer::~MessageContainer()
    {
        this->Clear("MessageContainer::~MessageContainer()");
    }

    void MessageContainer::PrintMessage()
    {
        std::cout << "Message: " << this->GetMessageUnion().unsigned8BitIntegerPointer << std::endl;
    }

    Message& MessageContainer::GetMessageUnion()
    {
        return this->mMessage;
    }

    void MessageContainer::SetMessageUnion(Message message)
    {
        this->Clear("MessageContainer::SetMessageUnion(Message&)");
        this->mMessage=message;
        this->mOwnsUnion=false;
        this->mOwnsMessageString=false;
    }

    Enigma::s32 MessageContainer::GetType()
    {
        if(this->mMessage.signed32BitIntegerPointer!=NULL)
        {
            return this->mMessage.signed32BitIntegerPointer[MESSAGE_TYPE];
        }
        else
        {
            return 0;
        }
    }

    void MessageContainer::SetType(Enigma::s32 value)
    {
        if(this->mMessage.signed32BitIntegerPointer!=NULL)
        {
            this->mMessage.signed32BitIntegerPointer[MESSAGE_TYPE]=value;
        }
        else
        {
            std::cout << "Message is NULL: MessageContainer::SetType" << std::endl;
        }
    }

    Enigma::s32 MessageContainer::GetLength()
    {
        if(this->mMessage.signed32BitIntegerPointer!=NULL)
        {
            return this->mMessage.signed32BitIntegerPointer[MESSAGE_LENGTH];
        }
        else
        {
            return 0;
        }
    }

    void MessageContainer::SetLength(Enigma::s32 value)
    {
        if(this->mMessage.signed32BitIntegerPointer!=NULL)
        {
            this->mMessage.signed32BitIntegerPointer[MESSAGE_LENGTH]=value;
        }
        else
        {
            std::cout << "Message is NULL: MessageContainer::SetLength" << std::endl;
        }
    }

    Enigma::s32 MessageContainer::GetStatus()
    {
        if(this->mMessage.signed32BitIntegerPointer!=NULL)
        {
            return this->mMessage.signed32BitIntegerPointer[MESSAGE_STATUS];
        }
        else
        {
            return STATUS_UNKNOWN_FAIL;
        }
    }

    void MessageContainer::SetStatus(Enigma::s32 value)
    {
        if(this->mMessage.signed32BitIntegerPointer!=NULL)
        {
            this->mMessage.signed32BitIntegerPointer[MESSAGE_STATUS]=value;
        }
        else
        {
            std::cout << "Message is NULL: MessageContainer::SetStatus" << std::endl;
        }
    }

    bool MessageContainer::GetReliable() const
    {
        return this->mReliable;
    }

    void MessageContainer::SetReliable(bool value)
    {
        this->mReliable=value;
    }

    size_t MessageContainer::GetChannel() const
	{
		if(this->mChannel==CHANNEL_NONE)
		{
			throw new EnigmaException("Message channel not valid!"); //this should make it easier to detect failure to assign channels on derived classes.
		}
		return this->mChannel;
	}

    void MessageContainer::SetChannel(size_t value)
	{
		if(value==CHANNEL_NONE)
		{
			throw new EnigmaException("Message channel not valid!");
		}
		this->mChannel=value;
	}

    //Return message minus the length & type.
    Enigma::c8* MessageContainer::GetInnerMessage()
    {
        if(this->mMessage.signed8BitIntegerPointer!=NULL)
        {
            return (Enigma::c8*)this->mMessage.signed8BitIntegerPointer+(sizeof(Enigma::s32)*MessageContainer::GetMessageLength());
        }
        else
        {
            return "";
        }
    }

    //Set a new inner message.
	void MessageContainer::SetInnerMessage(const std::string& message)
    {
        this->SetInnerMessage(message.size(),message.c_str());
    }

    //Set a new inner message.
    void MessageContainer::SetInnerMessage(const Enigma::c8* message)
    {
        this->SetInnerMessage(strlen(message),message);
    }

    //Set a new inner message.
    void MessageContainer::SetInnerMessage(size_t size,const Enigma::c8* message)
    {
        this->ResizeMessage(size/2,0);
        strcpy(this->GetInnerMessage(),message);
        this->mOwnsMessageString=true; //because this is a copy.
    }

    //Resize message.
    void MessageContainer::ResizeMessage(size_t size,int type)
    {
        try
        {
            int newSize=0;
            int newType=0;
            int charSize=0;
            int paddingSize=1;

            newSize = MessageContainer::GetMessageLength()+paddingSize+size;
            charSize=sizeof(Enigma::s32)/sizeof(Enigma::c8)*newSize;

            if(this->mMessage.signed32BitIntegerPointer!=NULL)
            {
                newType = this->GetType();
                delete[] this->mMessage.signed32BitIntegerPointer;
                this->mMessage.signed32BitIntegerPointer=NULL;
            }

			//Added () which as of C++03 initializes the allocated memory.
            this->mMessage.signed32BitIntegerPointer = new Enigma::s32[newSize]();

			//This may no longer be required but I'll leave it just in case \0 isn't really 0.
            this->mMessage.unsigned8BitIntegerPointer[charSize-1]='\0';

            this->mOwnsMessageString=true;

            this->SetType(newType);
            this->SetLength(charSize);
            this->SetStatus(STATUS_OK);

            this->mIsDirty=true;
        }
		catch(Enigma::EnigmaException& e)
		{
			std::cout << "Enigma Exception: " << e.what() << std::endl;
		}
		catch (Enigma::HardwareException&)
		{
			throw;
		}
		catch (std::overflow_error&)
		{
			throw; //may need to pop some calls off the stack.
		}
		catch (std::exception& e)
		{
			std::cout << "Exception: " << e.what() << std::endl;
		}
    }

    void MessageContainer::Clear(const Enigma::c8* callerName)
    {
        if(this->mIsDirty && this->mMessage.signed32BitIntegerPointer!=NULL)
        {
            this->mIsDirty=false;

            if(this->mOwnsMessageString)
            {
                delete[] this->mMessage.signed32BitIntegerPointer;
                this->mMessage.signed32BitIntegerPointer=NULL;
                this->mOwnsMessageString=false;
            }
            else
            {
                std::cout << "Message String not owned by container string not deallocated." << std::endl;
            }
        }
        else
        {
            //spams the console. even though calls are from destructor.
            //std::cout << "Extra call to MessageContainer::Clear() from " << callerName << "." << std::endl;
        }
    }

    void MessageContainer::SkipAutomaticCleanup()
    {
        this->mIsDirty=false;
    }

    u64 MessageContainer::GetSimpleChecksum(Enigma::u8* data,size_t length)
    {
        u64 total=0;
        for(size_t i=0;i<length;i++)
        {
            total+=(u64)data[i];
        }
        return total;
    }

    Enigma::u32 MessageContainer::GetFletcherChecksum(Enigma::u16* data,size_t length)
    {
        Enigma::u32 a=0xffff;
        Enigma::u32 b=0xffff;
        Enigma::u32 tlength=0;

        while(length)
        {
            tlength = length > 360 ? 360 : length;
            length -= tlength;

            do
            {
                a += *data++;
                b += a;
            }
            while(--tlength);

            a = (a & 0xffff) + (a >> 16);
            b = (b & 0xffff) + (b >> 16);
        }

        a = (a & 0xffff) + (a >> 16);
        b = (b & 0xffff) + (b >> 16);

        return b << 16 | a;
    }

    Enigma::u32 MessageContainer::GetAdlerChecksum(Enigma::u8* data,size_t length)
    {
        Enigma::u32 a=0;
        Enigma::u32 b=0;
        const size_t MOD_ADLER=65521;

        for(size_t i=0;i<length;i++)
        {
            a = (a+data[i]) % MOD_ADLER;
            b = (b+a) % MOD_ADLER;
        }

        return (b << 16) | a;
    }

    Enigma::f32 MessageContainer::GetFloat(size_t index)
    {
        index+=MessageContainer::GetMessageLength();
        return this->mMessage.signed32BitFloatPointer[index];
    }

    void MessageContainer::SetFloat(size_t index,Enigma::f32 value)
    {
        index+=MessageContainer::GetMessageLength();
        this->mMessage.signed32BitFloatPointer[index]=value;
    }

    Enigma::s32 MessageContainer::GetInt(size_t index)
    {
        index+=MessageContainer::GetMessageLength();
        return this->mMessage.signed32BitIntegerPointer[index];
    }

    void MessageContainer::SetInt(size_t index,Enigma::s32 value)
    {
        index+=MessageContainer::GetMessageLength();
        this->mMessage.signed32BitIntegerPointer[index]=value;
    }

    Enigma::c8* MessageContainer::GetString(size_t index)
    {
        index+=MessageContainer::GetMessageLength();
        return (Enigma::c8*)this->mMessage.signed8BitIntegerPointer+index*sizeof(Enigma::s32);
    }

	void MessageContainer::SetString(size_t index,std::string& value)
    {
        this->SetString(index,value.c_str());
    }

	void MessageContainer::SetString(size_t index,const std::string& value)
    {
        this->SetString(index,value.c_str());
    }

    void MessageContainer::SetString(size_t index,const Enigma::c8* value)
    {
        Enigma::c8* target=NULL;

        target = this->GetString(index); //grab pointer to correct spot in message.

        strcpy(target,value);
    }

    Enigma::u8* MessageContainer::GetBytes(size_t index,size_t& size)
    {
		size_t byteOffset = (index+MessageContainer::GetMessageLength())*sizeof(Enigma::s32);
		Enigma::u8* result = this->mMessage.unsigned8BitIntegerPointer+byteOffset;

		//total length minus fixed length equals variable length.
		size = ( this->GetLength() - (index+MessageContainer::GetMessageLength()+1)*sizeof(Enigma::s32) );

        return result;
    }

    void MessageContainer::SetBytes(size_t index,size_t length,const Enigma::u8* value)
    {	
		size_t byteOffset = (index+MessageContainer::GetMessageLength())*sizeof(Enigma::s32); 
		size_t newSize = index+(length/sizeof(Enigma::s32));

		//convert byte length to int length and add index then resize message to that size.
		this->ResizeMessage(newSize,this->GetType());

		Enigma::u8* target = this->mMessage.unsigned8BitIntegerPointer+byteOffset;

		//copy byte data to message.
		memcpy(target,value,length);

		//diagnostic
		/*std::cout << "Samples: ";
		Enigma::u16* samples = (Enigma::u16*)target;
		for(size_t i=0;i<(length/sizeof(Enigma::u16));i++)
		{
			std::cout << samples[i];
		}
		std::cout << std::endl;*/
    }

    u64 MessageContainer::GetChecksum()
    {
        return MessageContainer::GetSimpleChecksum(this->mMessage.unsigned8BitIntegerPointer,this->GetLength());
    }

    std::ostream& operator<< (std::ostream& os,const MessageContainer& value)
    {
        MessageContainer messageContainer=const_cast<MessageContainer&>(value);
        return os << messageContainer.GetMessageUnion().unsigned8BitIntegerPointer << std::endl;
    }
};
