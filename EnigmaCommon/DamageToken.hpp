#ifndef DAMAGETOKEN_HPP_INCLUDED
#define DAMAGETOKEN_HPP_INCLUDED

/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "DamageTypes.hpp"
#include "ImpactTypes.hpp"

namespace Enigma
{
    struct DamageToken
    {
        private:

        size_t mDamageType;
        size_t mImpactType;
        int mDamageAmount;

        protected:

        public:

        DamageToken()
        {
            this->mDamageType=DAMAGE_TYPE_PHYSICAL;
            this->mImpactType=IMPACT_TYPE_NONE;
            this->mDamageAmount=0;
        }

        ~DamageToken()
        {

        }

        size_t GetDamageType() const {return this->mDamageType;}
        void SetDamageType(size_t value){this->mDamageType=value;}

        size_t GetImpactType() const {return this->mImpactType;}
        void SetImpactType(size_t value){this->mImpactType=value;}

        int GetDamageAmount() const {return this->mDamageAmount;}
        void SetDamageAmount(int value){this->mDamageAmount=value;}
    };
};

#endif // DAMAGETOKEN_HPP_INCLUDED
