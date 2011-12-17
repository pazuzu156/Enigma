/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "Character.hpp"

namespace Enigma
{
    Character::Character()
    {
		this->mCurrentNpcId=0;
        this->mUserId=0;
        this->mPartyId=0;
		this->mGuildId=0;
		this->mRankName="";
    }

    Character::~Character()
    {
    }

    size_t Character::GetPartyId()
    {
        return this->mPartyId;
    }

    void Character::SetPartyId(size_t value)
    {
        this->mPartyId=value;
    }

    size_t Character::GetGuildId()
    {
        return this->mGuildId;
    }

    void Character::SetGuildId(size_t value)
    {
        this->mGuildId=value;
    }

    const std::string& Character::GetRankName()
    {
        return this->mRankName;
    }

    void Character::SetRankName(const std::string& value)
    {
        this->mRankName=value;
    }

	size_t Character::GetCurrentNpcId()
	{
		return this->mCurrentNpcId;
	}

    void Character::SetCurrentNpcId(size_t value)
	{
		this->mCurrentNpcId = value;
	}

	const std::string& Character::GetNpcQuestionId()
	{
		return this->mNpcQuestionId;
	}

    void Character::SetNpcQuestionId(const std::string& value)
	{
		this->mNpcQuestionId = value;
	}

    size_t Character::GetUserId()
    {
        return this->mUserId;
    }

	void Character::SetUserId(size_t value)
	{
		this->mUserId = value;
	}

    //Actual equipment.
    Item& Character::GetLeftHandEquipment()
    {
        return this->mLeftHandEquipment;
    }

    void Character::SetLeftHandEquipment(Item& item)
    {
        this->mLeftHandEquipment=item;
    }

    Item& Character::GetRightHandEquipment()
    {
        return this->mRightHandEquipment;
    }

    void Character::SetRightHandEquipment(Item& item)
    {
        this->mRightHandEquipment=item;
    }

    Item& Character::GetGlovesEquipment()
    {
        return this->mGlovesEquipment;
    }

    void Character::SetGlovesEquipment(Item& item)
    {
        this->mGlovesEquipment=item;
    }

    Item& Character::GetShoesEquipment()
    {
        return this->mShoesEquipment;
    }

    void Character::SetShoesEquipment(Item& item)
    {
        this->mShoesEquipment=item;
    }

    Item& Character::GetBeltEquipment()
    {
        return this->mBeltEquipment;
    }

    void Character::SetBeltEquipment(Item& item)
    {
        this->mBeltEquipment=item;
    }

    Item& Character::GetHelmetEquipment()
    {
        return this->mHelmetEquipment;
    }

    void Character::SetHelmetEquipment(Item& item)
    {
        this->mHelmetEquipment=item;
    }

    Item& Character::GetArmorEquipment()
    {
        return this->mArmorEquipment;
    }

    void Character::SetArmorEquipment(Item& item)
    {
        this->mArmorEquipment=item;
    }

    //Appearance
    Item& Character::GetLeftHandAppearance()
    {
        return this->mLeftHandAppearance;
    }

    void Character::SetLeftHandAppearance(Item& item)
    {
        this->mLeftHandAppearance=item;
    }

    Item& Character::GetRightHandAppearance()
    {
        return this->mRightHandAppearance;
    }

    void Character::SetRightHandAppearance(Item& item)
    {
        this->mRightHandAppearance=item;
    }

    Item& Character::GetGlovesAppearance()
    {
        return this->mGlovesAppearance;
    }

    void Character::SetGlovesAppearance(Item& item)
    {
        this->mGlovesAppearance=item;
    }

    Item& Character::GetShoesAppearance()
    {
        return this->mShoesAppearance;
    }

    void Character::SetShoesAppearance(Item& item)
    {
        this->mShoesAppearance=item;
    }

    Item& Character::GetBeltAppearance()
    {
        return this->mBeltAppearance;
    }

    void Character::SetBeltAppearance(Item& item)
    {
        this->mBeltAppearance=item;
    }

    Item& Character::GetHelmetAppearance()
    {
        return this->mHelmetAppearance;
    }

    void Character::SetHelmetAppearance(Item& item)
    {
        this->mHelmetAppearance=item;
    }

    Item& Character::GetArmorAppearance()
    {
        return this->mArmorAppearance;
    }

    void Character::SetArmorAppearance(Item& item)
    {
        this->mArmorAppearance=item;
    }

    std::map<size_t,Item>& Character::GetInventory()
    {
        return this->mInventory;
    }
}
