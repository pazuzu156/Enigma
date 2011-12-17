#ifndef SERVERSESSIONMANAGER_HPP_INCLUDED
#define SERVERSESSIONMANAGER_HPP_INCLUDED

/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "EventManager.hpp"
#include "Entities\Object.hpp"
#include "IServerTransmissionManager.hpp"
#include "Entities\Realm.hpp"
#include "Messages\RequestMessages.hpp"
#include "Messages\ResponseMessages.hpp"
#include "DataAccess.hpp"

namespace Enigma
{
	class DllExport ServerSessionManager : public EventManager
    {
        private:
 
		IServerTransmissionManager* mServerTransmissionManager;
		Realm mRealm;
		DataAccess mDataAccess;

        protected:

        //Send a packet to a single peer.
		void SendMessageToPeer(const std::string& peerId,MessageContainer& message);

        //Send a packet to all the peers on a single map.
        void SendMessageToMap(const std::string& peerId,MessageContainer& message);

        //Send a packet to all the peers in a party.
        void SendMessageToParty(const std::string& peerId,MessageContainer& message);

        //Send a packet to all the peers in a guild.
        void SendMessageToGuild(const std::string& peerId,MessageContainer& message);

        //Send a packet to all connected peers. (Include those not logged in.)
        void SendMessageToWorld(const std::string& peerId,MessageContainer& message);

		void EnterMap(const std::string& peerId);
		void EnterMap(const std::string& peerId,Character character);
		void ExitMap(const std::string& peerId);
		bool ChangeMap(const std::string& peerId,size_t newMapId);

        void SendCharactersOnMapToPlayer(const std::string& peerId);
        void SendNpcsOnMapToPlayer(const std::string& peerId);
        void SendItemsOnMapToPlayer(const std::string& peerId);
        void SendMonstersOnMapToPlayer(const std::string& peerId);

		void SendServerMessageToPeer(const std::string& peerId,const std::string& message);
		void SendServerMessageToParty(const std::string& peerId,const std::string& message);
		void SendServerMessageToGuild(const std::string& peerId,const std::string& message);
		void SendServerMessageToMap(const std::string& peerId,const std::string& message);
		void SendServerMessageToWorld(const std::string& peerId,const std::string& message);

		std::string GetUserByCharacterName(const std::string& name);
		std::string GetUserByCharacterId(size_t characterId);

		/*
		 * Takes in a peerId and returns a reference to that users current character.
		 * Note that if this user has not selected a character then this will result in an exception.
		 */
		Character& GetCurrentCharacter(const std::string& peerId);

		Character& GetCharacter(size_t mapId, size_t index);
		void CreateCharacter(size_t mapId, size_t index);
		void DeleteCharacter(size_t mapId, size_t index);

		Monster& GetMonster(size_t mapId, size_t index);
		void CreateMonster(size_t mapId, size_t index);
		void DeleteMonster(size_t mapId, size_t index);

		Npc& GetNpc(size_t mapId, const std::string& index);
		Npc& GetNpc(size_t mapId, size_t index);

		void CreateNpc(size_t mapId, const std::string& index);
		void CreateNpc(size_t mapId, size_t index);

		void DeleteNpc(size_t mapId, const std::string& index);
		void DeleteNpc(size_t mapId, size_t index);

		Item& GetItem(size_t mapId, size_t index);
		void CreateItem(size_t mapId, size_t index);
		void DeleteItem(size_t mapId, size_t index);

		/*
		 * Takes in a peerId and returns a reference to that users current character's guild.
		 * Note that if this user has not selected a character or is not in a guild this will result in an exception.
		 */
		Guild& GetCurrentGuild(const std::string& peerId);

		/*
		 * Takes in a peerId and returns a reference to that user's current character's party.
		 * Note that if this user has not selected a character or is not in a party this will result in an exception.
		 */
		Party& GetCurrentParty(const std::string& peerId);

		/*
		 * Takes in a peerId and returns a reference to that users current character's guild rank.
		 * Note that if this user has not selected a character or is not in a guild this will result in an exception.
		 */
		Rank& GetCurrentRank(const std::string& peerId);

		/*
		 * Takes in a peerId and returns a boolean that indicates whether or not the current user's character is the leader of their guild.
		 * Note that if this user has not selected a character or is not in a guild this will result in an exception.
		 */
		bool GetIsCurrentGuildLeader(const std::string& peerId);

		/*
		 * Takes in a peerId and returns a reference to that users current character's selected npc.
		 * Note that if this user has not selected a character or has not selected an npc this will result in an exception.
		 */
		Npc& GetCurrentNpc(const std::string& peerId);

		/*
		 * Takes in a peerId and returns a refernce to that user's current character's map.
		 * Note that if this user has not selected a characer and that character has entered a map this will result in an exception.
		 */
		Map& GetCurrentMap(const std::string& peerId);

		/*
		 * Takes in a peerId and returns a reference to that user's current character's current npc's current question.
		 * Note that if this user has not selected a character,entered a map, and selected an npc this will result in an exception.
		 */
		NpcQuestion& GetCurrentNpcQuestion(const std::string& peerId);

		/*
		 * Takes in a peerId and returns a reference to that user's stash.
		 * Note that if this user is not logged in this will result in an exception.
		 */
		std::map<size_t,Item>& GetStash(const std::string& peerId);

