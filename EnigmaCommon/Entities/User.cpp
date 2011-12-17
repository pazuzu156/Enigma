/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "User.hpp"

#include "../time.hpp"
#include "../iostream.hpp"

namespace Enigma
{
    User::User()
    {
        this->mUserName="";

		this->mCurrentCharacterId=0;
		this->mCurrentMapId=0;
		this->mCurrentVoiceChannel=CHAT_TYPE_MAP;

		this->mIsLoggedIn=false;
        this->mHasEnteredWorld=false;
    }

	User::User(const std::string& value)
	{
        this->mUserName=value;

		this->mCurrentCharacterId=0;
		this->mCurrentMapId=0;
		this->mCurrentVoiceChannel=CHAT_TYPE_MAP;

		this->mIsLoggedIn=true; //if there is a username the user is logged in.
        this->mHasEnteredWorld=false;
	}

    User::~User()
    {

    }

    const std::string& User::GetUserName() const
	{
		return this->mUserName;
	}

    void User::SetUserName(const std::string& value)
	{
		this->mUserName = value;
	}

    const size_t User::GetCurrentCharacterId() const
	{
		return this->mCurrentCharacterId;
	}

	void User::SetCurrentCharacterId(size_t value)
	{
		this->mCurrentCharacterId = value;
	}

    const size_t User::GetCurrentMapId() const
	{
		return this->mCurrentMapId;
	}

	void User::SetCurrentMapId(size_t value)
	{
		this->mCurrentMapId = value;
	}

	const bool User::GetIsLoggedIn() const
	{
		return this->mIsLoggedIn;
	}

	void User::SetIsLoggedIn(bool value)
	{
		this->mIsLoggedIn = value;
	}

    const bool User::GetHasEnteredWorld() const
	{
		return this->mHasEnteredWorld;
	}

	void User::SetHasEnteredWorld(bool value)
	{
		this->mHasEnteredWorld = value;
	}

	std::map<size_t,Item>& User::GetStash()
	{
		return this->mStash;
	}

	const size_t User::GetCurrentVoiceChannel() const
	{
		return this->mCurrentVoiceChannel;
	}

	void User::SetCurrentVoiceChannel(size_t value)
	{
		this->mCurrentVoiceChannel = value;
	}
}
