/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "stdafx.h"

#include "Skill.hpp"

namespace Enigma
{
        Skill::Skill(Attribute energyCost,Attribute healthCost,bool isAreaOfEffect)
        {
            this->mEnergyCost=energyCost;
            this->mHealthCost=healthCost;
			this->mIsAreaOfEffect=isAreaOfEffect;

        }

        Skill::Skill()
        {
            this->mEnergyCost=0; //most skills will take a small amount of energy.
            this->mHealthCost=0; //most skills will not take health.
			this->mIsAreaOfEffect=false; //most skills will be single target.
        }

        Skill::~Skill()
        {

        }

		bool Skill::GetIsAreaOfEffect()
		{
			return this->mIsAreaOfEffect;
		}

        bool Skill::ApplyCost(ICombatant& source,ICombatant& target)
        {
            Attribute currentHealth=0;
            Attribute currentEnergy=0;

            currentHealth=source.GetHealth();
            currentEnergy=source.GetEnergy();

            if(currentHealth<=this->mHealthCost)
            {
                return false;
            }

            if(currentEnergy<=this->mEnergyCost)
            {
                return false;
            }

			source.SetHealth(currentHealth-this->mHealthCost);
			source.SetEnergy(currentEnergy-this->mEnergyCost);

            return true;
        }
};
