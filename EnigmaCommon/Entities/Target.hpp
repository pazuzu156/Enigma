#ifndef TARGET_HPP_INCLUDED
#define TARGET_HPP_INCLUDED

/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "../TargetPreferenceTypes.hpp"

namespace Enigma
{
    class DllExport Target
    {
        private:
			
		size_t mCharacterTarget;
        size_t mMonsterTarget;
        size_t mNpcTarget;
        size_t mItemTarget;
		size_t mTargetPreference;

        protected:

        public:

		Target();
		~Target();
		
		size_t GetTargetPreference() const;
		void SetTargetPreference(size_t value);

		size_t GetCharacterTarget() const;
		void SetCharacterTarget(size_t value);

		size_t GetMonsterTarget() const;
		void SetMonsterTarget(size_t value);

		size_t GetNpcTarget() const;
		void SetNpcTarget(size_t value);

		size_t GetItemTarget() const;
		void SetItemTarget(size_t value);
    };
};

#endif // TARGET_HPP_INCLUDED