/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "Map.hpp"

namespace Enigma
{
	Map::Map()
	{
		//this->mCharacters.insert(CharacterPair(0,Character()));
		//this->mSpawnLocations.push_back(SpawnLocation());
		//this->mNpcs.insert(NpcPair("",Npc()));
		//this->mMonsters.insert(MonsterPair(0,Monster()));
		//this->mItems.insert(ItemPair(0,Item()));
	}

	Map::~Map()
	{

	}

	std::map<size_t,Character>& Map::GetCharacters()
	{
		return this->mCharacters;
	}

	std::vector<SpawnLocation>& Map::GetSpawnLocations()
	{
		return this->mSpawnLocations;
	}

	std::map<std::string,Npc>& Map::GetNpcs()
	{
		return this->mNpcs;
	}

	std::map<size_t,Monster>& Map::GetMonsters()
	{
		return this->mMonsters;
	}

	std::map<size_t,Item>& Map::GetItems()
	{
		return this->mItems;
	}

	Npc& Map::GetNpc(std::string id)
	{
		return this->mNpcs[id];
	}

	Npc& Map::GetNpc(Enigma::c8* id)
	{
		return this->mNpcs[std::string(id)];
	}

	Npc& Map::GetNpc(size_t id)
	{
		std::string stringId;

		BOOST_FOREACH( NpcPair npcPair,this->mNpcs )
		{
			if(npcPair.second.GetId()==id)
			{
				stringId=npcPair.first;
			}
		}

		return this->mNpcs[stringId];
	}

	const std::string& Map::FindNpc(size_t id)
	{
		BOOST_FOREACH( NpcPair npcPair,this->mNpcs )
		{
			if(npcPair.second.GetId()==id)
			{
				return npcPair.first;
			}
		}
		
		return mBlank;
	}

	void Map::ClearMap()
	{
		this->mCharacters.clear();
		this->mSpawnLocations.clear();
		this->mNpcs.clear();
		this->mMonsters.clear();
		this->mItems.clear();
	}
}
