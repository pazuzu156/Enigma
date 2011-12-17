#ifndef CLIENTSESSIONMANAGER_HPP_INCLUDED
#define CLIENTSESSIONMANAGER_HPP_INCLUDED

/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "EventManager.hpp"
#include "IClientTransmissionManager.hpp"
#include "Entities\RealmSnapshot.hpp"
#include "ClientConfigurationManager.hpp"
#include "Messages\RequestMessages.hpp"
#include "Messages\ResponseMessages.hpp"
#include "TargetPreferenceTypes.hpp"

#include "std_vector.hpp"
#include <boost/thread.hpp>

#include "IApplicationEventListener.hpp"

namespace Enigma
{
	class DllExport ClientSessionManager : public EventManager
    {
        private:
 
		IClientTransmissionManager* mClientTransmissionManager;
		RealmSnapshot mRealmSnapshot;

		std::vector< std::string > mAvailableCharacters;
		ClientConfigurationManager mClientConfigurationManager;

		boost::barrier mThreadBarrier;

        protected:

		/*
		 * Returns the character currently in use by the user.
		 * If the user has yet to select a character calling this method will result in an exception.
		 * Callers should make sure user "is in the world" before calling this method.
		 */
		Character& GetCurrentCharacter();

		Character& GetCharacter(size_t index);
		void CreateCharacter(size_t index);
		void DeleteCharacter(size_t index);

		Monster& GetMonster(size_t index);
		void CreateMonster(size_t index);
		void DeleteMonster(size_t index);

		Npc& GetNpc(const std::string& index);
		Npc& GetNpc(size_t index);

		void CreateNpc(const std::string& index);
		void CreateNpc(size_t index);

		void DeleteNpc(const std::string& index);
		void DeleteNpc(size_t index);

		Item& GetItem(size_t index);
		void CreateItem(size_t index);
		void DeleteItem(size_t index);

		bool ProcessChatCommand(const std::string& text,Enigma::s32& commandDivider,Enigma::s32& targetDivider,std::string& command,std::string& target,std::string& message);
		bool ProcessPlayerOrganizationCommand(const std::string& text,Enigma::s32& commandDivider,Enigma::s32& targetDivider,std::string& command,std::string& target,std::string& message);
		bool ProcessInformationCommand(const std::string& text,Enigma::s32& commandDivider,Enigma::s32& targetDivider,std::string& command,std::string& target,std::string& message);
		bool ProcessWhimsicalCommand(const std::string& text,Enigma::s32& commandDivider,Enigma::s32& targetDivider,std::string& command,std::string& target,std::string& message);
        bool ProcessCombatCommand(const std::string& text,Enigma::s32& commandDivider,Enigma::s32& targetDivider,std::string& command,std::string& target,std::string& message);
		bool ProcessStateChangeCommand(const std::string& text,Enigma::s32& commandDivider,Enigma::s32& targetDivider,std::string& command,std::string& target,std::string& message);

		public:

        ClientSessionManager();
        ~ClientSessionManager();

		/*
		 * This method allows other components wait for the next iteration of the main loop to save CPU cycles.
		 * This method depends on boost::barrier.
		 */
		void Wait();

		/*
		 * Assigns a pointer to the active client transmission manager for sending messages and reconnecting.
		 * 
		 */
		void SetClientTransmissionManager(IClientTransmissionManager* clientTransmissionManager){mClientTransmissionManager=clientTransmissionManager;}

		/*
		 * Returns a reference to the configuration manager which contains all variable data not stored in the database.
		 * Resolution, server connection information, and asset locations are examples of information provided by the configuration manager.
		 */
		ClientConfigurationManager& GetClientConfigurationManager(){return mClientConfigurationManager;}

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

		/*
		 * Makes sure user meets given requirements.
		 * Used to test to see if user has logged in or has entered world before executing a command.
		 */
		bool Validate(bool testIsLoggedIn,bool testHasEnteredWorld);

		/*
		 * Analyizes textual user input and generates appropriate events.
		 * This method will directly call the transmission layer and as such must wait for transmission availability.
		 */
		void ProcessCommand(const std::string& text,size_t chatType=CHAT_TYPE_MAP);

		/*
		 * Returns a const reference to the realm snap shot which contains a local copy of of portions of the world relevant to the current user.
		 * If the client is multi-threaded a reader lock should be obtained on the elements being read to prevent data corruption.
		 */
		const RealmSnapshot& GetRealmSnapshot() const {return this->mRealmSnapshot;}

		/*
		 * After a successful character list request calling this method will return a list of valid characters.
		 * It will not cause an exception to call this method before a request is made but the list will have zero elements.
		 */
		const std::vector< std::string >& GetAvailableCharacters() const {return this->mAvailableCharacters;}

		/*
		 * Disconnects from the server and then reconnects thus destroying previous session. Then fires change gamemode event.
		 * The Reconnect method called by this event locks the host object so transmissions will not be processed while this is executing.
		 */
		void Logout();

		/*
		 * This event fires when the client is disconnected from the server.
		 * This can be caused by the logout method being called or a timeout.
		 */
		void onDisconnected();

		/*
		 * Requests the server to authenticate the current user.
		 * failure to authenticate user will result in a security error status.
		 * Behavior for other failure types is undefined.
		 */
		void Login(const std::string& username,const std::string& password);

		/*
		 * Request to select an available character from the available character list.
		 * Calling this method before requesting available characters or otherwise selecting an invalid index will result in an exception.
		 */
		void SelectCharacter(int index);

