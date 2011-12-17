#ifndef RANK_HPP_INCLUDED
#define RANK_HPP_INCLUDED

/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "Object.hpp"

namespace Enigma
{
	class DllExport Rank : public Object
    {
        private:

        size_t mPermissions;
		std::string mName;

        protected:

        public:

        Rank();
        ~Rank();

        size_t GetId() const;
        void SetId(size_t value);

        size_t GetPermissions() const;
        void SetPermissions(size_t value);

        const std::string& GetName() const;
        void SetName(const std::string& value);

        bool GetCanInvite() const;
        void SetCanInvite(bool value);

        bool GetCanKick() const;
        void SetCanKick(bool value);

        bool GetCanAdjustRank() const;
        void SetCanAdjustRank(bool value);
    };
};

#endif // RANK_HPP_INCLUDED
