/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "NpcChatResponseMessage.hpp"

namespace Enigma
{

    NpcChatResponseMessage::NpcChatResponseMessage()
    {
        size_t size;
        size = NpcChatResponseMessage::GetMessageLength();
        this->ResizeMessage(size,0);
        this->SetType(NpcChatResponseMessage::GetMessageType());

        this->mChatResponse="";
        this->mNpcId=0;

		//this->SetChannel(CHANNEL_AUTHENTICATION);
		//this->SetChannel(CHANNEL_MOVEMENT);
		//this->SetChannel(CHANNEL_COMBAT);
		this->SetChannel(CHANNEL_CHAT);
		//this->SetChannel(CHANNEL_VOICE_CHAT);	
	}

    NpcChatResponseMessage::~NpcChatResponseMessage()
    {

    }

    Enigma::s32 NpcChatResponseMessage::GetNpcId()
    {
        return this->mNpcId;
    }

    void NpcChatResponseMessage::SetNpcId(Enigma::s32 value)
    {
        this->mNpcId=value;
    }

    const std::string& NpcChatResponseMessage::GetChatResponse() const
    {
        return this->mChatResponse;
    }

    void NpcChatResponseMessage::SetChatResponse(std::string& value)
    {
        this->mChatResponse=value;
    }

    void NpcChatResponseMessage::SetChatResponse(const std::string& value)
    {
        this->mChatResponse=value;
    }

    std::vector<std::string>& NpcChatResponseMessage::GetPossiblePlayerResponses()
    {
        return this->mPossiblePlayerResponses;
    }

    void NpcChatResponseMessage::UpdateMessageData()
    {
        std::string temp="";
        size_t length=1;

        try
        {

            if(this->mChatResponse.length()>0)
            {
                temp.append(this->mChatResponse);
                length+=this->mChatResponse.length();
            }

            temp.append("~");
            length+=1;

            if(this->mPossiblePlayerResponses.size()>0)
            {
                BOOST_FOREACH(std::string response, this->mPossiblePlayerResponses)
                {
                    temp.append(response);
                    length+=response.length();

                    temp.append("^");
                    length+=1;
                }
            }
            else
            {
                //std::cout << "No anwsers" << std::endl;
            }

            this->ResizeMessage(length,0);

            this->SetInt(0,this->mNpcId);

            std::cout << "Npc chat response message data: " << temp << std::endl;

            this->SetString(1,temp);


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

    void NpcChatResponseMessage::GetMessageData()
    {
        const size_t nothing = -1; //same as npos which find returns if nothing is found.
        size_t split=0;
        std::string temp="";
        std::string responses="";
        std::string response="";

        try
        {
            this->mNpcId=this->GetInt(0);
            temp=this->GetString(1);

            std::cout << "Npc chat response message data: " << temp << std::endl;

            split=temp.find("~");
            if(split!=nothing)
            {
                this->mChatResponse = temp.substr(0,split);
                responses = temp.substr(split,(temp.size()-this->mChatResponse.size()));
                split=0;
                while(split!=nothing)
                {
                    responses = responses.substr(split+1,responses.size()-response.size());
                    split=responses.find("^");

                    if(split!=nothing && responses.size()>split)
                    {
                        response = responses.substr(0,split);
                    }
                    else
                    {
                        if(responses.size()>1)
                        {
                            response = responses.substr(0,responses.size());
                        }
                        else
                        {
                           response = "";
                        }
                    }

                    if(response.length()>0)
                    {
                        this->mPossiblePlayerResponses.insert(this->mPossiblePlayerResponses.begin(),response);
                    }
                }
            }
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
};
