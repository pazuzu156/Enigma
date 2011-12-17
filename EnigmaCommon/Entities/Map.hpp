#ifndef MAP_HPP_INCLUDED
#define MAP_HPP_INCLUDED

/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "../std_map.hpp"
#include "../std_vector.hpp"

#include "Character.hpp"
#include "SpawnLocation.hpp"
#include "Npc.hpp"
#include "Item.hpp"

#include "boost/foreach.hpp"

namespace Enigma
{
    class DllExport Map : public Object, public StorableObject
    {
        private:

        std::map<size_t,Character> mCharacters;
        std::vector<SpawnLocation> mSpawnLocations;
        std::map<std::string,Npc> mNpcs;
        std::map<size_t,Monster> mMonsters;
        std::map<size_t,Item> mItems;

        protected:

        public:

        Map();
        ~Map();

        std::map<size_t,Character>& GetCharacters();
        std::vector<SpawnLocation>& GetSpawnLocations();
        std::map<std::string,Npc>& GetNpcs();
        std::map<size_t,Monster>& GetMonsters();
        std::map<size_t,Item>& GetItems();

        Npc& GetNpc(std::string id);
        Npc& GetNpc(Enigma::c8* id);
        Npc& GetNpc(size_t id);
		const std::string& FindNpc(size_t id);

		void ClearMap();
    };

    typedef std::pair<size_t,Character> CharacterPair;
    typedef std::pair<std::string,Npc> NpcPair;
    typedef std::pair<size_t,Monster> MonsterPair;
    typedef std::pair<size_t,Item> ItemPair;
}

#endif // MAP_HPP_INCLUDED
