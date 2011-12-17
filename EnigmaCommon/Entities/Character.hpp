#ifndef CHARACTER_HPP_INCLUDED
#define CHARACTER_HPP_INCLUDED

/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "Combatant.hpp"
#include "Item.hpp"

#include "../std_map.hpp"

namespace Enigma
{
    class DllExport Character : public Combatant
    {
        private:

        size_t mUserId;
        size_t mPartyId;
		size_t mGuildId;
        std::string mRankName;

		size_t mCurrentNpcId;
        std::string mNpcQuestionId;

        //Actual equipment.
        Item mLeftHandEquipment;
        Item mRightHandEquipment;
        Item mGlovesEquipment;
        Item mShoesEquipment;
        Item mBeltEquipment;
        Item mHelmetEquipment;
        Item mArmorEquipment;

        //Appearance equipment.
        Item mLeftHandAppearance;
        Item mRightHandAppearance;
        Item mGlovesAppearance;
        Item mShoesAppearance;
        Item mBeltAppearance;
        Item mHelmetAppearance;
        Item mArmorAppearance;

        std::map<size_t,Item> mInventory;

        protected:

        public:

        Character();
        ~Character();

        size_t GetPartyId();
        void SetPartyId(size_t value);

        size_t GetGuildId();
        void SetGuildId(size_t value);

        const std::string& GetRankName();
        void SetRankName(const std::string& value);

		size_t GetCurrentNpcId();
        void SetCurrentNpcId(size_t value);

		const std::string& GetNpcQuestionId();
        void SetNpcQuestionId(const std::string& value);

        size_t GetUserId();
		void SetUserId(size_t value);

        //Actual equipment.
        Item& GetLeftHandEquipment();
        void SetLeftHandEquipment(Item& item);

        Item& GetRightHandEquipment();
        void SetRightHandEquipment(Item& item);

        Item& GetGlovesEquipment();
        void SetGlovesEquipment(Item& item);

        Item& GetShoesEquipment();
        void SetShoesEquipment(Item& item);

        Item& GetBeltEquipment();
        void SetBeltEquipment(Item& item);

        Item& GetHelmetEquipment();
        void SetHelmetEquipment(Item& item);

        Item& GetArmorEquipment();
        void SetArmorEquipment(Item& item);

        //Appearance
        Item& GetLeftHandAppearance();
        void SetLeftHandAppearance(Item& item);

        Item& GetRightHandAppearance();
        void SetRightHandAppearance(Item& item);

        Item& GetGlovesAppearance();
        void SetGlovesAppearance(Item& item);

        Item& GetShoesAppearance();
        void SetShoesAppearance(Item& item);

        Item& GetBeltAppearance();
        void SetBeltAppearance(Item& item);

        Item& GetHelmetAppearance();
        void SetHelmetAppearance(Item& item);

        Item& GetArmorAppearance();
        void SetArmorAppearance(Item& item);

        std::map<size_t,Item>& GetInventory();
    };
};

#endif // CHARACTER_HPP_INCLUDED
