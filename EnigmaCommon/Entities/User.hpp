#ifndef USER_HPP_INCLUDED
#define USER_HPP_INCLUDED

/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "../string.hpp"
#include "../std_map.hpp"
#include "../TypeDefs.hpp"
#include "Item.hpp"
#include "Character.hpp"
#include "../StorableObject.hpp"
#include "../ChatType.hpp"

namespace Enigma
{
	class DllExport User : public StorableObject
    {
        private:

        std::string mUserName;

		size_t mCurrentCharacterId;
		size_t mCurrentMapId;
		size_t mCurrentVoiceChannel;

		bool mIsLoggedIn;
        bool mHasEnteredWorld;

		std::map<size_t,Item> mStash;

		std::vector<Enigma::Character> mAvailableCharacters;

        protected:

        public:

        User();
		User(const std::string& value);
        ~User();

		std::vector<Enigma::Character>& GetAvailableCharacters(){return mAvailableCharacters;}

        const std::string& GetUserName() const;
        void SetUserName(const std::string& value);

        const size_t GetCurrentCharacterId() const;
		void SetCurrentCharacterId(size_t value);

        const size_t GetCurrentMapId() const;
		void SetCurrentMapId(size_t value);

		const bool GetIsLoggedIn() const;
		void SetIsLoggedIn(bool value);

        const bool GetHasEnteredWorld() const;
		void SetHasEnteredWorld(bool value);

		std::map<size_t,Item>& GetStash();

		const size_t GetCurrentVoiceChannel() const;
		void SetCurrentVoiceChannel(size_t value);
    };
};

#endif // USER_HPP_INCLUDED
