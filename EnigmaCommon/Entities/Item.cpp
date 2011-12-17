/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "Item.hpp"

namespace Enigma
{
    Item::Item()
    {
        this->mDurability=0;
        this->mMaxDurability=0;
		this->mStackSize=0;
    }

    Item::~Item()
    {

    }

    size_t Item::GetDurability()
    {
        return this->mDurability;
    }

    void Item::SetDurability(size_t value)
    {
        this->mDurability=value;
    }

    size_t Item::GetMaxDurability()
    {
        return this->mMaxDurability;
    }

    void Item::SetMaxDurability(size_t value)
    {
        this->mMaxDurability=value;
    }

    size_t Item::GetStackSize()
    {
        return this->mStackSize;
    }

    void Item::SetStackSize(size_t value)
    {
        this->mStackSize=value;
    }
}
