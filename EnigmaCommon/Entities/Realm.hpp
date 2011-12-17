#ifndef REALM_HPP_INCLUDED
#define REALM_HPP_INCLUDED

/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "../TypeDefs.hpp"
#include <boost/thread.hpp> 

#include "Map.hpp"
#include "Party.hpp"
#include "Guild.hpp"
#include "User.hpp"

#include "../std_vector.hpp"
#include "../std_map.hpp"

namespace Enigma
{
    class DllExport Realm : public Object
    {
        private:

        std::vector<Map> mMaps;
        std::map<std::string,User> mUsers;
		std::vector<Party> mParties;
        std::map<size_t,Guild> mGuilds;

        protected:

        public:

        Realm();
        ~Realm();
		Map& GetMap(size_t index);
        std::vector<Map>& GetMaps();
        std::map<std::string,User>& GetUsers();
		std::vector<Party>& GetParties();
        std::map<size_t,Guild>& GetGuilds();	

		mutable boost::shared_mutex ApplicationMutex;
		mutable boost::shared_mutex SceneMutex;
		mutable boost::shared_mutex ChatMutex;
		//mutable boost::shared_mutex AudioMutex;
		mutable boost::shared_mutex CombatMutex;
    };

	typedef std::pair<std::string,User> UserPair;
	typedef std::pair<size_t,Guild> GuildPair;
};

#endif // REALM_HPP_INCLUDED
