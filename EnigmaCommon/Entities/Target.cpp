/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "Target.hpp"

namespace Enigma
{
	Target::Target()
	{
		this->mCharacterTarget=0;
		this->mMonsterTarget=0;
		this->mNpcTarget=0;
		this->mItemTarget=0;
		this->mTargetPreference=TARGETPREFERENCETYPE_NONE;
	}

	Target::~Target()
	{

	}
	
	size_t Target::GetTargetPreference() const
	{
		return this->mTargetPreference;
	}

	void Target::SetTargetPreference(size_t value)
	{
		this->mTargetPreference=value;
	}

	size_t Target::GetCharacterTarget() const
	{
		return this->mCharacterTarget;
	}

	void Target::SetCharacterTarget(size_t value)
	{
		this->mCharacterTarget=value;
	}

	size_t Target::GetMonsterTarget() const
	{
		return this->mMonsterTarget;
	}

	void Target::SetMonsterTarget(size_t value)
	{
		this->mMonsterTarget=value;
	}

	size_t Target::GetNpcTarget() const
	{
		return this->mNpcTarget;
	}

	void Target::SetNpcTarget(size_t value)
	{
		this->mNpcTarget=value;
	}

	size_t Target::GetItemTarget() const
	{
		return this->mItemTarget;
	}

	void Target::SetItemTarget(size_t value)
	{
		this->mItemTarget=value;
	}
};
