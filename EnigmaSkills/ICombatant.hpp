#ifndef ICOMBATANT_HPP_INCLUDED
#define ICOMBATANT_HPP_INCLUDED

/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#ifndef ENIMGA_ATTRIBUTE
	#define ENIMGA_ATTRIBUTE
	typedef size_t Attribute;
#endif

#include "TypeDefs.hpp"

#include "DamageToken.hpp"

#include "IBuff.hpp"

namespace Enigma
{
    class DllExport ICombatant
    {
		private:

		protected:

		public:

		virtual ~ICombatant(){};

		//stat
		virtual Attribute GetStrength()=0;
        virtual void SetStrength(Attribute value)=0;

        virtual Attribute GetWisdom()=0;
        virtual void SetWisdom(Attribute value)=0;

        virtual Attribute GetIntelligence()=0;
        virtual void SetIntelligence(Attribute value)=0;

        virtual Attribute GetDexterity()=0;
        virtual void SetDexterity(Attribute value)=0;

        virtual Attribute GetAgility()=0;
        virtual void SetAgility(Attribute value)=0;

		//stat experience
		virtual Attribute GetStrengthExperience()=0;
        virtual void SetStrengthExperience(Attribute value)=0;

        virtual Attribute GetWisdomExperience()=0;
        virtual void SetWisdomExperience(Attribute value)=0;

        virtual Attribute GetIntelligenceExperience()=0;
        virtual void SetIntelligenceExperience(Attribute value)=0;

        virtual Attribute GetDexterityExperience()=0;
        virtual void SetDexterityExperience(Attribute value)=0;

        virtual Attribute GetAgilityExperience()=0;
        virtual void SetAgilityExperience(Attribute value)=0;

		//pools
		virtual Attribute GetHealth()=0;
        virtual void SetHealth(Attribute value)=0;

        virtual Attribute GetMaxHealth()=0;
        virtual void SetMaxHealth(Attribute value)=0;

        virtual Attribute GetEnergy()=0;
        virtual void SetEnergy(Attribute value)=0;

        virtual Attribute GetMaxEnergy()=0;
        virtual void SetMaxEnergy(Attribute value)=0;

		//level
		virtual Attribute GetBaseLevel()=0;
        virtual void SetBaseLevel(Attribute value)=0;

        virtual Attribute GetBaseLevelExperience()=0;
        virtual void SetBaseLevelExperience(Attribute value)=0;

		//resists
		virtual Attribute GetPhysicalResist()=0;
		virtual Attribute GetMagicalResist()=0;
		virtual Attribute GetDarkResist()=0;
		virtual Attribute GetLightResist()=0;
		virtual Attribute GetFireResist()=0;
		virtual Attribute GetWindResist()=0;
		virtual Attribute GetWaterResist()=0;
		virtual Attribute GetEarthResist()=0;
		virtual Attribute GetTimeResist()=0;
		virtual Attribute GetIceResist()=0;
		virtual Attribute GetElectricResist()=0;
		virtual Attribute GetPoisonGenericResist()=0;
		virtual Attribute GetPoisonSnakeResist()=0;
		virtual Attribute GetPoisonSpiderResist()=0;
		virtual Attribute GetPoisonToadResist()=0;
		virtual Attribute GetPoisonPlantResist()=0;
		virtual Attribute GetPoisonFishResist()=0;
		virtual Attribute GetPoisonAcidResist()=0;

		//penetration
		virtual Attribute GetPhysicalPenetration()=0;
		virtual Attribute GetMagicalPenetration()=0;
		virtual Attribute GetDarkPenetration()=0;
		virtual Attribute GetLightPenetration()=0;
		virtual Attribute GetFirePenetration()=0;
		virtual Attribute GetWindPenetration()=0;
		virtual Attribute GetWaterPenetration()=0;
		virtual Attribute GetEarthPenetration()=0;
		virtual Attribute GetTimePenetration()=0;
		virtual Attribute GetIcePenetration()=0;
		virtual Attribute GetElectricPenetration()=0;
		virtual Attribute GetPoisonGenericPenetration()=0;
		virtual Attribute GetPoisonSnakePenetration()=0;
		virtual Attribute GetPoisonSpiderPenetration()=0;
		virtual Attribute GetPoisonToadPenetration()=0;
		virtual Attribute GetPoisonPlantPenetration()=0;
		virtual Attribute GetPoisonFishPenetration()=0;
		virtual Attribute GetPoisonAcidPenetration()=0;

		//buffs
		virtual void ApplyBuff(IBuff* buff)=0;
		virtual void RemoveBuff(IBuff* buff)=0;
		virtual void RemoveAllBuffs()=0;

		//misc
		virtual bool GetIsCharacter()=0;
		virtual bool GetIsNpc()=0;
		virtual bool GetIsMonster()=0;
		virtual bool GetIsInParty(ICombatant& value)=0;
	};