        void GivePlayerItem(const std::string& peerId,Item item);
        void GivePlayerItem(const std::string& peerId,Item item,size_t stackSize);

        void TakePlayerItem(const std::string& peerId,size_t locationId);
        void TakePlayerItem(const std::string& peerId,size_t locationId,size_t stackSize);

        void TransferPlayerItemToStash(const std::string& peerId,size_t locationId);
        void TransferPlayerItemToStash(const std::string& peerId,size_t locationId,size_t stackSize);

        void TransferPlayerItemFromStash(const std::string& peerId,size_t locationId);
        void TransferPlayerItemFromStash(const std::string& peerId,size_t locationId,size_t stackSize);

        void TransferPlayerItemToPlayer(const std::string& peerId,size_t locationId,size_t characterId);
        void TransferPlayerItemToPlayer(const std::string& peerId,size_t locationId,size_t characterId,size_t stackSize);

        size_t FindNextAvailableSlot(std::map<size_t,Item> itemCollection);
        size_t FindSlotWithItem(std::map<size_t,Item> itemCollection,size_t itemId);

        public:

        ServerSessionManager();
        ~ServerSessionManager();

		void PreInit();
        void Init(IServerTransmissionManager* serverTransmissionManager);
        void Load();
        void Unload();

		/*
		 * Used by the transmission manager to inform the session manager that a user has been disconnected and should be removed from the world.
		 * if the peerId is invalid it may result in an exception.
		 */
		void DisconnectUser(const std::string& peerId);

		/*
		 * Fires all application events for all application listeners from the current thread.
		 * Although not limited by the design it is intended that this method be called from one thread.
		 */
		void DoApplicationEvents();

		/*
		 * Fires all scene events for all scene listeners from the current thread.
		 * Although not limited by the design it is intended that this method be called from one thread.
		 */
		void DoSceneEvents();

		/*
		 * Fires all chat events for all chat listeners from the current thread.
		 * Although not limited by the design it is intended that this method be called from one thread.
		 */
		void DoChatEvents();

		/*
		 * Fires all audio events for all audio listeners from the current thread.
		 * Although not limited by the design it is intended that this method be called from one thread.
		 */
		void DoAudioEvents();

//Authentication Start
		//Process a client message related to authentication.
		std::string ProcessMessage(LoginRequestMessage& message,const std::string& peerId);

		//Process a client message related to authentication.
        std::string ProcessMessage(CharacterCreationRequestMessage& message,const std::string& peerId);

		//Process a client message related to authentication.
        std::string ProcessMessage(CharacterListRequestMessage& message,const std::string& peerId);

		//Process a client message related to authentication.
        std::string ProcessMessage(CharacterSelectionRequestMessage& message,const std::string& peerId);
//Authentication End

//Movement Start
        //Process a client message related to movement.
        std::string ProcessMessage(MovementRequestMessage& message,const std::string& peerId);

        //Process a client message related to player state change.
        std::string ProcessMessage(PlayerOnMapUpdateRequestMessage& message,const std::string& peerId);

        //Process a client message related to player state change.
        std::string ProcessMessage(ChangeMapRequestMessage& message,const std::string& peerId);

        //Process a client message related to npc state change.
        std::string ProcessMessage(NpcOnMapUpdateRequestMessage& message,const std::string& peerId);

        //Process a client message related to monster state change.
        std::string ProcessMessage(MonsterOnMapUpdateRequestMessage& message,const std::string& peerId);

        //Process a client message related to item state change.
        std::string ProcessMessage(ItemOnMapUpdateRequestMessage& message,const std::string& peerId);
//Movement End

//Chat Start
        //Process a client message related to chat.
        std::string ProcessMessage(ChatRequestMessage& message,const std::string& peerId);

        //Process a client message related to inviting.
        std::string ProcessMessage(InviteRequestMessage& message,const std::string& peerId);

        //Process a client message related to kicking.
        std::string ProcessMessage(KickRequestMessage& message,const std::string& peerId);

        //Process a client message related to player list.
        std::string ProcessMessage(PlayerListRequestMessage& message,const std::string& peerId);

        //Process a client message related to ?.
        std::string ProcessMessage(CreatePlayerOrganizationRequestMessage& message,const std::string& peerId);

        //Process a client message related to ?.
        std::string ProcessMessage(ModifyPlayerOrganizationRankRequestMessage& message,const std::string& peerId);

        //Process a client message related to ?.
        std::string ProcessMessage(SetPlayerRankRequestMessage& message,const std::string& peerId);

        //Process a client message related to ?.
        std::string ProcessMessage(RollRequestMessage& message,const std::string& peerId);

        //Process a client message related to ?.
        std::string ProcessMessage(ServerTimeRequestMessage& message,const std::string& peerId);

        //Process a client message related to ?.
        std::string ProcessMessage(NpcChatRequestMessage& message,const std::string& peerId);
//Chat End

//Voice Start
        //Process a client message related to voice chat.
        std::string ProcessMessage(VoiceChatRequestMessage& message,const std::string& peerId);
//Voice End

//Combat Start
		//Process a client message related to combat
        std::string ProcessMessage(ItemTransferRequestMessage& message,const std::string& peerId);

		//Process a client message related to combat
		std::string ProcessMessage(SkillRequestMessage& message,const std::string& peerId);
//Combat End
    };
};

#endif // SERVERSESSIONMANAGER_HPP_INCLUDED