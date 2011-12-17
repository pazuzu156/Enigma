/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "Rank.hpp"

namespace Enigma
{

    Rank::Rank()
    {
        this->SetPermissions(0);
    }

    Rank::~Rank()
    {

    }

    size_t Rank::GetPermissions() const
    {
        return this->mPermissions;
    }

    void Rank::SetPermissions(size_t value)
    {
        this->mPermissions=value;
    }

    bool Rank::GetCanInvite() const
    {
        return (this->mPermissions & 0x01)!=0;
    }

    const std::string& Rank::GetName() const
    {
        return this->mName;
    }

    void Rank::SetName(const std::string& value)
    {
        this->mName=value;
    }

    void Rank::SetCanInvite(bool value)
    {
        if(value)
        {
            this->mPermissions |= 0x01;
        }
        else
        {
            this->mPermissions &= ~0x01;
        }
    }

    bool Rank::GetCanKick() const
    {
        return (this->mPermissions & 0x02)!=0;
    }

    void Rank::SetCanKick(bool value)
    {
        if(value)
        {
            this->mPermissions |= 0x02;
        }
        else
        {
            this->mPermissions &= ~0x02;
        }
    }

    bool Rank::GetCanAdjustRank() const
    {
        return (this->mPermissions & 0x04)!=0;
    }

    void Rank::SetCanAdjustRank(bool value)
    {
         if(value)
        {
            this->mPermissions |= 0x04;
        }
        else
        {
            this->mPermissions &= ~0x04;
        }
    }

};