    //Apply damage without impact type calculations.
    inline void ApplyDamage(ICombatant& source,ICombatant& target,unsigned int damageType,unsigned int impactType,int amount)
    {

        int applicableResist=0;
		int applicablePenetration=0;

        int health=0;
		double damageReduction;

        switch(damageType)
        {
            case DAMAGE_TYPE_PHYSICAL:
                    applicableResist = target.GetPhysicalResist();
					applicablePenetration = source.GetPhysicalPenetration();
                break;
            case DAMAGE_TYPE_MAGICAL:
                    applicableResist = target.GetMagicalResist();
					applicablePenetration = source.GetMagicalPenetration();
                break;
            case DAMAGE_TYPE_DARK:
                    applicableResist = target.GetDarkResist();
					applicablePenetration = source.GetDarkPenetration();
                break;
            case DAMAGE_TYPE_LIGHT:
                    applicableResist = target.GetLightResist();
					applicablePenetration = source.GetLightPenetration();
                break;
            case DAMAGE_TYPE_FIRE:
                    applicableResist = target.GetFireResist();
					applicablePenetration = source.GetFirePenetration();
                break;
            case DAMAGE_TYPE_WIND:
                    applicableResist = target.GetWindResist();
					applicablePenetration = source.GetWindPenetration();
                break;
            case DAMAGE_TYPE_WATER:
                    applicableResist = target.GetWaterResist();
					applicablePenetration = source.GetWaterPenetration();
                break;
            case DAMAGE_TYPE_EARTH:
                    applicableResist = target.GetEarthResist();
					applicablePenetration = source.GetEarthPenetration();
                break;
            case DAMAGE_TYPE_TIME:
                    applicableResist = target.GetTimeResist();
					applicablePenetration = source.GetTimePenetration();
                break;
            case DAMAGE_TYPE_ICE:
                    applicableResist = target.GetIceResist();
					applicablePenetration = source.GetIcePenetration();
                break;
            case DAMAGE_TYPE_ELECTRIC:
                    applicableResist = target.GetElectricResist();
					applicablePenetration = source.GetElectricPenetration();
                break;
            case DAMAGE_TYPE_POISON_GENERIC:
                    applicableResist = target.GetPoisonGenericResist();
					applicablePenetration = source.GetPoisonGenericPenetration();
                break;
            case DAMAGE_TYPE_POISON_SNAKE:
                    applicableResist = target.GetPoisonSnakeResist();
					applicablePenetration = source.GetPoisonSnakePenetration();
                break;
            case DAMAGE_TYPE_POISON_SPIDER:
                    applicableResist = target.GetPoisonSpiderResist();
					applicablePenetration = source.GetPoisonSpiderPenetration();
                break;
            case DAMAGE_TYPE_POISON_TOAD:
                    applicableResist = target.GetPoisonToadResist();
					applicablePenetration = source.GetPoisonToadPenetration();
                break;
            case DAMAGE_TYPE_POISON_PLANT:
                    applicableResist = target.GetPoisonPlantResist();
					applicablePenetration = source.GetPoisonPlantPenetration();
                break;
            case DAMAGE_TYPE_POISON_FISH:
                    applicableResist = target.GetPoisonFishResist();
					applicablePenetration = source.GetPoisonFishPenetration();
                break;
            case DAMAGE_TYPE_POISON_ACID:
                    applicableResist = target.GetPoisonAcidResist();
					applicablePenetration = source.GetPoisonAcidPenetration();
                break;
            default:
                    applicableResist = 1;
					applicablePenetration = 0;
                break;
        }

        health = target.GetHealth();

		if(amount>0)
		{
			if(source.GetBaseLevel() < target.GetBaseLevel())
			{
				applicableResist = (applicableResist / (target.GetBaseLevel()-source.GetBaseLevel()));
			}

			if(source.GetBaseLevel() > target.GetBaseLevel())
			{
				applicableResist = (applicableResist * (source.GetBaseLevel()-target.GetBaseLevel()));
			}
			damageReduction = 1.00f * applicableResist / (applicablePenetration + applicableResist);

			if(damageReduction>0.99)
			{
				damageReduction=0.99;
			}

			health-=(amount-(amount*damageReduction));
		}

        target.SetHealth(health);
    }

    //Apply damage without impact type calculations.
    inline void ApplyDamage(ICombatant& source,ICombatant& target,DamageToken& damageToken)
    {
		ApplyDamage(source,target,damageToken.GetDamageType(),damageToken.GetImpactType(),damageToken.GetDamageAmount());
	}

    //Apply damage without impact type calculations.
    inline void ApplyHeal(ICombatant& source,ICombatant& target,DamageToken& damageToken)
    {
		int health=0;
		health = target.GetHealth();
		health+=damageToken.GetDamageAmount();

		if(health < target.GetMaxHealth())
		{
			target.SetHealth(health);
		}
		else
		{
			target.SetHealth(target.GetMaxHealth());
		}
	}
};

#endif // ICOMBATANT_HPP_INCLUDED