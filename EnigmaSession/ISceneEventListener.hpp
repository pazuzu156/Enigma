#ifndef ISCENEEVENTLISTENER_HPP_INCLUDED
#define ISCENEEVENTLISTENER_HPP_INCLUDED

/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "TypeDefs.hpp"

#include "Entities\Character.hpp"
#include "Entities\Npc.hpp"
#include "Entities\Monster.hpp"
#include "Entities\Item.hpp"

namespace Enigma
{
    class DllExport ISceneEventListener
    {
        private:

        protected:

        public:

        virtual ~ISceneEventListener(){};

		/*
		 * This method is called when the player entered event occurs.
		 * When this event is received one of the listeners should be responsible for adding any meshes and such to the scene.
		 */
		virtual void onPlayerEnteredMap(Enigma::s32 characterId,const Enigma::Character& character)=0;

		/*
		 * This method is called when the npc entered event occurs.
		 * When this event is received one of the listeners should be responsible for adding any meshes and such to the scene.
		 */
		virtual void onNpcEnteredMap(Enigma::s32 npcId,const Enigma::Npc& npc)=0;

		/*
		 * This method is called when the monster entered event occurs.
		 * When this event is received one of the listeners should be responsible for adding any meshes and such to the scene.
		 */
		virtual void onMonsterEnteredMap(Enigma::s32 monsterId,const Enigma::Monster& monster)=0;

		/*
		 * This method is called when the item entered event occurs.
		 * When this event is received one of the listeners should be responsible for adding any meshes and such to the scene.
		 */
		virtual void onItemEnteredMap(Enigma::s32 itemId,const Enigma::Item& item)=0;


		/*
		 * This method is called when the player exited event occurs.
		 * When this event is received one of the listeners should be responsible for removing any meshes and such from the scene.
		 */
		virtual void onPlayerExitedMap(Enigma::s32 characterId,const Enigma::Character& character)=0;

		/*
		 * This method is called when the npc exited event occurs.
		 * When this event is received one of the listeners should be responsible for removing any meshes and such from the scene.
		 */
		virtual void onNpcExitedMap(Enigma::s32 npcId,const Enigma::Npc& npc)=0;

		/*
		 * This method is called when the monster exited event occurs.
		 * When this event is received one of the listeners should be responsible for removing any meshes and such from the scene.
		 */
		virtual void onMonsterExitedMap(Enigma::s32 monsterId,const Enigma::Monster& monster)=0;

		/*
		 * This method is called when the item exited event occurs.
		 * When this event is received one of the listeners should be responsible for removing any meshes and such from the scene.
		 */
		virtual void onItemExitedMap(Enigma::s32 itemId,const Enigma::Item& item)=0;


		/*
		 * This method is called when the player update event occurs.
		 * When this event is received one or more of the listeners should be responsible for updating any client elements related to the entity's new state.
		 */
		virtual void onPlayerUpdate(Enigma::s32 characterId,const Enigma::Character& character)=0;

		/*
		 * This method is called when the npc update event occurs.
		 * When this event is received one or more of the listeners should be responsible for updating any client elements related to the entity's new state.
		 */
		virtual void onNpcUpdate(Enigma::s32 npcId,const Enigma::Npc& npc)=0;

		/*
		 * This method is called when the monster update event occurs.
		 * When this event is received one or more of the listeners should be responsible for updating any client elements related to the entity's new state.
		 */
		virtual void onMonsterUpdate(Enigma::s32 monsterId,const Enigma::Monster& monster)=0;

		/*
		 * This method is called when the item update event occurs.
		 * When this event is received one or more of the listeners should be responsible for updating any client elements related to the entity's new state.
		 */
		virtual void onItemUpdate(Enigma::s32 itemId,const Enigma::Item& item)=0;


		/*
		 * This method is called when the player move event occurs.
		 * When this event is received one or more of the listeners should be responsible for updating the location of meshes and other client elements related to the entity.
		 * Note: More than likely in addition to the graphics engine the audio engine will want this information for 3D sound so the "listener" position can be updated.
		 */
		virtual void onPlayerMove(Enigma::s32 characterId,const Enigma::Character& character)=0;
    };
};

#endif // ISCENEEVENTLISTENER_HPP_INCLUDED