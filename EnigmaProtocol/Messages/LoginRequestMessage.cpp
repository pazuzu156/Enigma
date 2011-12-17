/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "LoginRequestMessage.hpp"
#include <openssl/sha.h>

namespace Enigma
{

    LoginRequestMessage::LoginRequestMessage(const std::string& username,const std::string& password)
    {
        size_t size;
        size = LoginRequestMessage::GetMessageLength() + username.length() + password.length();
        this->ResizeMessage(size,0);
        this->SetType(LoginRequestMessage::GetMessageType());
        this->SetCredential(username,password);

		this->SetChannel(CHANNEL_AUTHENTICATION);
		//this->SetChannel(CHANNEL_MOVEMENT);
		//this->SetChannel(CHANNEL_COMBAT);
		//this->SetChannel(CHANNEL_CHAT);
		//this->SetChannel(CHANNEL_VOICE_CHAT);	
	}

    LoginRequestMessage::LoginRequestMessage()
    {
        this->ResizeMessage(LoginRequestMessage::GetMessageLength(),0);
        this->SetType(LoginRequestMessage::GetMessageType());

		this->SetChannel(CHANNEL_AUTHENTICATION);
		//this->SetChannel(CHANNEL_MOVEMENT);
		//this->SetChannel(CHANNEL_COMBAT);
		//this->SetChannel(CHANNEL_CHAT);
		//this->SetChannel(CHANNEL_VOICE_CHAT);	
	}

    LoginRequestMessage::~LoginRequestMessage()
    {

    }

    const std::string& LoginRequestMessage::GetUsername()
    {
        if(this->mUsername.length()==0)
        {
            PopulateCredentials();
        }
        return this->mUsername;
    }

    const std::string& LoginRequestMessage::GetPassword()
    {
        if(this->mPassword.length()==0)
        {
            PopulateCredentials();
        }
        return this->mPassword;
    }

    void LoginRequestMessage::SetCredential(const std::string& username,const std::string& password)
    {
        this->mUsername.clear();
        this->mPassword.clear();

        std::string temp;
        temp.append(username);
        temp.append("|");
        temp.append(this->NullSafeHash(password));

        this->SetInnerMessage(temp.c_str());
    }

    const Enigma::c8* LoginRequestMessage::Hash(const std::string& value)
    {
        const Enigma::u8* input = (const Enigma::u8*)value.c_str();
        unsigned long inputLength = value.length();
        Enigma::u8* buffer=NULL;

        buffer = SHA1(input,inputLength,NULL);  //NULL makes openssl allocate.

        return (const Enigma::c8*)buffer;
    }

    const Enigma::c8* LoginRequestMessage::NullSafeHash(const std::string& value)
    {
        const Enigma::u8* input = (const Enigma::u8*)value.c_str();
        size_t inputLength = value.length();

        const int allowedCharactersLimit = 61; //62 count.
        const Enigma::u8 allowedCharacters[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";


        char* output = new char[inputLength+1];
        int cache; //oh CB your favorite variable name lol.

        for(size_t i=0;i<inputLength;i++)
        {
            cache = (input[i]+inputLength)*3/7; //another job algorithm.
            if(cache>allowedCharactersLimit)
            {
                cache = allowedCharactersLimit;
            }
            output[i] = allowedCharacters[cache];
        }

        output[inputLength] = '\0';

        return output;
    }

    void LoginRequestMessage::PopulateCredentials()
    {
        Enigma::c8* temp;
        Enigma::c8* temp2;
        temp=this->GetInnerMessage();

        temp2=strtok(temp,"|");
        if(temp2==NULL)
        {
            temp2="";
        }
        this->mUsername=temp2;

        temp2=strtok(NULL,"|");
        if(temp2==NULL)
        {
            temp2="";
        }
        this->mPassword = temp2;
    }
};