		/*
		 * Request to change the current characters current map.
		 * Calling this method with an invalid map index may result in an exception.
		 */
		void RequestMapChange(size_t index);

		/*
		 * Request that the server provide a list of valid characters for the current user.
		 * If the user is not logged in the behavior is undefined.
		 * If successful the available character list will be populated.
		 */
		void RequestCharacterList(bool shouldLock=true);

		/*
		 * Request the creation of a new character for the player to use.
		 *
		 */
		void RequestCreateCharacter(const std::string& name);

		/*
		 * Request to use a skill on the current target.
		 * Target it pulled from realm snapshot.
		 */
		void RequestUseSkill(Enigma::s32 skillId,Enigma::s32 skillLevel);

		/*
		 * Requests the server to move the currently selected character to the given location.
		 * Rotation is included but as of 8/23/2011 is otherwise not implemented.
		 */
		void MovePlayer(Enigma::f32 x1,Enigma::f32 y1, Enigma::f32 z1,Enigma::f32 x2,Enigma::f32 y2, Enigma::f32 z2);

		/*
		 * Requests the server to move the currently selected character to the given location.
		 * Rotation is assumed to be nothing.
		 */
		void MovePlayer(Enigma::f32 x1,Enigma::f32 y1, Enigma::f32 z1);

		/*
		 * Requests a list of all players on the map from the server.
		 */
		void RequestPlayersOnMap();

		/*
		 * Requests a list of all npcs on the map from the server.
		 */
		void RequestNpcsOnMap();

		/*
		 * Requests a list of all monsters on the map from the server.
		 */
		void RequestMonstersOnMap();

		/*
		 * Requests a list of all items on the map from the server.
		 */
		void RequestItemsOnMap();

		/*
		 * Accept or reject another players invitation to a party or guild.
		 */
		void RespondToInvite(bool accepted,size_t inviteId,size_t inviteType);

		/*
		 * Accept or reject another players invitation to a guild.
		 */
		void RespondToGuildInvite(bool accepted,size_t inviteId);

		/*
		 * Accept or reject another players invitation to a party.
		 */
		void RespondToPartyInvite(bool accepted,size_t inviteId);

		/*
		 * Sends collected voice data to the server with correct channel information.
		 * Should result in all user users in the same channel or area getting this voice data.
		 */
		void SendVoiceData(Enigma::u8* inputBuffer,size_t inputSize);

//Authentication Start
		//Process a server message related to login.
        void ProcessMessage(LoginResponseMessage& message);

		//Process a server message related to the creation of a character.
		/*
		 * Processes the character creation response from the server and where applicable updates character selection status.
		 */
        void ProcessMessage(CharacterCreationResponseMessage& message);

		/*
		 * Processes the character list response from the server and where applicable populates the available character list.
		 * If there are no characters available the list will contain zero elements.
		 * Behavior for failure is not defined.
		 */
        void ProcessMessage(CharacterListResponseMessage& message);

		//Process a server message related to selecting a character.
        void ProcessMessage(CharacterSelectionResponseMessage& message);

//Authentication End

//Movement Start
		/*
		 * Adds, removes, or modifies players on the map.
		 * Modifications to entities will only occur if the entity is not being removed.
		 */
        void ProcessMessage(PlayerOnMapUpdateResponseMessage& message);
		
		/*
		 * Adds, removes, or modifies npcs on the map.
		 * Modifications to entities will only occur if the entity is not being removed.
		 */
        void ProcessMessage(NpcOnMapUpdateResponseMessage& message);
		
		/*
		 * Adds, removes, or modifies monsters on the map.
		 * Modifications to entities will only occur if the entity is not being removed.
		 */
        void ProcessMessage(MonsterOnMapUpdateResponseMessage& message);

		/*
		 * Adds, removes, or modifies items on the map.
		 * Modifications to entities will only occur if the entity is not being removed.
		 */
        void ProcessMessage(ItemOnMapUpdateResponseMessage& message);

		//Process a server message related to items on map.
        void ProcessMessage(MovementResponseMessage& message);

		/*
		 * Processes the response to the clients request to change map.
		 * Failure behavior is not defined.
		 */
        void ProcessMessage(ChangeMapResponseMessage& message);
//Movement End

//Chat Start
		//Process a server message related to chat.
        void ProcessMessage(ChatResponseMessage& message);

		//Process a server message related to an invitation request.
        void ProcessMessage(KickResponseMessage& message);

		//Process a server message related to invitation response.
        void ProcessMessage(InviteResponseMessage& message);

		//Process a server message related to player list.
        void ProcessMessage(PlayerListResponseMessage& message);

		//Process a server message related to creating a player organization.
        void ProcessMessage(CreatePlayerOrganizationResponseMessage& message);

		//Process a server message related to ?.
        void ProcessMessage(ModifyPlayerOrganizationRankResponseMessage& message);

		//Process a server message related to player guild rank.
        void ProcessMessage(SetPlayerRankResponseMessage& message);

		//Process a server message related to npc chat.
        void ProcessMessage(NpcChatResponseMessage& message);
//Chat End

//Voice Start
		//Process a server message related to voice chat.
		void ProcessMessage(VoiceChatResponseMessage& message);
//Voice End

//Combat Start
		//Process a server message related to skill usage.
        void ProcessMessage(SkillResponseMessage& message);

		//Process a server message related to item transfer.
		void ProcessMessage(ItemTransferResponseMessage& message);
//Combat End

    };
};

#endif // CLIENTSESSIONMANAGER_HPP_INCLUDED