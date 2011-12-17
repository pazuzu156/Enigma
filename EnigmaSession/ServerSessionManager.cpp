/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "ServerSessionManager.hpp"
#include "EnigmaException.hpp"

#include "boost_foreach.hpp"

namespace Enigma
{
    ServerSessionManager::ServerSessionManager()
    {
		this->PreInit();
    }

    ServerSessionManager::~ServerSessionManager()
    {
		this->Unload();
    }

	void ServerSessionManager::PreInit()
	{
		this->mServerTransmissionManager = NULL;
	}

    void ServerSessionManager::Init(IServerTransmissionManager* serverTransmissionManager)
	{
		this->mServerTransmissionManager = serverTransmissionManager;
	}

    void ServerSessionManager::Load()
	{
		this->mDataAccess.Start();
	}

    void ServerSessionManager::Unload()
	{
		this->mDataAccess.Stop();
	}

	void ServerSessionManager::DisconnectUser(const std::string& peerId)
	{
		boost::unique_lock< boost::shared_mutex > lock1(this->mRealm.SceneMutex);
		boost::unique_lock< boost::shared_mutex > lock2(this->mRealm.ApplicationMutex);

		this->ExitMap(peerId); //remove character from map.
		this->mRealm.GetUsers().erase(peerId); //remove user from world.
	}

	void ServerSessionManager::DoApplicationEvents()
	{
		boost::shared_lock< boost::shared_mutex > lock(this->mRealm.ApplicationMutex);
		bool isValid=false;
		EnigmaEvent enigmaEvent;

		enigmaEvent = this->PopApplicationEvent(isValid);
		while(isValid)
		{
			switch(enigmaEvent.EventType)
			{
				case enigmaEvent.EVENT_APPLICATION_EXIT:
						this->RaiseApplicationEventExit();
					break;
				case enigmaEvent.EVENT_APPLICATION_GAME_MODE_CHANGED:
						this->RaiseApplicationEventChangeGameMode(enigmaEvent.Index);
					break;
				case enigmaEvent.EVENT_APPLICATION_LOG:
						this->RaiseApplicationEventLog(enigmaEvent.Text);
					break;
				case enigmaEvent.EVENT_APPLICATION_OPEN_BROWSER_WINDOW:
						this->RaiseApplicationEventOpenBrowserWindow(enigmaEvent.Text);
					break;
				case enigmaEvent.EVENT_APPLICATION_OPEN_VIDEO_WINDOW:
						this->RaiseApplicationEventOpenVideoWindow(enigmaEvent.Text);
					break;
				default:
					//this is important to catch events going into the wrong queues.
					std::cout << "Application event type: " << enigmaEvent.EventType << " skipped because there was no case to handle it." << std::endl;
					std::cout << "\t text: " << enigmaEvent.Text << std::endl;
					std::cout << "\t sender: " << enigmaEvent.Sender << std::endl;
					break;
			}
			enigmaEvent = this->PopApplicationEvent(isValid);
		}
	}

	void ServerSessionManager::DoSceneEvents()
	{
		boost::shared_lock< boost::shared_mutex > lock(this->mRealm.SceneMutex);
		bool isValid=false;
		EnigmaEvent enigmaEvent;

		enigmaEvent = this->PopSceneEvent(isValid);
		while(isValid)
		{
			switch(enigmaEvent.EventType)
			{
				case enigmaEvent.EVENT_SCENE_CHARACTER_ENTERED_MAP:
						this->RaiseSceneEventPlayerEnteredMap(enigmaEvent.EntityId,this->GetCharacter(enigmaEvent.MapId,enigmaEvent.EntityId));
					break;
				case enigmaEvent.EVENT_SCENE_CHARACTER_EXITED_MAP:
						this->RaiseSceneEventPlayerExitedMap(enigmaEvent.EntityId,this->GetCharacter(enigmaEvent.MapId,enigmaEvent.EntityId));
					break;
				case enigmaEvent.EVENT_SCENE_CHARACTER_UPDATED:
						this->RaiseSceneEventPlayerUpdate(enigmaEvent.EntityId,this->GetCharacter(enigmaEvent.MapId,enigmaEvent.EntityId));
					break;
				case enigmaEvent.EVENT_SCENE_PLAYER_MOVED:
						this->RaiseSceneEventPlayerMove(enigmaEvent.EntityId,this->GetCharacter(enigmaEvent.MapId,enigmaEvent.EntityId));
					break;
				case enigmaEvent.EVENT_SCENE_ITEM_ENTERED_MAP:
						this->RaiseSceneEventItemEnteredMap(enigmaEvent.EntityId,this->GetItem(enigmaEvent.MapId,enigmaEvent.EntityId));
					break;
				case enigmaEvent.EVENT_SCENE_ITEM_EXITED_MAP:
						this->RaiseSceneEventItemExitedMap(enigmaEvent.EntityId,this->GetItem(enigmaEvent.MapId,enigmaEvent.EntityId));
					break;
				case enigmaEvent.EVENT_SCENE_ITEM_UPDATED:
						this->RaiseSceneEventItemUpdate(enigmaEvent.EntityId,this->GetItem(enigmaEvent.MapId,enigmaEvent.EntityId));
					break;
				case enigmaEvent.EVENT_SCENE_MONSTER_ENTERED_MAP:
						this->RaiseSceneEventMonsterEnteredMap(enigmaEvent.EntityId,this->GetMonster(enigmaEvent.MapId,enigmaEvent.EntityId));
					break;
				case enigmaEvent.EVENT_SCENE_MONSTER_EXITED_MAP:
						this->RaiseSceneEventMonsterExitedMap(enigmaEvent.EntityId,this->GetMonster(enigmaEvent.MapId,enigmaEvent.EntityId));
					break;
				case enigmaEvent.EVENT_SCENE_MONSTER_UPDATED:
						this->RaiseSceneEventMonsterUpdate(enigmaEvent.EntityId,this->GetMonster(enigmaEvent.MapId,enigmaEvent.EntityId));
					break;
				case enigmaEvent.EVENT_SCENE_NPC_ENTERED_MAP:
						this->RaiseSceneEventNpcEnteredMap(enigmaEvent.EntityId,this->GetNpc(enigmaEvent.MapId,enigmaEvent.EntityId));
					break;
				case enigmaEvent.EVENT_SCENE_NPC_EXITED_MAP:
						this->RaiseSceneEventNpcExitedMap(enigmaEvent.EntityId,this->GetNpc(enigmaEvent.MapId,enigmaEvent.EntityId));
					break;
				case enigmaEvent.EVENT_SCENE_NPC_UPDATED:
						this->RaiseSceneEventNpcUpdate(enigmaEvent.EntityId,this->GetNpc(enigmaEvent.MapId,enigmaEvent.EntityId));
					break;
				default:
					//this is important to catch events going into the wrong queues.
					std::cout << "Scene event type: " << enigmaEvent.EventType << " skipped because there was no case to handle it." << std::endl;
					std::cout << "\t text: " << enigmaEvent.Text << std::endl;
					std::cout << "\t sender: " << enigmaEvent.Sender << std::endl;
					break;
			}
			enigmaEvent = this->PopSceneEvent(isValid);
		}
	}

	void ServerSessionManager::DoChatEvents()
	{
		boost::shared_lock< boost::shared_mutex > lock(this->mRealm.ChatMutex);
		bool isValid=false;
		EnigmaEvent enigmaEvent;

		enigmaEvent = this->PopChatEvent(isValid);
		while(isValid)
		{
			switch(enigmaEvent.EventType)
			{
				case enigmaEvent.EVENT_CHAT_RANK_MODIFIED:
						this->RaiseChatEventRankModified(enigmaEvent.ScopeId,enigmaEvent.Text,enigmaEvent.Index);
					break;
				case enigmaEvent.EVENT_CHAT_MODIFIED:
						this->RaiseChatEventModified(enigmaEvent.ScopeId,enigmaEvent.Sender,enigmaEvent.Text);
					break;
				case enigmaEvent.EVENT_CHAT_EXPELLED:
						this->RaiseChatEventExpelled(enigmaEvent.ScopeId,enigmaEvent.Sender,enigmaEvent.Text);
					break;
				case enigmaEvent.EVENT_CHAT_INVITED:
						this->RaiseChatEventInvited(enigmaEvent.ScopeId,enigmaEvent.EntityId,enigmaEvent.Sender);
					break;
				case enigmaEvent.EVENT_CHAT_JOINED:
						this->RaiseChatEventJoined(enigmaEvent.ScopeId,enigmaEvent.Sender);
					break;
				case enigmaEvent.EVENT_CHAT_RECEIVED_MESSAGE:
						this->RaiseChatEventReceivedMessage(enigmaEvent.ScopeId,enigmaEvent.Text,enigmaEvent.Sender);
					break;
				case enigmaEvent.EVENT_CHAT_NPC_RESPONSE:
						this->RaiseChatEventNpcResponse(enigmaEvent.EntityId,enigmaEvent.Text,enigmaEvent.Values);
					break;
				default:
					//this is important to catch events going into the wrong queues.
					std::cout << "Chat event type: " << enigmaEvent.EventType << " skipped because there was no case to handle it." << std::endl;
					std::cout << "\t text: " << enigmaEvent.Text << std::endl;
					std::cout << "\t sender: " << enigmaEvent.Sender << std::endl;
					break;
			}
			enigmaEvent = this->PopChatEvent(isValid);
		}
	}

	void ServerSessionManager::DoAudioEvents()
	{
		boost::shared_lock< boost::shared_mutex > lock(this->mRealm.ApplicationMutex);

		bool isValid=false;
		EnigmaEvent enigmaEvent;

		enigmaEvent = this->PopAudioEvent(isValid);
		while(isValid)
		{
			switch(enigmaEvent.EventType)
			{
				case enigmaEvent.EVENT_AUDIO_VOICE_RECEIVED:
					this->RaiseAudioEventVoiceReceived(enigmaEvent.EntityId,this->GetCharacter(enigmaEvent.MapId,enigmaEvent.EntityId),(Enigma::u8*)enigmaEvent.UserData,enigmaEvent.Index);
					break;
				default:
					//this is important to catch events going into the wrong queues.
					std::cout << "Audio event type: " << enigmaEvent.EventType << " skipped because there was no case to handle it." << std::endl;
					std::cout << "\t text: " << enigmaEvent.Text << std::endl;
					std::cout << "\t sender: " << enigmaEvent.Sender << std::endl;
					break;
			}
			enigmaEvent = this->PopAudioEvent(isValid);
		}
	}

	//Authentication Start
	std::string ServerSessionManager::ProcessMessage(LoginRequestMessage& message,const std::string& peerId)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mRealm.ApplicationMutex);

        LoginResponseMessage loginResponseMessage;
		loginResponseMessage.SetStatus(STATUS_UNKNOWN_FAIL);
		bool isLoggedIn=false;
		std::string result="";

        try
        {	
			//isLoggedIn = true;
			size_t userId=0;
			isLoggedIn = this->mDataAccess.Authenticate(message.GetUsername(),message.GetPassword(),userId);
            if(isLoggedIn)
			{	
				result = message.GetUsername();

				std::stringstream s;
				s << result << " has signed in.";
				this->Log(s);

				User user(result);
				user.SetId(userId);

                loginResponseMessage.SetStatus(STATUS_OK);	

				if(this->mRealm.GetUsers().find(result)==this->mRealm.GetUsers().end())
				{			
					this->mRealm.GetUsers().insert(std::pair<std::string,User>(result,user));
				}
				else
				{
					this->mRealm.GetUsers()[result] = user;
				}

				this->mRealm.GetUsers()[result].SetIsLoggedIn(true);
            }
            else
            {
				std::stringstream s;
				s << message.GetUsername() << " has failed to sign in.";
				this->Log(s);
                loginResponseMessage.SetStatus(STATUS_SECURITY_FAIL);

				result = peerId;
            }

			this->mServerTransmissionManager->SendMessageToPeer(result,loginResponseMessage);
        }
		catch(Enigma::EnigmaException& e)
		{
			std::cout << "Enigma Exception: " << e.what() << std::endl;
		}
		catch (Enigma::HardwareException&)
		{
			throw;
		}
		catch (std::overflow_error&)
		{
			throw; //may need to pop some calls off the stack.
		}
		catch (std::exception& e)
		{
			std::cout << "Exception: " << e.what() << std::endl;
		}

		return result;
	}

    std::string ServerSessionManager::ProcessMessage(CharacterCreationRequestMessage& message,const std::string& peerId)
	{
		boost::unique_lock< boost::shared_mutex > lock1(this->mRealm.ApplicationMutex);
		boost::unique_lock< boost::shared_mutex > lock2(this->mRealm.SceneMutex);

		std::string result=peerId;

		if(peerId.empty())
		{ 
				std::stringstream s;
				s << "Unauthenticated users cannot perform this action.";
				this->Log(s);
			return peerId;
		}
		else
		{
			if(this->mRealm.GetUsers().find(peerId)==this->mRealm.GetUsers().end())
			{
				if(!this->mRealm.GetUsers()[peerId].GetIsLoggedIn())
				{
					std::stringstream s;
					s << "A user claiming to be " << peerId << " attempted to perform an operation that requires authentication.";
					this->Log(s);
					return peerId;
				}
				if(this->mRealm.GetUsers()[peerId].GetHasEnteredWorld())
				{
					std::stringstream s;
					s << peerId << " attempted to perform an operation that requires no character in the world.";
					this->Log(s);
					return peerId;
				}
			}
		}

		try
        {
			CharacterCreationResponseMessage characterCreationResponseMessage;
			
			Character character;

			character.SetName(message.GetName());
			this->mRealm.GetUsers()[peerId].SetCurrentMapId(0); //TODO: remove fake starting map
			character.SetUserId(this->mRealm.GetUsers()[peerId].GetId());

			this->mDataAccess.CreateCharacter<Character>(character);
			if(character.GetId()>0)
			{			
				this->mRealm.GetUsers()[peerId].SetCurrentCharacterId(character.GetId());

				std::stringstream s;
				s << "Character named " << character.GetName() << " has been created." << std::endl;
				this->Log(s);

				this->SendMessageToPeer(peerId,characterCreationResponseMessage);
				this->EnterMap(peerId,character);
			}
			else
			{
				characterCreationResponseMessage.SetStatus(STATUS_DATABASE_FAIL);
				std::stringstream s;
				s << "Character named " << character.GetName() << " was not created. It may already exist in the database." << std::endl;
				this->Log(s);

				this->SendMessageToPeer(peerId,characterCreationResponseMessage);
			}
		}
		catch(Enigma::EnigmaException& e)
		{
			std::cout << "Enigma Exception: " << e.what() << std::endl;
		}
		catch (Enigma::HardwareException&)
		{
			throw;
		}
		catch (std::overflow_error&)
		{
			throw; //may need to pop some calls off the stack.
		}
		catch (std::exception& e)
		{
			std::cout << "Exception: " << e.what() << std::endl;
		}

		return result;
	}

    std::string ServerSessionManager::ProcessMessage(CharacterListRequestMessage& message,const std::string& peerId)
	{
		boost::unique_lock< boost::shared_mutex > lock1(this->mRealm.ApplicationMutex);
		boost::unique_lock< boost::shared_mutex > lock2(this->mRealm.SceneMutex);

		std::string result=peerId;

		if(peerId.empty())
		{ 
			std::stringstream s;
			s << "Unauthenticated users cannot perform this action.";
			this->Log(s);
			return peerId;
		}
		else
		{
			if(this->mRealm.GetUsers().find(peerId)==this->mRealm.GetUsers().end())
			{
				if(!this->mRealm.GetUsers()[peerId].GetIsLoggedIn())
				{
					std::stringstream s;
					s << "A user claiming to be " << peerId << " attempted to perform an operation that requires authentication.";
					this->Log(s);
					return peerId;
				}
			}
		}

		try
        {
			std::cout << "Character list requested by user " << peerId << std::endl;
            //notify the other players that the user has camped.
			this->ExitMap(peerId);

            //reset combatant index so server knows no Character is currently selected.
			this->mRealm.GetUsers()[peerId].SetCurrentCharacterId(0);
			this->mRealm.GetUsers()[peerId].SetCurrentMapId(0);

			this->mRealm.GetUsers()[peerId].GetAvailableCharacters().clear();
			this->mDataAccess.LoadCharactersByUserId(this->mRealm.GetUsers()[peerId].GetAvailableCharacters(),this->mRealm.GetUsers()[peerId].GetId());

			BOOST_FOREACH(Enigma::Character character,this->mRealm.GetUsers()[peerId].GetAvailableCharacters())
			{
				CharacterListResponseMessage characterListResponseMessage;
				characterListResponseMessage.SetName(character.GetName());
				this->SendMessageToPeer(peerId,characterListResponseMessage);
			}

			//send empty entry so client knows there are no more.
			CharacterListResponseMessage characterListResponseMessage;
			characterListResponseMessage.SetName(std::string(""));
			this->SendMessageToPeer(peerId,characterListResponseMessage);
		}
		catch(Enigma::EnigmaException& e)
		{
			std::cout << "Enigma Exception: " << e.what() << std::endl;
		}
		catch (Enigma::HardwareException&)
		{
			throw;
		}
		catch (std::overflow_error&)
		{
			throw; //may need to pop some calls off the stack.
		}
		catch (std::exception& e)
		{
			std::cout << "Exception: " << e.what() << std::endl;
		}

		return result;
	}

    std::string ServerSessionManager::ProcessMessage(CharacterSelectionRequestMessage& message,const std::string& peerId)
	{
		boost::unique_lock< boost::shared_mutex > lock1(this->mRealm.ApplicationMutex);
		boost::unique_lock< boost::shared_mutex > lock2(this->mRealm.SceneMutex);

		std::string result=peerId;

		if(peerId.empty())
		{ 
				std::stringstream s;
				s << "Unauthenticated users cannot perform this action.";
				this->Log(s);
			return peerId;
		}
		else
		{
			if(this->mRealm.GetUsers().find(peerId)==this->mRealm.GetUsers().end())
			{
				if(!this->mRealm.GetUsers()[peerId].GetIsLoggedIn())
				{
					std::stringstream s;
					s << "A user claiming to be " << peerId << " attempted to perform an operation that requires authentication.";
					this->Log(s);
					return peerId;
				}
				if(this->mRealm.GetUsers()[peerId].GetHasEnteredWorld())
				{
					std::stringstream s;
					s << peerId << " attempted to perform an operation that requires no character in the world.";
					this->Log(s);
					return peerId;
				}
			}
		}

		try
        {					
			this->EnterMap(peerId,this->mRealm.GetUsers()[peerId].GetAvailableCharacters()[message.GetCharacterIndex()]);
			
			CharacterSelectionResponseMessage characterSelectionResponseMessage;
			this->SendMessageToPeer(peerId,characterSelectionResponseMessage);

			std::cout << "Character named " << this->GetCurrentCharacter(peerId).GetName() << " has been selected." << std::endl;

			//load characters guild if it hasn't been loaded. Not done on create because a newly created character won't have a guild.
			if(this->mRealm.GetGuilds().find(this->GetCurrentCharacter(peerId).GetGuildId())==this->mRealm.GetGuilds().end())
			{
				Guild guild;
				this->mDataAccess.LoadGuild<Guild>(guild,this->GetCurrentCharacter(peerId).GetGuildId());
				this->mRealm.GetGuilds()[guild.GetId()]=guild;
			}
		}
		catch(Enigma::EnigmaException& e)
		{
			std::cout << "Enigma Exception: " << e.what() << std::endl;
		}
		catch (Enigma::HardwareException&)
		{
			throw;
		}
		catch (std::overflow_error&)
		{
			throw; //may need to pop some calls off the stack.
		}
		catch (std::exception& e)
		{
			std::cout << "Exception: " << e.what() << std::endl;
		}

		return result;
	}
//Authentication End

//Movement Start
    std::string ServerSessionManager::ProcessMessage(MovementRequestMessage& message,const std::string& peerId)
	{
		boost::unique_lock< boost::shared_mutex > lock1(this->mRealm.SceneMutex);
		boost::shared_lock< boost::shared_mutex > lock2(this->mRealm.ApplicationMutex);

		std::string result=peerId;

		if(peerId.empty())
		{ 
				std::stringstream s;
				s << "Unauthenticated users cannot perform this action.";
				this->Log(s);
			return peerId;
		}
		else
		{
			if(this->mRealm.GetUsers().find(peerId)==this->mRealm.GetUsers().end())
			{
				if(!this->mRealm.GetUsers()[peerId].GetIsLoggedIn())
				{
					std::stringstream s;
					s << "A user claiming to be " << peerId << " attempted to perform an operation that requires authentication.";
					this->Log(s);
					return peerId;
				}
				if(!this->mRealm.GetUsers()[peerId].GetHasEnteredWorld())
				{
					std::stringstream s;
					s << peerId << " attempted to perform an operation that requires a character in the world.";
					this->Log(s);
					return peerId;
				}
			}
		}

		try
        {
            //TODO: validate stuff
            if(true)
            {
                MovementResponseMessage movementResponseMessage;

                //location
                movementResponseMessage.SetX1(message.GetX1());
                movementResponseMessage.SetY1(message.GetY1());
                movementResponseMessage.SetZ1(message.GetZ1());

				this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetCharacters()[this->mRealm.GetUsers()[peerId].GetCurrentCharacterId()].SetX(message.GetX1());
				this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetCharacters()[this->mRealm.GetUsers()[peerId].GetCurrentCharacterId()].SetY(message.GetY1());
				this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetCharacters()[this->mRealm.GetUsers()[peerId].GetCurrentCharacterId()].SetZ(message.GetZ1());

                //direction
                movementResponseMessage.SetX2(message.GetX2());
                movementResponseMessage.SetY2(message.GetY2());
                movementResponseMessage.SetZ2(message.GetZ2());

                movementResponseMessage.SetCombatantId(this->mRealm.GetUsers()[peerId].GetCurrentCharacterId());

                this->SendMessageToMap(peerId,movementResponseMessage);
            }
            else
            {
				//notify player of real position.

				//location
				MovementResponseMessage movementResponseMessage;
                movementResponseMessage.SetX1(this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetCharacters()[this->mRealm.GetUsers()[peerId].GetCurrentCharacterId()].GetX());
                movementResponseMessage.SetY1(this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetCharacters()[this->mRealm.GetUsers()[peerId].GetCurrentCharacterId()].GetY());
                movementResponseMessage.SetZ1(this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetCharacters()[this->mRealm.GetUsers()[peerId].GetCurrentCharacterId()].GetZ());

                //direction
                movementResponseMessage.SetX2(message.GetX2()); //don't track this so for now it is fine if they face where they thought they were.
                movementResponseMessage.SetY2(message.GetY2()); //don't track this so for now it is fine if they face where they thought they were.
                movementResponseMessage.SetZ2(message.GetZ2()); //don't track this so for now it is fine if they face where they thought they were.

				movementResponseMessage.SetCombatantId(this->mRealm.GetUsers()[peerId].GetCurrentCharacterId());

				this->SendMessageToPeer(peerId,movementResponseMessage);
            }
		}
		catch(Enigma::EnigmaException& e)
		{
			std::cout << "Enigma Exception: " << e.what() << std::endl;
		}
		catch (Enigma::HardwareException&)
		{
			throw;
		}
		catch (std::overflow_error&)
		{
			throw; //may need to pop some calls off the stack.
		}
		catch (std::exception& e)
		{
			std::cout << "Exception: " << e.what() << std::endl;
		}

		return result;
	}

    std::string ServerSessionManager::ProcessMessage(PlayerOnMapUpdateRequestMessage& message,const std::string& peerId)
	{
		boost::shared_lock< boost::shared_mutex > lock1(this->mRealm.SceneMutex);
		boost::shared_lock< boost::shared_mutex > lock2(this->mRealm.ApplicationMutex);

		std::string result=peerId;

		if(peerId.empty())
		{ 
				std::stringstream s;
				s << "Unauthenticated users cannot perform this action.";
				this->Log(s);
			return peerId;
		}
		else
		{
			if(this->mRealm.GetUsers().find(peerId)==this->mRealm.GetUsers().end())
			{
				if(!this->mRealm.GetUsers()[peerId].GetIsLoggedIn())
				{
					std::stringstream s;
					s << "A user claiming to be " << peerId << " attempted to perform an operation that requires authentication.";
					this->Log(s);
					return peerId;
				}
				if(!this->mRealm.GetUsers()[peerId].GetHasEnteredWorld())
				{
					std::stringstream s;
					s << peerId << " attempted to perform an operation that requires a character in the world.";
					this->Log(s);
					return peerId;
				}
			}
		}

		try
        {
			this->SendCharactersOnMapToPlayer(peerId);
		}
		catch(Enigma::EnigmaException& e)
		{
			std::cout << "Enigma Exception: " << e.what() << std::endl;
		}
		catch (Enigma::HardwareException&)
		{
			throw;
		}
		catch (std::overflow_error&)
		{
			throw; //may need to pop some calls off the stack.
		}
		catch (std::exception& e)
		{
			std::cout << "Exception: " << e.what() << std::endl;
		}

		return result;
	}

    std::string ServerSessionManager::ProcessMessage(ChangeMapRequestMessage& message,const std::string& peerId)
	{
		boost::unique_lock< boost::shared_mutex > lock1(this->mRealm.SceneMutex);
		boost::unique_lock< boost::shared_mutex > lock2(this->mRealm.ApplicationMutex);

		std::string result=peerId;

		if(peerId.empty())
		{ 
				std::stringstream s;
				s << "Unauthenticated users cannot perform this action.";
				this->Log(s);
			return peerId;
		}
		else
		{
			if(this->mRealm.GetUsers().find(peerId)==this->mRealm.GetUsers().end())
			{
				if(!this->mRealm.GetUsers()[peerId].GetIsLoggedIn())
				{
					std::stringstream s;
					s << "A user claiming to be " << peerId << " attempted to perform an operation that requires authentication.";
					this->Log(s);
					return peerId;
				}
				if(!this->mRealm.GetUsers()[peerId].GetHasEnteredWorld())
				{
					std::stringstream s;
					s << peerId << " attempted to perform an operation that requires a character in the world.";
					this->Log(s);
					return peerId;
				}
			}
		}

		try
        {
			//TODO: validate request

			//Inform other users of change.
			if(this->ChangeMap(peerId,message.GetTargetMap()))
			{
				//Inform client that request was accepted.
				ChangeMapResponseMessage response;
				response.SetTargetMap(message.GetTargetMap());
				this->SendMessageToPeer(peerId,response);

				//Send player all players, npcs, items, and monsters on new map.
				this->SendCharactersOnMapToPlayer(peerId);
				this->SendNpcsOnMapToPlayer(peerId);
				this->SendItemsOnMapToPlayer(peerId);
				this->SendMonstersOnMapToPlayer(peerId);
			}
			else
			{
				//nothing at this point.
			}
		}
		catch(Enigma::EnigmaException& e)
		{
			std::cout << "Enigma Exception: " << e.what() << std::endl;
		}
		catch (Enigma::HardwareException&)
		{
			throw;
		}
		catch (std::overflow_error&)
		{
			throw; //may need to pop some calls off the stack.
		}
		catch (std::exception& e)
		{
			std::cout << "Exception: " << e.what() << std::endl;
		}

		return result;
	}

    std::string ServerSessionManager::ProcessMessage(NpcOnMapUpdateRequestMessage& message,const std::string& peerId)
	{
		boost::shared_lock< boost::shared_mutex > lock1(this->mRealm.SceneMutex);
		boost::shared_lock< boost::shared_mutex > lock2(this->mRealm.ApplicationMutex);

		std::string result=peerId;

		if(peerId.empty())
		{ 
				std::stringstream s;
				s << "Unauthenticated users cannot perform this action.";
				this->Log(s);
			return peerId;
		}
		else
		{
			if(this->mRealm.GetUsers().find(peerId)==this->mRealm.GetUsers().end())
			{
				if(!this->mRealm.GetUsers()[peerId].GetIsLoggedIn())
				{
					std::stringstream s;
					s << "A user claiming to be " << peerId << " attempted to perform an operation that requires authentication.";
					this->Log(s);
					return peerId;
				}
				if(!this->mRealm.GetUsers()[peerId].GetHasEnteredWorld())
				{
					std::stringstream s;
					s << peerId << " attempted to perform an operation that requires a character in the world.";
					this->Log(s);
					return peerId;
				}
			}
		}

		try
        {
			this->SendNpcsOnMapToPlayer(peerId);
		}
		catch(Enigma::EnigmaException& e)
		{
			std::cout << "Enigma Exception: " << e.what() << std::endl;
		}
		catch (Enigma::HardwareException&)
		{
			throw;
		}
		catch (std::overflow_error&)
		{
			throw; //may need to pop some calls off the stack.
		}
		catch (std::exception& e)
		{
			std::cout << "Exception: " << e.what() << std::endl;
		}

		return result;
	}

    std::string ServerSessionManager::ProcessMessage(MonsterOnMapUpdateRequestMessage& message,const std::string& peerId)
	{
		boost::shared_lock< boost::shared_mutex > lock1(this->mRealm.SceneMutex);
		boost::shared_lock< boost::shared_mutex > lock2(this->mRealm.ApplicationMutex);

		std::string result=peerId;

		if(peerId.empty())
		{ 
				std::stringstream s;
				s << "Unauthenticated users cannot perform this action.";
				this->Log(s);
			return peerId;
		}
		else
		{
			if(this->mRealm.GetUsers().find(peerId)==this->mRealm.GetUsers().end())
			{
				if(!this->mRealm.GetUsers()[peerId].GetIsLoggedIn())
				{
					std::stringstream s;
					s << "A user claiming to be " << peerId << " attempted to perform an operation that requires authentication.";
					this->Log(s);
					return peerId;
				}
				if(!this->mRealm.GetUsers()[peerId].GetHasEnteredWorld())
				{
					std::stringstream s;
					s << peerId << " attempted to perform an operation that requires a character in the world.";
					this->Log(s);
					return peerId;
				}
			}
		}

		try
        {
			this->SendMonstersOnMapToPlayer(peerId);
		}
		catch(Enigma::EnigmaException& e)
		{
			std::cout << "Enigma Exception: " << e.what() << std::endl;
		}
		catch (Enigma::HardwareException&)
		{
			throw;
		}
		catch (std::overflow_error&)
		{
			throw; //may need to pop some calls off the stack.
		}
		catch (std::exception& e)
		{
			std::cout << "Exception: " << e.what() << std::endl;
		}

		return result;
	}

    std::string ServerSessionManager::ProcessMessage(ItemOnMapUpdateRequestMessage& message,const std::string& peerId)
	{
		boost::shared_lock< boost::shared_mutex > lock1(this->mRealm.SceneMutex);
		boost::shared_lock< boost::shared_mutex > lock2(this->mRealm.ApplicationMutex);

		std::string result=peerId;

		if(peerId.empty())
		{ 
				std::stringstream s;
				s << "Unauthenticated users cannot perform this action.";
				this->Log(s);
			return peerId;
		}
		else
		{
			if(this->mRealm.GetUsers().find(peerId)==this->mRealm.GetUsers().end())
			{
				if(!this->mRealm.GetUsers()[peerId].GetIsLoggedIn())
				{
					std::stringstream s;
					s << "A user claiming to be " << peerId << " attempted to perform an operation that requires authentication.";
					this->Log(s);
					return peerId;
				}
				if(!this->mRealm.GetUsers()[peerId].GetHasEnteredWorld())
				{
					std::stringstream s;
					s << peerId << " attempted to perform an operation that requires a character in the world.";
					this->Log(s);
					return peerId;
				}
			}
		}

		try
        {
			this->SendItemsOnMapToPlayer(peerId);
		}
		catch(Enigma::EnigmaException& e)
		{
			std::cout << "Enigma Exception: " << e.what() << std::endl;
		}
		catch (Enigma::HardwareException&)
		{
			throw;
		}
		catch (std::overflow_error&)
		{
			throw; //may need to pop some calls off the stack.
		}
		catch (std::exception& e)
		{
			std::cout << "Exception: " << e.what() << std::endl;
		}

		return result;
	}
//Movement End

//Chat Start
    std::string ServerSessionManager::ProcessMessage(ChatRequestMessage& message,const std::string& peerId)
	{
		boost::shared_lock< boost::shared_mutex > lock1(this->mRealm.ApplicationMutex);
		boost::shared_lock< boost::shared_mutex > lock2(this->mRealm.SceneMutex);

		std::string result=peerId;

		if(peerId.empty())
		{ 
				std::stringstream s;
				s << "Unauthenticated users cannot perform this action.";
				this->Log(s);
			return peerId;
		}
		else
		{
			if(this->mRealm.GetUsers().find(peerId)==this->mRealm.GetUsers().end())
			{
				if(!this->mRealm.GetUsers()[peerId].GetIsLoggedIn())
				{
					std::stringstream s;
					s << "A user claiming to be " << peerId << " attempted to perform an operation that requires authentication.";
					this->Log(s);
					return peerId;
				}
				if(!this->mRealm.GetUsers()[peerId].GetHasEnteredWorld())
				{
					std::stringstream s;
					s << peerId << " attempted to perform an operation that requires a character in the world.";
					this->Log(s);
					return peerId;
				}
			}
		}

		try
        {
            ChatResponseMessage response;
			std::string whisperUser="";

            response.SetChatMessage(message.GetChatMessage());
            response.SetChatTarget(this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetCharacters()[this->mRealm.GetUsers()[peerId].GetCurrentCharacterId()].GetName()); //target will be the person who sent it.
            response.SetChatType(message.GetChatType());

            switch(response.GetChatType())
            {
                case CHAT_TYPE_MAP:
                    this->SendMessageToMap(peerId,response);
                break;
                case CHAT_TYPE_BROADCAST:
                    this->SendMessageToWorld(peerId,response);
                break;
                case CHAT_TYPE_WHISPER:
                    whisperUser=this->GetUserByCharacterName(message.GetChatTarget());
                    if(!whisperUser.empty())
                    {
                        this->SendMessageToPeer(whisperUser,response);
                    }
                    else
                    {
                        this->SendServerMessageToPeer(peerId,"Couldn't whisper user. User may not exist.");
                    }
                break;
                case CHAT_TYPE_PARTY:
					{
					boost::shared_lock< boost::shared_mutex > lock3(this->mRealm.ChatMutex);
                    this->SendMessageToParty(peerId,response);
					}
                break;
                case CHAT_TYPE_GUILD:
					{
					boost::shared_lock< boost::shared_mutex > lock4(this->mRealm.ChatMutex);
					this->SendMessageToGuild(peerId,response);
					}
                break;
                default:
                    std::cout << "Unknown chat type." << std::endl;
                break;
            }
		}
		catch(Enigma::EnigmaException& e)
		{
			std::cout << "Enigma Exception: " << e.what() << std::endl;
		}
		catch (Enigma::HardwareException&)
		{
			throw;
		}
		catch (std::overflow_error&)
		{
			throw; //may need to pop some calls off the stack.
		}
		catch (std::exception& e)
		{
			std::cout << "Exception: " << e.what() << std::endl;
		}

		return result;
	}

    std::string ServerSessionManager::ProcessMessage(InviteRequestMessage& message,const std::string& peerId)
	{
		boost::unique_lock< boost::shared_mutex > lock1(this->mRealm.ApplicationMutex);
		boost::unique_lock< boost::shared_mutex > lock2(this->mRealm.SceneMutex);
		boost::unique_lock< boost::shared_mutex > lock3(this->mRealm.ChatMutex);

		std::string result=peerId;

		if(peerId.empty())
		{ 
				std::stringstream s;
				s << "Unauthenticated users cannot perform this action.";
				this->Log(s);
			return peerId;
		}
		else
		{
			if(this->mRealm.GetUsers().find(peerId)==this->mRealm.GetUsers().end())
			{
				if(!this->mRealm.GetUsers()[peerId].GetIsLoggedIn())
				{
					std::stringstream s;
					s << "A user claiming to be " << peerId << " attempted to perform an operation that requires authentication.";
					this->Log(s);
					return peerId;
				}
				if(!this->mRealm.GetUsers()[peerId].GetHasEnteredWorld())
				{
					std::stringstream s;
					s << peerId << " attempted to perform an operation that requires a character in the world.";
					this->Log(s);
					return peerId;
				}
			}
		}

		try
        {
            std::string invitedUser="";
            InviteResponseMessage response;
            std::string msg="";

            if(this->mRealm.GetParties().size()<=0)
            {
                Party fakeParty;
                fakeParty.SetLeaderId(0);
                fakeParty.SetName("");
                this->mRealm.GetParties().push_back(fakeParty);
            }

            if(this->mRealm.GetGuilds().size()<=0)
            {
                Guild fakeGuild;
                fakeGuild.SetLeaderId(0);
                fakeGuild.SetName("");
                this->mRealm.GetGuilds()[0]=fakeGuild;
            }

			int currentPartyId = this->GetCurrentCharacter(peerId).GetPartyId();
			int currentGuildId = this->GetCurrentCharacter(peerId).GetGuildId();

            if(message.GetTarget().length()>0)
            {
				invitedUser=this->GetUserByCharacterName(message.GetTarget());
                if(!invitedUser.empty())
                {
                    if(invitedUser==peerId)
                    {
						this->SendServerMessageToPeer(peerId,"You can't invite yourself to a party.");
                        return result;
                    }

                    switch(message.GetInviteType())
                    {
                        case INVITE_TYPE_PARTY:	
                            /*
							 * Check to see if the current party is zero if so then create a new party &
							 * make the inviting player the party leader. If not then simply proceed.
							 */
							if(currentPartyId==0)
                            {
                                Party party;
								party.SetLeaderId(this->GetCurrentCharacter(peerId).GetId());
                                party.SetName(this->GetCurrentCharacter(peerId).GetName());
								this->mRealm.GetParties().push_back(party);
								this->GetCurrentCharacter(peerId).SetPartyId(this->mRealm.GetParties().size()-1);
								
								//Update current party Id so later code is looking at the right party.
								currentPartyId = this->GetCurrentCharacter(peerId).GetPartyId();
                            }

							/*
							 * Check to see if the user requesting to invite another player is the party leader.
							 * If the requesting player has that authority then send the request to the invitee.
							 * If not then inform the inviting player that they can't invite players to the party.
							 */
                            if(this->mRealm.GetParties()[currentPartyId].GetLeaderId()==this->GetCurrentCharacter(peerId).GetId())
                            {
                                response.SetName(this->GetCurrentCharacter(peerId).GetName());
                                response.SetInviteId(this->GetCurrentCharacter(peerId).GetPartyId());
								response.SetInviteType(message.GetInviteType());
                                this->SendMessageToPeer(invitedUser,response);
                            }
                            else
                            {
                                this->SendServerMessageToPeer(peerId,"You do not have permission to invite to the party.");
                            }
                        break;
                        case INVITE_TYPE_GUILD:
							if(currentGuildId!=0)
                            {
								int neededLeaderId = this->GetCurrentCharacter(peerId).GetId();
								int actualLeaderId = this->GetCurrentGuild(peerId).GetLeaderId();
                                if(neededLeaderId == actualLeaderId)
                                {
                                    response.SetName(this->GetCurrentGuild(peerId).GetName());
                                    response.SetInviteId(currentGuildId);
                                    response.SetInviteType(message.GetInviteType());
                                    this->SendMessageToPeer(invitedUser,response);
                                }
                                else
                                {
                                    this->SendServerMessageToPeer(peerId,"You do not have permission to invite to the guild.");
									
									std::cout << neededLeaderId << " != " <<  actualLeaderId << std::endl;
                                }
                            }
                            else
                            {
                                this->SendServerMessageToPeer(peerId,"You are not in a guild.");
                            }
                        break;
                        case INVITE_TYPE_UNKNOWN:
                            std::cout << "Unknown invite type." << std::endl;
                        break;
                        default:
                            std::cout << "Invalid invite type." << std::endl;
                        break;
                    }
                }
                else
                {
                    this->SendServerMessageToPeer(peerId,"Couldn't invite user. User may not exist.");
                }
            }
            else
            {
                switch(message.GetInviteType())
                {
                    case INVITE_TYPE_PARTY:
                        if(message.GetStatus()==STATUS_OK)
                        {
                            if(this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetCharacters()[this->mRealm.GetUsers()[peerId].GetCurrentCharacterId()].GetPartyId()>0)
                            {
                                msg="";
                                msg.append(this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetCharacters()[this->mRealm.GetUsers()[peerId].GetCurrentCharacterId()].GetName());
                                msg.append(" left party.");
                                this->SendServerMessageToParty(peerId,msg);

                                msg="You left party.";
                                this->SendServerMessageToPeer(peerId,msg);
                            }

                            msg="";
                            msg.append(this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetCharacters()[this->mRealm.GetUsers()[peerId].GetCurrentCharacterId()].GetName());
                            msg.append(" joined party.");
                            this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetCharacters()[this->mRealm.GetUsers()[peerId].GetCurrentCharacterId()].SetPartyId(message.GetInviteId());
                            this->SendServerMessageToParty(peerId,msg);

                            msg="You joined party.";
                            this->SendServerMessageToPeer(peerId,msg);
                        }
                        else
                        {
                            msg="";
                            msg.append(this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetCharacters()[this->mRealm.GetUsers()[peerId].GetCurrentCharacterId()].GetName());
                            msg.append(" refused to join party.");
                            this->SendServerMessageToParty(peerId,msg);

                            msg="You refused to join party.";
                            this->SendServerMessageToPeer(peerId,msg);
                        }
                    break;
                    case INVITE_TYPE_GUILD:
                        if(message.GetStatus()==STATUS_OK)
                        {
                            if(this->GetCurrentCharacter(peerId).GetGuildId()>0)
                            {
                                msg="";
                                msg.append(this->GetCurrentCharacter(peerId).GetName());
                                msg.append(" left guild.");
                                this->SendServerMessageToGuild(peerId,msg);

                                msg="You left guild.";
                                this->SendServerMessageToPeer(peerId,msg);
                            }

                            msg="";
                            msg.append(this->GetCurrentCharacter(peerId).GetName());
                            msg.append(" joined guild.");

                            this->GetCurrentCharacter(peerId).SetGuildId(message.GetInviteId());
							this->mDataAccess.UpdateCharacterGuild(this->GetCurrentCharacter(peerId).GetId(),this->GetCurrentCharacter(peerId).GetGuildId());

                            this->SendServerMessageToGuild(peerId,msg);

                            msg="You joined guild.";
                            this->SendServerMessageToPeer(peerId,msg);
                        }
                        else
                        {
                            msg="";
                            msg.append(this->GetCurrentCharacter(peerId).GetName());
                            msg.append(" refused to join guild.");
                            this->SendServerMessageToGuild(peerId,msg);

                            msg="You refused to join guild.";
                            this->SendServerMessageToPeer(peerId,msg);
                        }
                    break;
                    case INVITE_TYPE_UNKNOWN:
                        std::cout << "Unknown invite type." << std::endl;
                    break;
                    default:
                        std::cout << "Invalid invite type." << std::endl;
                    break;
                }
            }
		}
		catch(Enigma::EnigmaException& e)
		{
			std::cout << "Enigma Exception: " << e.what() << std::endl;
		}
		catch (Enigma::HardwareException&)
		{
			throw;
		}
		catch (std::overflow_error&)
		{
			throw; //may need to pop some calls off the stack.
		}
		catch (std::exception& e)
		{
			std::cout << "Exception: " << e.what() << std::endl;
		}

		return result;
	}

    std::string ServerSessionManager::ProcessMessage(KickRequestMessage& message,const std::string& peerId)
	{
		boost::unique_lock< boost::shared_mutex > lock1(this->mRealm.ApplicationMutex);
		boost::unique_lock< boost::shared_mutex > lock2(this->mRealm.SceneMutex);
		boost::unique_lock< boost::shared_mutex > lock3(this->mRealm.ChatMutex);

		std::string result=peerId;

		if(peerId.empty())
		{ 
				std::stringstream s;
				s << "Unauthenticated users cannot perform this action.";
				this->Log(s);
			return peerId;
		}
		else
		{
			if(this->mRealm.GetUsers().find(peerId)==this->mRealm.GetUsers().end())
			{
				if(!this->mRealm.GetUsers()[peerId].GetIsLoggedIn())
				{
					std::stringstream s;
					s << "A user claiming to be " << peerId << " attempted to perform an operation that requires authentication.";
					this->Log(s);
					return peerId;
				}
				if(!this->mRealm.GetUsers()[peerId].GetHasEnteredWorld())
				{
					std::stringstream s;
					s << peerId << " attempted to perform an operation that requires a character in the world.";
					this->Log(s);
					return peerId;
				}
			}
		}

		try
        {
			std::string kickedUser="";
			std::string msg="";

			int currentPartyId = this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetCharacters()[this->mRealm.GetUsers()[peerId].GetCurrentCharacterId()].GetPartyId();
			int currentGuildId = this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetCharacters()[this->mRealm.GetUsers()[peerId].GetCurrentCharacterId()].GetGuildId();

			if(message.GetName().length()>0)
			{
				kickedUser=this->GetUserByCharacterName(message.GetName());
				if(kickedUser.empty())
				{
					this->SendServerMessageToPeer(peerId,"Player not found.");
					return result;
				}

				int kickedPartyId = this->mRealm.GetMaps()[this->mRealm.GetUsers()[kickedUser].GetCurrentMapId()].GetCharacters()[this->mRealm.GetUsers()[kickedUser].GetCurrentCharacterId()].GetPartyId();
				int kickedGuildId = this->mRealm.GetMaps()[this->mRealm.GetUsers()[kickedUser].GetCurrentMapId()].GetCharacters()[this->mRealm.GetUsers()[kickedUser].GetCurrentCharacterId()].GetGuildId();

				switch(message.GetKickType())
				{
					case KICK_TYPE_PARTY:
						if(kickedPartyId==currentPartyId)
						{
							if(this->mRealm.GetParties()[currentPartyId].GetLeaderId()==this->mRealm.GetUsers()[peerId].GetCurrentCharacterId())
							{
								msg="";
								msg.append(this->mRealm.GetMaps()[this->mRealm.GetUsers()[kickedUser].GetCurrentMapId()].GetCharacters()[this->mRealm.GetUsers()[kickedUser].GetCurrentCharacterId()].GetName());
								msg.append(" was kicked from the party by ");
								msg.append(this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetCharacters()[this->mRealm.GetUsers()[peerId].GetCurrentCharacterId()].GetName());
								msg.append(".");
								this->mRealm.GetMaps()[this->mRealm.GetUsers()[kickedUser].GetCurrentMapId()].GetCharacters()[this->mRealm.GetUsers()[kickedUser].GetCurrentCharacterId()].SetPartyId(0);
								this->SendServerMessageToPeer(peerId,msg);
								this->SendServerMessageToParty(peerId,msg);
								this->SendServerMessageToPeer(kickedUser,"You were kicked from the party.");
							}
							else
							{
								this->SendServerMessageToPeer(kickedUser,"You do not have permission to kick from the party.");
							}
						}
					break;
					case KICK_TYPE_GUILD:
						if(kickedGuildId==currentGuildId)
						{
							if(this->mRealm.GetGuilds()[currentPartyId].GetLeaderId()==this->mRealm.GetUsers()[peerId].GetCurrentCharacterId())
							{
								msg="";
								msg.append(this->mRealm.GetMaps()[this->mRealm.GetUsers()[kickedUser].GetCurrentMapId()].GetCharacters()[this->mRealm.GetUsers()[kickedUser].GetCurrentCharacterId()].GetName());
								msg.append(" was kicked from the guild by ");
								msg.append(this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetCharacters()[this->mRealm.GetUsers()[peerId].GetCurrentCharacterId()].GetName());
								msg.append(".");
								
								this->GetCurrentCharacter(kickedUser).SetGuildId(0);
								this->mDataAccess.UpdateCharacterGuild(this->GetCurrentCharacter(peerId).GetId(),this->GetCurrentCharacter(peerId).GetGuildId());
								
								this->SendServerMessageToPeer(peerId,msg);
								this->SendServerMessageToGuild(peerId,msg);
								this->SendServerMessageToPeer(kickedUser,"You were kicked from the guild.");
							}
							else
							{
								this->SendServerMessageToPeer(kickedUser,"You do not have permission to kick from the guild.");
							}
						}
					break;
					case KICK_TYPE_UNKNOWN:
						std::cout << "Unknown kick type." << std::endl;
					break;
					default:
						std::cout << "Invalid kick type." << std::endl;
					break;
				}
			}
			else
			{
				switch(message.GetKickType())
				{
					case KICK_TYPE_PARTY:
						if(currentPartyId>0)
						{
							msg="";
							msg.append(this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetCharacters()[this->mRealm.GetUsers()[peerId].GetCurrentCharacterId()].GetName());
							msg.append(" left the party.");
							this->SendServerMessageToPeer(peerId,"You left the party.");
							this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetCharacters()[this->mRealm.GetUsers()[peerId].GetCurrentCharacterId()].SetPartyId(0);
							this->SendServerMessageToParty(peerId,msg);
						}
					break;
					case KICK_TYPE_GUILD:
						if(currentGuildId>0)
						{
							msg="";
							msg.append(this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetCharacters()[this->mRealm.GetUsers()[peerId].GetCurrentCharacterId()].GetName());
							msg.append(" left the guild.");
							this->SendServerMessageToPeer(peerId,"You left the guild.");
							this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetCharacters()[this->mRealm.GetUsers()[peerId].GetCurrentCharacterId()].SetGuildId(0);
							this->SendServerMessageToGuild(peerId,msg);
						}
					break;
					case KICK_TYPE_UNKNOWN:
						std::cout << "Unknown kick type." << std::endl;
					break;
					default:
						std::cout << "Invalid kick type." << std::endl;
					break;
				}
			}
		}
		catch(Enigma::EnigmaException& e)
		{
			std::cout << "Enigma Exception: " << e.what() << std::endl;
		}
		catch (Enigma::HardwareException&)
		{
			throw;
		}
		catch (std::overflow_error&)
		{
			throw; //may need to pop some calls off the stack.
		}
		catch (std::exception& e)
		{
			std::cout << "Exception: " << e.what() << std::endl;
		}

		return result;
	}

    std::string ServerSessionManager::ProcessMessage(PlayerListRequestMessage& message,const std::string& peerId)
	{
		boost::shared_lock< boost::shared_mutex > lock1(this->mRealm.ApplicationMutex);
		boost::shared_lock< boost::shared_mutex > lock2(this->mRealm.SceneMutex);

		std::string result=peerId;

		if(peerId.empty())
		{ 
				std::stringstream s;
				s << "Unauthenticated users cannot perform this action.";
				this->Log(s);
			return peerId;
		}
		else
		{
			if(this->mRealm.GetUsers().find(peerId)==this->mRealm.GetUsers().end())
			{
				if(!this->mRealm.GetUsers()[peerId].GetIsLoggedIn())
				{
					std::stringstream s;
					s << "A user claiming to be " << peerId << " attempted to perform an operation that requires authentication.";
					this->Log(s);
					return peerId;
				}
				if(!this->mRealm.GetUsers()[peerId].GetHasEnteredWorld())
				{
					std::stringstream s;
					s << peerId << " attempted to perform an operation that requires a character in the world.";
					this->Log(s);
					return peerId;
				}
			}
		}

		try
        {
			User user = this->mRealm.GetUsers()[peerId];
			switch(message.GetPlayerListType())
			{
				case LIST_TYPE_ALL:
					BOOST_FOREACH(UserPair player,this->mRealm.GetUsers())
					{
						if(player.second.GetCurrentCharacterId()!=user.GetCurrentCharacterId())
						{
							this->SendServerMessageToPeer(peerId,GetCurrentCharacter(player.first).GetName());
						}
					}
				break;
				case LIST_TYPE_MAP:
					BOOST_FOREACH(UserPair player,this->mRealm.GetUsers())
					{
						if(player.second.GetCurrentMapId()==user.GetCurrentMapId()
							&&player.second.GetCurrentCharacterId()!=user.GetCurrentCharacterId())
						{
							this->SendServerMessageToPeer(peerId,GetCurrentCharacter(player.first).GetName());
						}
					}
				break;
				case LIST_TYPE_GUILD:
					{
					boost::shared_lock< boost::shared_mutex > lock3(this->mRealm.ChatMutex);
					BOOST_FOREACH(UserPair player,this->mRealm.GetUsers())
					{
						if(this->GetCurrentCharacter(user.GetUserName()).GetGuildId()>0
							&& this->GetCurrentGuild(player.first).GetId()==this->GetCurrentGuild(user.GetUserName()).GetId()
							&& player.second.GetCurrentCharacterId()!=user.GetCurrentCharacterId())
						{
							this->SendServerMessageToPeer(peerId,GetCurrentCharacter(player.first).GetName());
						}
					}
					}
				break;
				case LIST_TYPE_PARTY:
					{
					boost::shared_lock< boost::shared_mutex > lock4(this->mRealm.ChatMutex);
					BOOST_FOREACH(UserPair player,this->mRealm.GetUsers())
					{
						if(this->GetCurrentCharacter(user.GetUserName()).GetPartyId()>0
							&& this->GetCurrentParty(player.first).GetId()==this->GetCurrentParty(user.GetUserName()).GetId()
							&& player.second.GetCurrentCharacterId()!=user.GetCurrentCharacterId())
						{
							this->SendServerMessageToPeer(peerId,GetCurrentCharacter(player.first).GetName());
						}
					}
					}
				break;
				case LIST_TYPE_UNKNOWN:
					std::cout << "Unknown list type." << std::endl;
				break;
				default:
					std::cout << "Invalid list type." << std::endl;
				break;
			}
		}
		catch(Enigma::EnigmaException& e)
		{
			std::cout << "Enigma Exception: " << e.what() << std::endl;
		}
		catch (Enigma::HardwareException&)
		{
			throw;
		}
		catch (std::overflow_error&)
		{
			throw; //may need to pop some calls off the stack.
		}
		catch (std::exception& e)
		{
			std::cout << "Exception: " << e.what() << std::endl;
		}

		return result;
	}

    std::string ServerSessionManager::ProcessMessage(CreatePlayerOrganizationRequestMessage& message,const std::string& peerId)
	{
		boost::shared_lock< boost::shared_mutex > lock1(this->mRealm.ApplicationMutex);
		boost::unique_lock< boost::shared_mutex > lock2(this->mRealm.SceneMutex);

		std::string result=peerId;

		if(peerId.empty())
		{ 
				std::stringstream s;
				s << "Unauthenticated users cannot perform this action.";
				this->Log(s);
			return peerId;
		}
		else
		{
			if(this->mRealm.GetUsers().find(peerId)==this->mRealm.GetUsers().end())
			{
				if(!this->mRealm.GetUsers()[peerId].GetIsLoggedIn())
				{
					std::stringstream s;
					s << "A user claiming to be " << peerId << " attempted to perform an operation that requires authentication.";
					this->Log(s);
					return peerId;
				}
				if(!this->mRealm.GetUsers()[peerId].GetHasEnteredWorld())
				{
					std::stringstream s;
					s << peerId << " attempted to perform an operation that requires a character in the world.";
					this->Log(s);
					return peerId;
				}
			}
		}

		try
        {
			switch(message.GetOrganizationType())
			{
				case ORGANIZATION_TYPE_GUILD:
					{
					boost::unique_lock< boost::shared_mutex > lock3(this->mRealm.ChatMutex);
					if(this->GetCurrentCharacter(peerId).GetGuildId()==0)
					{
						Guild guild;
						guild.SetLeaderId(this->GetCurrentCharacter(peerId).GetId());
						guild.SetName(message.GetName());
						
						this->mDataAccess.CreateGuild<Guild>(guild);

						this->mRealm.GetGuilds()[guild.GetId()]=guild;
						this->GetCurrentCharacter(peerId).SetGuildId(guild.GetId());
						
						this->mDataAccess.UpdateCharacterGuild(this->GetCurrentCharacter(peerId).GetId(),this->GetCurrentCharacter(peerId).GetGuildId());

						this->SendServerMessageToPeer(peerId,"Guild created.");
					}
					else
					{
						this->SendServerMessageToPeer(peerId,"You are already in a guild.");
					}
					}
				break;
				case ORGANIZATION_TYPE_PARTY:
					{
					boost::unique_lock< boost::shared_mutex > lock4(this->mRealm.ChatMutex);
					if(this->GetCurrentCharacter(peerId).GetPartyId()==0)
					{
						Party party;
						party.SetLeaderId(this->GetCurrentCharacter(peerId).GetId());
						party.SetName(message.GetName());
						this->mRealm.GetParties().push_back(party);
						this->GetCurrentCharacter(peerId).SetPartyId(this->mRealm.GetParties().size()-1);
						this->SendServerMessageToPeer(peerId,"Party created.");
					}
					else
					{
						this->SendServerMessageToPeer(peerId,"You are already in a party.");
					}
					}
				break;
				case ORGANIZATION_TYPE_UNKNOWN:
					std::cout << "Unknown organization type." << std::endl;
				break;
				default:
					std::cout << "Invalid organization type." << std::endl;
				break;
			}
		}
		catch(Enigma::EnigmaException& e)
		{
			std::cout << "Enigma Exception: " << e.what() << std::endl;
		}
		catch (Enigma::HardwareException&)
		{
			throw;
		}
		catch (std::overflow_error&)
		{
			throw; //may need to pop some calls off the stack.
		}
		catch (std::exception& e)
		{
			std::cout << "Exception: " << e.what() << std::endl;
		}

		return result;
	}

    std::string ServerSessionManager::ProcessMessage(ModifyPlayerOrganizationRankRequestMessage& message,const std::string& peerId)
	{
		boost::shared_lock< boost::shared_mutex > lock1(this->mRealm.ApplicationMutex);
		boost::unique_lock< boost::shared_mutex > lock2(this->mRealm.SceneMutex);

		std::string result=peerId;

		if(peerId.empty())
		{ 
				std::stringstream s;
				s << "Unauthenticated users cannot perform this action.";
				this->Log(s);
			return peerId;
		}
		else
		{
			if(this->mRealm.GetUsers().find(peerId)==this->mRealm.GetUsers().end())
			{
				if(!this->mRealm.GetUsers()[peerId].GetIsLoggedIn())
				{
					std::stringstream s;
					s << "A user claiming to be " << peerId << " attempted to perform an operation that requires authentication.";
					this->Log(s);
					return peerId;
				}
				if(!this->mRealm.GetUsers()[peerId].GetHasEnteredWorld())
				{
					std::stringstream s;
					s << peerId << " attempted to perform an operation that requires a character in the world.";
					this->Log(s);
					return peerId;
				}
			}
		}

		try
        {
			switch(message.GetOrganizationType())
			{
				case ORGANIZATION_TYPE_GUILD:
					{
					boost::unique_lock< boost::shared_mutex > lock3(this->mRealm.ChatMutex);
					if(this->GetCurrentCharacter(peerId).GetGuildId()!=0)
					{
						if(this->GetIsCurrentGuildLeader(peerId) || this->GetCurrentRank(peerId).GetCanAdjustRank())
						{
							Rank rank;
							rank.SetPermissions(message.GetPermissions());
							rank.SetName(message.GetName());

							this->mRealm.GetGuilds()[this->GetCurrentCharacter(peerId).GetGuildId()].GetRanks()[rank.GetName()]=rank;
							this->SendServerMessageToPeer(peerId,"Guild ranks updated.");
						}
						else
						{
							this->SendServerMessageToPeer(peerId,"You do not have permission to change guild ranks.");
						}
					}
					else
					{
						this->SendServerMessageToPeer(peerId,"You are not in a guild.");
					}
					}
				break;
				case ORGANIZATION_TYPE_PARTY:
					{
					boost::unique_lock< boost::shared_mutex > lock4(this->mRealm.ChatMutex);
					if(this->GetCurrentCharacter(peerId).GetPartyId()!=0)
					{
						//No ranks in party this is just a placeholder.
					}
					else
					{
						this->SendServerMessageToPeer(peerId,"You are not in a party.");
					}
					}
				break;
				case ORGANIZATION_TYPE_UNKNOWN:
					std::cout << "Unknown organization type." << std::endl;
				break;
				default:
					std::cout << "Invalid organization type." << std::endl;
				break;
			}
		}
		catch(Enigma::EnigmaException& e)
		{
			std::cout << "Enigma Exception: " << e.what() << std::endl;
		}
		catch (Enigma::HardwareException&)
		{
			throw;
		}
		catch (std::overflow_error&)
		{
			throw; //may need to pop some calls off the stack.
		}
		catch (std::exception& e)
		{
			std::cout << "Exception: " << e.what() << std::endl;
		}

		return result;
	}

    std::string ServerSessionManager::ProcessMessage(SetPlayerRankRequestMessage& message,const std::string& peerId)
	{
		boost::unique_lock< boost::shared_mutex > lock1(this->mRealm.ApplicationMutex);
		boost::shared_lock< boost::shared_mutex > lock2(this->mRealm.SceneMutex); //for getting character

		std::string result=peerId;

		if(peerId.empty())
		{ 
				std::stringstream s;
				s << "Unauthenticated users cannot perform this action.";
				this->Log(s);
			return peerId;
		}
		else
		{
			if(this->mRealm.GetUsers().find(peerId)==this->mRealm.GetUsers().end())
			{
				if(!this->mRealm.GetUsers()[peerId].GetIsLoggedIn())
				{
					std::stringstream s;
					s << "A user claiming to be " << peerId << " attempted to perform an operation that requires authentication.";
					this->Log(s);
					return peerId;
				}
				if(!this->mRealm.GetUsers()[peerId].GetHasEnteredWorld())
				{
					std::stringstream s;
					s << peerId << " attempted to perform an operation that requires a character in the world.";
					this->Log(s);
					return peerId;
				}
			}
		}

		try
        {
			std::string msg="";
			std::string rankUser="";
			message.Populate();

			rankUser = this->GetUserByCharacterName(message.GetPlayerName());
			if(rankUser.empty())
			{
				this->SendServerMessageToPeer(peerId,"Player not found.");
				return peerId;
			}

			switch(message.GetOrganizationType())
			{
				case ORGANIZATION_TYPE_GUILD:
					if(this->GetCurrentCharacter(peerId).GetGuildId()!=0)
					{
						if(this->GetIsCurrentGuildLeader(peerId) || this->GetCurrentRank(peerId).GetCanAdjustRank())
						{
							this->GetCurrentCharacter(rankUser).SetRankName(message.GetRankName());
							msg="";
							msg.append(this->GetCurrentCharacter(rankUser).GetName());
							msg.append("'s guild rank was changed to ");
							msg.append(message.GetRankName());
							msg.append(".");
							this->SendServerMessageToGuild(rankUser,msg); //send as rankUser so user gets message.
							msg="Your guild rank was changed to ";
							msg.append(message.GetRankName());
							msg.append(".");
							this->SendServerMessageToPeer(rankUser,msg);
						}
						else
						{
							this->SendServerMessageToPeer(peerId,"You do not have permission to assign guild ranks.");
						}
					}
					else
					{
						this->SendServerMessageToPeer(peerId,"You are not in a guild.");
					}
				break;
				case ORGANIZATION_TYPE_PARTY:
					if(this->GetCurrentCharacter(peerId).GetPartyId()!=0)
					{
						//No ranks in party this is just a placeholder.
					}
					else
					{
						this->SendServerMessageToPeer(peerId,"You are not in a party.");
					}
				break;
				case ORGANIZATION_TYPE_UNKNOWN:
					std::cout << "Unknown organization type." << std::endl;
				break;
				default:
					std::cout << "Invalid organization type." << std::endl;
				break;
			}
		}
		catch(Enigma::EnigmaException& e)
		{
			std::cout << "Enigma Exception: " << e.what() << std::endl;
		}
		catch (Enigma::HardwareException&)
		{
			throw;
		}
		catch (std::overflow_error&)
		{
			throw; //may need to pop some calls off the stack.
		}
		catch (std::exception& e)
		{
			std::cout << "Exception: " << e.what() << std::endl;
		}

		return result;
	}

    std::string ServerSessionManager::ProcessMessage(RollRequestMessage& message,const std::string& peerId)
	{
		boost::shared_lock< boost::shared_mutex > lock(this->mRealm.ApplicationMutex);

		std::string result=peerId;

		if(peerId.empty())
		{ 
				std::stringstream s;
				s << "Unauthenticated users cannot perform this action.";
				this->Log(s);
			return peerId;
		}
		else
		{
			if(this->mRealm.GetUsers().find(peerId)==this->mRealm.GetUsers().end())
			{
				if(!this->mRealm.GetUsers()[peerId].GetIsLoggedIn())
				{
					std::stringstream s;
					s << "A user claiming to be " << peerId << " attempted to perform an operation that requires authentication.";
					this->Log(s);
					return peerId;
				}
				if(!this->mRealm.GetUsers()[peerId].GetHasEnteredWorld())
				{
					std::stringstream s;
					s << peerId << " attempted to perform an operation that requires a character in the world.";
					this->Log(s);
					return peerId;
				}
			}
		}

		try
        {
			Enigma::s32 min;
			Enigma::s32 max;
			Enigma::s32 result;
			std::string msg;

			min=1;
			max=message.GetRollLimit();
			result=0;

			srand((unsigned int)time(NULL));
			result = rand() % max + min;

			msg="";
			msg.append(this->GetCurrentCharacter(peerId).GetName().c_str());
			msg.append(" rolls ");
			msg.append(boost::lexical_cast<std::string>(min));
			msg.append("-");
			msg.append(boost::lexical_cast<std::string>(max));
			msg.append(" and gets ");
			msg.append(boost::lexical_cast<std::string>(result));
			msg.append(".");

			this->SendServerMessageToMap(peerId,msg);

			msg="";
			msg.append("You roll ");
			msg.append(boost::lexical_cast<std::string>(min));
			msg.append("-");
			msg.append(boost::lexical_cast<std::string>(max));
			msg.append(" and get ");
			msg.append(boost::lexical_cast<std::string>(result));
			msg.append(".");

			this->SendServerMessageToPeer(peerId,msg);
		}
		catch(Enigma::EnigmaException& e)
		{
			std::cout << "Enigma Exception: " << e.what() << std::endl;
		}
		catch (Enigma::HardwareException&)
		{
			throw;
		}
		catch (std::overflow_error&)
		{
			throw; //may need to pop some calls off the stack.
		}
		catch (std::exception& e)
		{
			std::cout << "Exception: " << e.what() << std::endl;
		}

		return result;
	}

    std::string ServerSessionManager::ProcessMessage(ServerTimeRequestMessage& message,const std::string& peerId)
	{
		boost::shared_lock< boost::shared_mutex > lock(this->mRealm.ApplicationMutex);

		std::string result=peerId;

		if(peerId.empty())
		{ 
				std::stringstream s;
				s << "Unauthenticated users cannot perform this action.";
				this->Log(s);
			return peerId;
		}
		else
		{
			if(this->mRealm.GetUsers().find(peerId)==this->mRealm.GetUsers().end())
			{
				if(!this->mRealm.GetUsers()[peerId].GetIsLoggedIn())
				{
					std::stringstream s;
					s << "A user claiming to be " << peerId << " attempted to perform an operation that requires authentication.";
					this->Log(s);
					return peerId;
				}
			}
		}

		try
        {
            std::string msg;
            time_t rawTime;
            struct tm* timeInfo;

            time (&rawTime);
            timeInfo = localtime(&rawTime);
            msg=asctime(timeInfo);

            msg[msg.length()-1]='\0'; //this will remove the \n that is there by default.

            this->SendServerMessageToPeer(peerId,msg);
		}
		catch(Enigma::EnigmaException& e)
		{
			std::cout << "Enigma Exception: " << e.what() << std::endl;
		}
		catch (Enigma::HardwareException&)
		{
			throw;
		}
		catch (std::overflow_error&)
		{
			throw; //may need to pop some calls off the stack.
		}
		catch (std::exception& e)
		{
			std::cout << "Exception: " << e.what() << std::endl;
		}

		return result;
	}

    std::string ServerSessionManager::ProcessMessage(NpcChatRequestMessage& message,const std::string& peerId)
	{
		boost::shared_lock< boost::shared_mutex > lock1(this->mRealm.ApplicationMutex);
		boost::unique_lock< boost::shared_mutex > lock2(this->mRealm.SceneMutex);

		std::string result=peerId;
		
		if(peerId.empty())
		{ 
				std::stringstream s;
				s << "Unauthenticated users cannot perform this action.";
				this->Log(s);
			return peerId;
		}
		else
		{
			if(this->mRealm.GetUsers().find(peerId)==this->mRealm.GetUsers().end())
			{
				if(!this->mRealm.GetUsers()[peerId].GetIsLoggedIn())
				{
					std::stringstream s;
					s << "A user claiming to be " << peerId << " attempted to perform an operation that requires authentication.";
					this->Log(s);
					return peerId;
				}
				if(!this->mRealm.GetUsers()[peerId].GetHasEnteredWorld())
				{
					std::stringstream s;
					s << peerId << " attempted to perform an operation that requires a character in the world.";
					this->Log(s);
					return peerId;
				}
			}
		}

		bool isLastMessage=false;

		try
        {
			NpcChatResponseMessage npcChatResponseMessage;
			std::string responseId="";
			//update properties variables from internal data structure.

			//if there is a response this is not the initial request.
			if(message.GetChatResponse().length()>0)
			{
				//find next question id so the next set of anwsers can be sent to the client.
				BOOST_FOREACH(NpcAnswerPair npcAnswerPair,this->GetCurrentNpcQuestion(peerId).Answers)
				{
					if(message.GetChatResponse()==npcAnswerPair.second.Text)
					{
						responseId=npcAnswerPair.first;
					}
				}

				std::string nextQuestionId = this->GetCurrentNpcQuestion(peerId).Answers[responseId].NextQuestionId;
				if(nextQuestionId.length()==0)
				{
					isLastMessage=true;
				}
				else
				{
					isLastMessage=false;
				}
				//set new question.
				this->GetCurrentCharacter(peerId).SetNpcQuestionId(nextQuestionId);
			}
			else
			{
				//update current npc so server sends correct information to client.
				this->GetCurrentCharacter(peerId).SetCurrentNpcId(message.GetTargetNpcId());
				this->GetCurrentCharacter(peerId).SetNpcQuestionId(this->GetCurrentNpc(peerId).InitialQuestionId);
			}

			if(!isLastMessage)
			{
				//loop through all the anwsers and add them to the response for the player to pick from.
				BOOST_FOREACH(NpcAnswerPair npcAnswerPair,this->GetCurrentNpcQuestion(peerId).Answers)
				{
					npcChatResponseMessage.GetPossiblePlayerResponses().push_back(npcAnswerPair.second.Text);
				}

				//set the npc message so the player knows what they are responding to.
				npcChatResponseMessage.SetChatResponse(this->GetCurrentNpcQuestion(peerId).Text);
			}
			else
			{
				npcChatResponseMessage.SetChatResponse("");
			}

			//set the npc id so the client knows what npc the message is from.
			npcChatResponseMessage.SetNpcId(this->GetCurrentNpc(peerId).GetId());

			//update the internal data structure that is sent over the network/internet.

			//finally send the message to client.
			this->SendMessageToPeer(peerId,npcChatResponseMessage);
		}
		catch(Enigma::EnigmaException& e)
		{
			std::cout << "Enigma Exception: " << e.what() << std::endl;
		}
		catch (Enigma::HardwareException&)
		{
			throw;
		}
		catch (std::overflow_error&)
		{
			throw; //may need to pop some calls off the stack.
		}
		catch (std::exception& e)
		{
			std::cout << "Exception: " << e.what() << std::endl;
		}

		return result;
	}
//Chat End

//Voice Start
    std::string ServerSessionManager::ProcessMessage(VoiceChatRequestMessage& message,const std::string& peerId)
	{
		boost::shared_lock< boost::shared_mutex > lock1(this->mRealm.ApplicationMutex);
		boost::shared_lock< boost::shared_mutex > lock2(this->mRealm.SceneMutex);

		std::string result=peerId;

		if(peerId.empty())
		{ 
				std::stringstream s;
				s << "Unauthenticated users cannot perform this action.";
				this->Log(s);
			return peerId;
		}
		else
		{
			if(this->mRealm.GetUsers().find(peerId)==this->mRealm.GetUsers().end())
			{
				if(!this->mRealm.GetUsers()[peerId].GetIsLoggedIn())
				{
					std::stringstream s;
					s << "A user claiming to be " << peerId << " attempted to perform an operation that requires authentication.";
					this->Log(s);
					return peerId;
				}
				if(!this->mRealm.GetUsers()[peerId].GetHasEnteredWorld())
				{
					std::stringstream s;
					s << peerId << " attempted to perform an operation that requires a character in the world.";
					this->Log(s);
					return peerId;
				}
			}
		}

		try
        {
			size_t size=0;
			Enigma::u8* buffer=NULL;
			Enigma::s32 voiceChannel=0;
            VoiceChatResponseMessage response;

			voiceChannel = message.GetVoiceChannel();
			buffer = message.GetAudioData(size);

			response.SetCombatantId(this->GetCurrentCharacter(peerId).GetId());
			response.SetVoiceChannel(voiceChannel);
            response.SetAudioData(buffer,size);

			switch(voiceChannel)
			{
				case CHAT_TYPE_BROADCAST:
						this->SendMessageToWorld(peerId,response);
					break;
				case CHAT_TYPE_GUILD:
						this->SendMessageToGuild(peerId,response);
					break;
				case CHAT_TYPE_MAP:
						this->SendMessageToMap(peerId,response);
					break;
				case CHAT_TYPE_PARTY:
					this->SendMessageToParty(peerId,response);
					break;
				default:
						this->SendMessageToMap(peerId,response);
					break;
			}
            
		}
		catch(Enigma::EnigmaException& e)
		{
			std::cout << "Enigma Exception: " << e.what() << std::endl;
		}
		catch (Enigma::HardwareException&)
		{
			throw;
		}
		catch (std::overflow_error&)
		{
			throw; //may need to pop some calls off the stack.
		}
		catch (std::exception& e)
		{
			std::cout << "Exception: " << e.what() << std::endl;
		}

		return result;
	}
//Voice End

//Combat Start
    std::string ServerSessionManager::ProcessMessage(ItemTransferRequestMessage& message,const std::string& peerId)
	{
		boost::shared_lock< boost::shared_mutex > lock1(this->mRealm.ApplicationMutex);
		boost::unique_lock< boost::shared_mutex > lock2(this->mRealm.SceneMutex);

		std::string result=peerId;

		if(peerId.empty())
		{ 
				std::stringstream s;
				s << "Unauthenticated users cannot perform this action.";
				this->Log(s);
			return peerId;
		}
		else
		{
			if(this->mRealm.GetUsers().find(peerId)==this->mRealm.GetUsers().end())
			{
				if(!this->mRealm.GetUsers()[peerId].GetIsLoggedIn())
				{
					std::stringstream s;
					s << "A user claiming to be " << peerId << " attempted to perform an operation that requires authentication.";
					this->Log(s);
					return peerId;
				}
				if(!this->mRealm.GetUsers()[peerId].GetHasEnteredWorld())
				{
					std::stringstream s;
					s << peerId << " attempted to perform an operation that requires a character in the world.";
					this->Log(s);
					return peerId;
				}
			}
		}

		try
        {
			ItemTransferResponseMessage itemTransferResponseMessage;

			//TODO: actually check item stuff.

			itemTransferResponseMessage.SetSourceLocation(message.GetSourceLocation());
			itemTransferResponseMessage.SetSourceId(message.GetSourceId());
			itemTransferResponseMessage.SetDestinationLocation(message.GetDestinationLocation());
			itemTransferResponseMessage.SetDestinationId(message.GetDestinationId());
			itemTransferResponseMessage.SetStackSize(message.GetStackSize());

			this->SendMessageToPeer(peerId,itemTransferResponseMessage);
		}
		catch(Enigma::EnigmaException& e)
		{
			std::cout << "Enigma Exception: " << e.what() << std::endl;
		}
		catch (Enigma::HardwareException&)
		{
			throw;
		}
		catch (std::overflow_error&)
		{
			throw; //may need to pop some calls off the stack.
		}
		catch (std::exception& e)
		{
			std::cout << "Exception: " << e.what() << std::endl;
		}

		return result;
	}

	std::string ServerSessionManager::ProcessMessage(SkillRequestMessage& message,const std::string& peerId)
	{
		boost::shared_lock< boost::shared_mutex > lock1(this->mRealm.ApplicationMutex);
		boost::unique_lock< boost::shared_mutex > lock2(this->mRealm.SceneMutex);

		std::string result=peerId;

		if(peerId.empty())
		{ 
				std::stringstream s;
				s << "Unauthenticated users cannot perform this action.";
				this->Log(s);
			return peerId;
		}
		else
		{
			if(this->mRealm.GetUsers().find(peerId)==this->mRealm.GetUsers().end())
			{
				if(!this->mRealm.GetUsers()[peerId].GetIsLoggedIn())
				{
					std::stringstream s;
					s << "A user claiming to be " << peerId << " attempted to perform an operation that requires authentication.";
					this->Log(s);
					return peerId;
				}
				if(!this->mRealm.GetUsers()[peerId].GetHasEnteredWorld())
				{
					std::stringstream s;
					s << peerId << " attempted to perform an operation that requires a character in the world.";
					this->Log(s);
					return peerId;
				}
			}
		}

		try
        {
			SkillResponseMessage skillResponseMessage;

			//TODO: actually check stuff.

			switch(message.GetTargetPreferenceType())
			{
				case TARGETPREFERENCETYPE_NONE:
					skillResponseMessage.SetTargetId(0);
				break;
				case TARGETPREFERENCETYPE_CHARACTER:
					skillResponseMessage.SetTargetId(message.GetTargetCharacterId());
				break;
				case TARGETPREFERENCETYPE_NPC:
					skillResponseMessage.SetTargetId(message.GetTargetNpcId());
				break;
				case TARGETPREFERENCETYPE_MONSTER:
					skillResponseMessage.SetTargetId(message.GetTargetMonsterId());
				break;
				case TARGETPREFERENCETYPE_ITEM:
					skillResponseMessage.SetTargetId(0);
				break;
				case TARGETPREFERENCETYPE_AREA:
					skillResponseMessage.SetTargetId(0);
				break;
				default:
					skillResponseMessage.SetTargetId(0);
				break;
			}

			std::cout << "Skill used: " << message.GetSkillId() << " at level " << message.GetSkillLevel() << "targeting " << skillResponseMessage.GetTargetId() << std::endl;

			skillResponseMessage.SetSkillId(message.GetSkillId());
			skillResponseMessage.SetSkillLevel(message.GetSkillLevel());

			//TODO: change status

			this->SendMessageToPeer(peerId,skillResponseMessage);
		}
		catch(Enigma::EnigmaException& e)
		{
			std::cout << "Enigma Exception: " << e.what() << std::endl;
		}
		catch (Enigma::HardwareException&)
		{
			throw;
		}
		catch (std::overflow_error&)
		{
			throw; //may need to pop some calls off the stack.
		}
		catch (std::exception& e)
		{
			std::cout << "Exception: " << e.what() << std::endl;
		}

		return result;
	}
//Combat End

	void ServerSessionManager::SendMessageToPeer(const std::string& peerId,MessageContainer& message)
	{
		if(!peerId.empty()) //might be overkill but oh well.
		{
			this->mServerTransmissionManager->SendMessageToPeer(peerId,message);
		}
		else
		{
			throw EnigmaException("A valid peerId is required to send packets except when broadcasting.");
		}
	}

    void ServerSessionManager::SendMessageToMap(const std::string& peerId,MessageContainer& message)
	{
		if(!peerId.empty())
		{		
			BOOST_FOREACH(UserPair item,this->mRealm.GetUsers())
			{
				User user = item.second;
				if(user.GetCurrentMapId() == this->mRealm.GetUsers()[peerId].GetCurrentMapId())
				{
					if(user.GetId() != this->mRealm.GetUsers()[peerId].GetId())
					{ /*No reason to send this to the user who caused it.*/
						this->mServerTransmissionManager->SendMessageToPeer(user.GetUserName(),message);
					}
				}
			}
		}
		else
		{
			throw EnigmaException("A valid peerId is required to send packets except when broadcasting.");
		}
	}

    void ServerSessionManager::SendMessageToParty(const std::string& peerId,MessageContainer& message)
	{
		if(!peerId.empty())
		{
			BOOST_FOREACH(UserPair item,this->mRealm.GetUsers())
			{
				User user = item.second;	
				if(user.GetHasEnteredWorld() && this->GetCurrentCharacter(item.first).GetPartyId()>0)
				{
					if(this->GetCurrentParty(item.first).GetId() == this->GetCurrentParty(peerId).GetId())
					{
						if(user.GetId() != this->mRealm.GetUsers()[peerId].GetId())
						{ /*No reason to send this to the user who caused it.*/
							this->mServerTransmissionManager->SendMessageToPeer(user.GetUserName(),message);
						}
					}
				}
			}
		}
		else
		{
			throw EnigmaException("A valid peerId is required to send packets except when broadcasting.");
		}
	}

    void ServerSessionManager::SendMessageToGuild(const std::string& peerId,MessageContainer& message)
	{
		if(!peerId.empty())
		{
			BOOST_FOREACH(UserPair item,this->mRealm.GetUsers())
			{
				User user = item.second;	
				if(user.GetHasEnteredWorld() && this->GetCurrentCharacter(item.first).GetGuildId()>0)
				{
					if(this->GetCurrentGuild(item.first).GetId() == this->GetCurrentGuild(peerId).GetId())
					{
						
						if(user.GetId() != this->mRealm.GetUsers()[peerId].GetId())
						{ /*No reason to send this to the user who caused it.*/
							this->mServerTransmissionManager->SendMessageToPeer(user.GetUserName(),message);
						}
					}
				}
			}
		}
		else
		{
			throw EnigmaException("A valid peerId is required to send packets except when broadcasting.");
		}
	}

    void ServerSessionManager::SendMessageToWorld(const std::string& peerId,MessageContainer& message)
	{
		this->mServerTransmissionManager->SendMessageToWorld(peerId,message);
	}

	void ServerSessionManager::EnterMap(const std::string& peerId)
	{
		if(!peerId.empty())
		{
			Character character;
			character.GenerateFakeId(); //TODO: remove fake id generation.
			character.SetName(peerId); //TODO: remove fake name.
			this->mRealm.GetUsers()[peerId].SetCurrentMapId(0); //TODO: remove fake starting map.
			this->mRealm.GetUsers()[peerId].SetCurrentCharacterId(character.GetId());

			this->EnterMap(peerId,character);
		}
	}

	void ServerSessionManager::EnterMap(const std::string& peerId,Character character)
	{
		size_t characterId = character.GetId();
        if(!peerId.empty() && characterId > 0)
        {
			this->mRealm.GetUsers()[peerId].SetHasEnteredWorld(true);
			this->mRealm.GetUsers()[peerId].SetCurrentCharacterId(character.GetId());
			this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetCharacters()[this->mRealm.GetUsers()[peerId].GetCurrentCharacterId()]=character;

            PlayerOnMapUpdateResponseMessage response;
            response.SetName(character.GetName());
            response.SetId(character.GetId());
            response.SetModelId(character.GetModelId());
            response.SetIsEnteringMap(true);

            response.SetX(character.GetX());
            response.SetY(character.GetY());
            response.SetZ(character.GetZ());

            this->SendMessageToMap(peerId,response);

            std::cout << character.GetName() << " has entered map " << this->mRealm.GetUsers()[peerId].GetCurrentMapId() << std::endl;
        }
	}

	void ServerSessionManager::ExitMap(const std::string& peerId)
	{
		if(!peerId.empty())
		{
			std::map<size_t,Character>::iterator it = this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetCharacters().find(this->mRealm.GetUsers()[peerId].GetCurrentCharacterId());
			if(it!=this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetCharacters().end())
			{
				Character character=it->second;

                PlayerOnMapUpdateResponseMessage response;
                response.SetId(this->mRealm.GetUsers()[peerId].GetCurrentCharacterId());
                //response.SetModelId(character.GetModelId()); //doesn't matter they are leaving.
                response.SetIsLeavingMap(true);

				this->SendMessageToMap(peerId,response);

				std::cout << character.GetName() << " has exited map " << this->mRealm.GetUsers()[peerId].GetCurrentMapId() << std::endl;
				this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetCharacters().erase(it);

				this->mRealm.GetUsers()[peerId].SetCurrentMapId(0);
				this->mRealm.GetUsers()[peerId].SetHasEnteredWorld(false);
			}
		}
		else
		{
			std::cout << "A user must be valid and have a selected Character to exit a map." << std::endl;
		}
	}

    bool ServerSessionManager::ChangeMap(const std::string& peerId,size_t newMapId)
    {
        try
        {
            Character character;
            character = this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetCharacters()[this->mRealm.GetUsers()[peerId].GetCurrentCharacterId()];

            if(this->mRealm.GetMaps().size()>newMapId)
            {
                this->ExitMap(peerId);

				this->mRealm.GetUsers()[peerId].SetCurrentMapId(newMapId);

                this->EnterMap(peerId,character);
                return true;
            }
            else
            {
                this->SendServerMessageToPeer(peerId,"Invalid Map!");
                return false;
            }
        }
		catch(Enigma::EnigmaException& e)
		{
			std::cout << "Enigma Exception: " << e.what() << std::endl;
		}
		catch (Enigma::HardwareException&)
		{
			throw;
		}
		catch (std::overflow_error&)
		{
			throw; //may need to pop some calls off the stack.
		}
		catch (std::exception& e)
		{
			std::cout << "Exception: " << e.what() << std::endl;
		}

		return false;
    }

	void ServerSessionManager::SendCharactersOnMapToPlayer(const std::string& peerId)
	{
		if(!peerId.empty())
		{
			const int self=1; //the player isn't actually sent so we need to deduct one from the count.
			size_t count=0;
			std::cout << "sending " << this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetCharacters().size()-self << " Characters to " << peerId << " on map " << this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetId() << std::endl;

			BOOST_FOREACH(CharacterPair element,this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetCharacters())
			{
				Character Character=element.second;
				if(Character.GetUserId()>0 &&
					Character.GetId()>0 &&
					Character.GetId()!=this->mRealm.GetUsers()[peerId].GetId())
				{
					PlayerOnMapUpdateResponseMessage response;
					response.SetName(Character.GetName());
					response.SetId(Character.GetId());
					response.SetModelId(Character.GetModelId());
					response.SetIsEnteringMap(true);

					response.SetX(Character.GetX());
					response.SetY(Character.GetY());
					response.SetZ(Character.GetZ());

					this->SendMessageToPeer(peerId,response);
					count++;
				}
			}

			std::cout << "sent " << count << " Characters to " << peerId << " on map " << this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetId() << std::endl;
		}
	}

	void ServerSessionManager::SendNpcsOnMapToPlayer(const std::string& peerId)
	{
		if(!peerId.empty())
		{
			size_t count=0;
			std::cout << "sending " << this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetNpcs().size() << " npcs to " << peerId << " on map " << this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetId() << std::endl;

			BOOST_FOREACH(NpcPair element,this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetNpcs())
			{
				Npc npc=element.second;
				if(npc.GetId()>0)
				{
					NpcOnMapUpdateResponseMessage response;
					response.SetName(npc.GetName());
					response.SetId(npc.GetId());
					response.SetModelId(npc.GetModelId());
					response.SetIsEnteringMap(true);

					response.SetX(npc.GetX());
					response.SetY(npc.GetY());
					response.SetZ(npc.GetZ());

					this->SendMessageToPeer(peerId,response);
					count++;
				}
			}

			std::cout << "sent " << count << " npcs to " << peerId << " on map " << this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetId() << std::endl;
		}
	}

	void ServerSessionManager::SendItemsOnMapToPlayer(const std::string& peerId)
	{
		if(!peerId.empty())
		{
			size_t count=0;
			std::cout << "sending " << this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetItems().size() << " items to " << peerId << " on map " << this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetId() << std::endl;

			BOOST_FOREACH(ItemPair element,this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetItems())
			{
				Item item=element.second;
				if(item.GetId()!=0)
				{
					ItemOnMapUpdateResponseMessage response;
					response.SetName(item.GetName());
					response.SetId(item.GetId());
					response.SetModelId(item.GetModelId());
					response.SetIsEnteringMap(true);

					response.SetX(item.GetX());
					response.SetY(item.GetY());
					response.SetZ(item.GetZ());

					this->SendMessageToPeer(peerId,response);
					count++;
				}
			}

			std::cout << "sent " << count << " items to " << peerId << " on map " << this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetId() << std::endl;
		}
	}

	void ServerSessionManager::SendMonstersOnMapToPlayer(const std::string& peerId)
	{
		if(!peerId.empty())
		{
			size_t count=0;
			std::cout << "sending " << this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetMonsters().size() << " monsters to " << peerId << " on map " << this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetId() << std::endl;

			BOOST_FOREACH(MonsterPair element,this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetMonsters())
			{
				Monster monster=element.second;
				if(monster.GetId()!=0)
				{
					MonsterOnMapUpdateResponseMessage response;
					response.SetName(monster.GetName());
					response.SetId(monster.GetId());
					response.SetModelId(monster.GetModelId());
					response.SetIsEnteringMap(true);

					response.SetX(monster.GetX());
					response.SetY(monster.GetY());
					response.SetZ(monster.GetZ());

					this->SendMessageToPeer(peerId,response);
					count++;
				}
			}

			std::cout << "sent " << count << " monsters to " << peerId << " on map " << this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetId() << std::endl;
		}
	}

    void ServerSessionManager::SendServerMessageToPeer(const std::string& peerId,const std::string& message)
    {
        ChatResponseMessage response;
        response.SetChatType(CHAT_TYPE_SYSTEM);
        response.SetChatMessage(message);
        this->SendMessageToPeer(peerId,response);
    }

    void ServerSessionManager::SendServerMessageToParty(const std::string& peerId,const std::string& message)
    {
        ChatResponseMessage response;
        response.SetChatType(CHAT_TYPE_SYSTEM);
        response.SetChatMessage(message);
        this->SendMessageToParty(peerId,response);
    }

    void ServerSessionManager::SendServerMessageToGuild(const std::string& peerId,const std::string& message)
    {
        ChatResponseMessage response;
        response.SetChatType(CHAT_TYPE_SYSTEM);
        response.SetChatMessage(message);
        this->SendMessageToGuild(peerId,response);
    }

    void ServerSessionManager::SendServerMessageToMap(const std::string& peerId,const std::string& message)
    {
        ChatResponseMessage response;
        response.SetChatType(CHAT_TYPE_SYSTEM);
        response.SetChatMessage(message);
        this->SendMessageToMap(peerId,response);
    }

    void ServerSessionManager::SendServerMessageToWorld(const std::string& peerId,const std::string& message)
    {
        ChatResponseMessage response;
        response.SetChatType(CHAT_TYPE_SYSTEM);
        response.SetChatMessage(message);
        this->SendMessageToWorld(peerId,response);
    }

    std::string ServerSessionManager::GetUserByCharacterName(const std::string& name)
    {
        if(!name.empty())
        {
            BOOST_FOREACH(UserPair targetUser,this->mRealm.GetUsers())
            {
				if(targetUser.second.GetHasEnteredWorld())
				{
					#ifdef ENIGMA_PLATFORM_WINDOWS
					if(_stricmp(this->GetCurrentCharacter(targetUser.first).GetName().c_str(),name.c_str())==0)
					#else
					if(strcasecmp(this->GetCurrentCharacter(targetUser.first).GetName().c_str(),name.c_str())==0)
					#endif
					{
						return targetUser.first;
					}
				}
            }
        }

		return std::string("");
    }

    std::string ServerSessionManager::GetUserByCharacterId(size_t characterId)
    {
        if(characterId>0)
        {
            BOOST_FOREACH(UserPair targetUser,this->mRealm.GetUsers())
            {
				if(targetUser.second.GetCurrentCharacterId()==characterId)
				{
					return targetUser.first;
				}
            }
        }

        return std::string("");
    }

	Character& ServerSessionManager::GetCurrentCharacter(const std::string& peerId)
	{
		return this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()].GetCharacters()[this->mRealm.GetUsers()[peerId].GetCurrentCharacterId()];
	}

   Character& ServerSessionManager::GetCharacter(size_t mapId, size_t index)
    {
		return this->mRealm.GetMap(mapId).GetCharacters()[index];
    }

    void ServerSessionManager::CreateCharacter(size_t mapId, size_t index)
    {
        if(this->mRealm.GetMap(mapId).GetCharacters().find(index)!=this->mRealm.GetMap(mapId).GetCharacters().end())
        {
            this->DeleteCharacter(mapId,index);
        }
		Character character;
		//initialization
		this->mRealm.GetMap(mapId).GetCharacters()[index] = character;
    }

    void ServerSessionManager::DeleteCharacter(size_t mapId, size_t index)
    {
		if(this->mRealm.GetMap(mapId).GetCharacters().find(index)!=this->mRealm.GetMap(mapId).GetCharacters().end())
        {	
            this->mRealm.GetMap(mapId).GetCharacters().erase(index);
        }
    }

    Monster& ServerSessionManager::GetMonster(size_t mapId, size_t index)
    {
		return this->mRealm.GetMap(mapId).GetMonsters()[index];
    }

    void ServerSessionManager::CreateMonster(size_t mapId, size_t index)
    {
        if(this->mRealm.GetMap(mapId).GetMonsters().find(index)!=this->mRealm.GetMap(mapId).GetMonsters().end())
        {
            this->DeleteMonster(mapId,index);
        }
		Monster entity;
		//initialization
		this->mRealm.GetMap(mapId).GetMonsters()[index] = entity;
    }

    void ServerSessionManager::DeleteMonster(size_t mapId, size_t index)
    {
		if(this->mRealm.GetMap(mapId).GetMonsters().find(index)!=this->mRealm.GetMap(mapId).GetMonsters().end())
        {	
            this->mRealm.GetMap(mapId).GetMonsters().erase(index);
        }
    }

    Npc& ServerSessionManager::GetNpc(size_t mapId, const std::string& index)
    {
		return this->mRealm.GetMap(mapId).GetNpcs()[index];
    }

    Npc& ServerSessionManager::GetNpc(size_t mapId, size_t index)
    {
		return this->mRealm.GetMap(mapId).GetNpc(index);
    }

    void ServerSessionManager::CreateNpc(size_t mapId, const std::string& index)
    {
        if(this->mRealm.GetMap(mapId).GetNpcs().find(index)!=this->mRealm.GetMap(mapId).GetNpcs().end())
        {
            this->DeleteNpc(mapId,index);
        }
		Npc entity;
		//initialization
		this->mRealm.GetMap(mapId).GetNpcs()[index] = entity;
    }

    void ServerSessionManager::CreateNpc(size_t mapId, size_t index)
    {
        if(!this->mRealm.GetMap(mapId).FindNpc(index).empty())
        {
            this->DeleteNpc(mapId,index);
        }
		
		Npc entity;
		//initialization
		this->mRealm.GetMap(mapId).GetNpcs()[this->mRealm.GetMap(mapId).FindNpc(index)] = entity;
    }

    void ServerSessionManager::DeleteNpc(size_t mapId, const std::string& index)
    {
		if(this->mRealm.GetMap(mapId).GetNpcs().find(index)!=this->mRealm.GetMap(mapId).GetNpcs().end())
        {	
            this->mRealm.GetMap(mapId).GetNpcs().erase(index);
        }
    }

    void ServerSessionManager::DeleteNpc(size_t mapId, size_t index)
    {
		if(!this->mRealm.GetMap(mapId).FindNpc(index).empty())
        {	
            this->mRealm.GetMap(mapId).GetNpcs().erase(this->mRealm.GetMap(mapId).FindNpc(index));
        }
    }

    Item& ServerSessionManager::GetItem(size_t mapId, size_t index)
    {
        return this->mRealm.GetMap(mapId).GetItems()[index];
    }

    void ServerSessionManager::CreateItem(size_t mapId, size_t index)
    {
        if(this->mRealm.GetMap(mapId).GetItems().find(index)!=this->mRealm.GetMap(mapId).GetItems().end())
        {
            this->DeleteItem(mapId,index);
        }
		Item entity;
		//initialization
		this->mRealm.GetMap(mapId).GetItems()[index] = entity;
    }

    void ServerSessionManager::DeleteItem(size_t mapId, size_t index)
    {
		if(this->mRealm.GetMap(mapId).GetItems().find(index)!=this->mRealm.GetMap(mapId).GetItems().end())
        {	
            this->mRealm.GetMap(mapId).GetItems().erase(index);
        }
    }

	Guild& ServerSessionManager::GetCurrentGuild(const std::string& peerId)
	{
		return this->mRealm.GetGuilds()[this->GetCurrentCharacter(peerId).GetGuildId()];
	}

	Party& ServerSessionManager::GetCurrentParty(const std::string& peerId)
	{
		return this->mRealm.GetParties()[this->GetCurrentCharacter(peerId).GetPartyId()];
	}

	Rank& ServerSessionManager::GetCurrentRank(const std::string& peerId)
	{
		return this->GetCurrentGuild(peerId).GetRanks()[this->GetCurrentCharacter(peerId).GetRankName()];
	}

	bool ServerSessionManager::GetIsCurrentGuildLeader(const std::string& peerId)
	{
		return (this->GetCurrentCharacter(peerId).GetId() == this->GetCurrentGuild(peerId).GetLeaderId());
	}

	Npc& ServerSessionManager::GetCurrentNpc(const std::string& peerId)
	{
		return this->GetCurrentMap(peerId).GetNpc(this->GetCurrentCharacter(peerId).GetCurrentNpcId());
	}

	Map& ServerSessionManager::GetCurrentMap(const std::string& peerId)
	{
		return this->mRealm.GetMaps()[this->mRealm.GetUsers()[peerId].GetCurrentMapId()];
	}

	NpcQuestion& ServerSessionManager::GetCurrentNpcQuestion(const std::string& peerId)
	{
		std::string questionId = this->GetCurrentCharacter(peerId).GetNpcQuestionId();
		if(!questionId.empty())
		{
			return this->GetCurrentNpc(peerId).Questions[questionId];
		}
		else
		{
			return this->GetCurrentNpc(peerId).Questions[this->GetCurrentNpc(peerId).InitialQuestionId];
		}
	}

	std::map<size_t,Item>& ServerSessionManager::GetStash(const std::string& peerId)
	{
		return this->mRealm.GetUsers()[peerId].GetStash();
	}

	void ServerSessionManager::GivePlayerItem(const std::string& peerId,Item item)
    {
        this->GivePlayerItem(peerId,item,0);
    }

    void ServerSessionManager::GivePlayerItem(const std::string& peerId,Item item,size_t stackSize)
    {
        ItemTransferResponseMessage itemTransferResponseMessage;
        itemTransferResponseMessage.SetSourceLocation(ITEM_LOCATION_TYPE_NONE);
        itemTransferResponseMessage.SetSourceId(0);
        itemTransferResponseMessage.SetDestinationLocation(ITEM_LOCATION_TYPE_INVENTORY);
        itemTransferResponseMessage.SetDestinationId(0);
        itemTransferResponseMessage.SetStackSize(stackSize);

        this->GetCurrentCharacter(peerId).GetInventory().insert(
            std::pair<size_t,Item>(
                this->FindNextAvailableSlot(this->GetCurrentCharacter(peerId).GetInventory()),item));

        this->SendMessageToPeer(peerId,itemTransferResponseMessage);
    }

    void ServerSessionManager::TakePlayerItem(const std::string& peerId,size_t locationId)
    {
        this->TakePlayerItem(peerId,locationId,0);
    }

    void ServerSessionManager::TakePlayerItem(const std::string& peerId,size_t locationId,size_t stackSize)
    {
        ItemTransferResponseMessage itemTransferResponseMessage;
        itemTransferResponseMessage.SetSourceLocation(ITEM_LOCATION_TYPE_INVENTORY);
        itemTransferResponseMessage.SetSourceId(locationId);
        itemTransferResponseMessage.SetDestinationLocation(ITEM_LOCATION_TYPE_NONE);
        itemTransferResponseMessage.SetDestinationId(0);
        itemTransferResponseMessage.SetStackSize(stackSize);

        if(stackSize!=0)
        {

        }
        else
        {
            this->GetCurrentCharacter(peerId).GetInventory()[locationId] = Item();
        }

        this->SendMessageToPeer(peerId,itemTransferResponseMessage);
    }

    void ServerSessionManager::TransferPlayerItemToStash(const std::string& peerId,size_t locationId)
    {
        this->TransferPlayerItemToStash(peerId,locationId,0);
    }

    void ServerSessionManager::TransferPlayerItemToStash(const std::string& peerId,size_t locationId,size_t stackSize)
    {
        Item item;
        ItemTransferResponseMessage itemTransferResponseMessage;

        itemTransferResponseMessage.SetSourceLocation(ITEM_LOCATION_TYPE_INVENTORY);
        itemTransferResponseMessage.SetSourceId(locationId);
        itemTransferResponseMessage.SetDestinationLocation(ITEM_LOCATION_TYPE_STASH);
        itemTransferResponseMessage.SetDestinationId(0);
        itemTransferResponseMessage.SetStackSize(stackSize);

        if(stackSize!=0)
        {

        }
        else
        {
            item = this->GetCurrentCharacter(peerId).GetInventory()[locationId];
            this->GetCurrentCharacter(peerId).GetInventory()[locationId] = Item();
            this->GetStash(peerId).insert(std::pair<size_t,Item>(this->FindNextAvailableSlot(this->GetStash(peerId)),item));
        }


        this->SendMessageToPeer(peerId,itemTransferResponseMessage);
    }

    void ServerSessionManager::TransferPlayerItemFromStash(const std::string& peerId,size_t locationId)
    {
        this->TransferPlayerItemFromStash(peerId,locationId,0);
    }

    void ServerSessionManager::TransferPlayerItemFromStash(const std::string& peerId,size_t locationId,size_t stackSize)
    {
        Item item;
        ItemTransferResponseMessage itemTransferResponseMessage;

        itemTransferResponseMessage.SetSourceLocation(ITEM_LOCATION_TYPE_STASH);
        itemTransferResponseMessage.SetSourceId(locationId);
        itemTransferResponseMessage.SetDestinationLocation(ITEM_LOCATION_TYPE_INVENTORY);
        itemTransferResponseMessage.SetDestinationId(0);
        itemTransferResponseMessage.SetStackSize(stackSize);

        if(stackSize!=0)
        {

        }
        else
        {
            item = this->GetStash(peerId)[locationId];
            this->GetStash(peerId)[locationId] = Item();
            this->GetCurrentCharacter(peerId).GetInventory().insert(
                std::pair<size_t,Item>(
                    this->FindNextAvailableSlot(this->GetCurrentCharacter(peerId).GetInventory()),item));
        }


        this->SendMessageToPeer(peerId,itemTransferResponseMessage);
    }

    void ServerSessionManager::TransferPlayerItemToPlayer(const std::string& peerId,size_t locationId,size_t characterId)
    {
        this->TransferPlayerItemToPlayer(peerId,locationId,characterId,0);
    }

    void ServerSessionManager::TransferPlayerItemToPlayer(const std::string& peerId,size_t locationId,size_t characterId,size_t stackSize)
    {
        Item item;
        ItemTransferResponseMessage itemTransferResponseMessage;

        if(stackSize!=0)
        {
            size_t stackableItemIndex=0;
            item = this->GetCurrentCharacter(peerId).GetInventory()[locationId];

            if(item.GetId()==0)
            {
                //this is not a valid item.
                return;
            }

            stackableItemIndex = this->FindSlotWithItem(this->GetCurrentCharacter(peerId).GetInventory(),item.GetId());

            if(stackableItemIndex>0)
            {
                //the target has an item of this type so it can be reduced or removed.
                size_t newStackSize=0;
                newStackSize = this->GetCurrentCharacter(peerId).GetInventory()[locationId].GetStackSize()-stackSize;
                this->GetCurrentCharacter(peerId).GetInventory()[locationId].SetStackSize(newStackSize);
            }
            else
            {
                //this means the source doesn't have the item!
                return;
            }

            stackableItemIndex = this->FindSlotWithItem(this->GetCurrentMap(peerId).GetCharacters()[characterId].GetInventory(),item.GetId());

            if(stackableItemIndex>0)
            {
                //the target has an item of this type so it can be stacked.
                size_t newStackSize=0;
                newStackSize = this->GetCurrentMap(peerId).GetCharacters()[characterId].GetInventory()[stackableItemIndex].GetStackSize()+stackSize;
                this->GetCurrentMap(peerId).GetCharacters()[characterId].GetInventory()[stackableItemIndex].SetStackSize(newStackSize);
            }
            else
            {
                //the target doesn't have an item of this type so we must add a new stack.
                item.SetStackSize(stackSize);
                this->GetCurrentMap(peerId).GetCharacters()[characterId].GetInventory().insert(
                    std::pair<size_t,Item>(
                        this->FindNextAvailableSlot(this->GetCurrentMap(peerId).GetCharacters()[characterId].GetInventory()),item));
            }

        }
        else
        {
            item = this->GetCurrentCharacter(peerId).GetInventory()[locationId];
            this->GetCurrentCharacter(peerId).GetInventory()[locationId] = Item();
            this->GetCurrentMap(peerId).GetCharacters()[characterId].GetInventory().insert(
                std::pair<size_t,Item>(
                    this->FindNextAvailableSlot(this->GetCurrentMap(peerId).GetCharacters()[characterId].GetInventory()),item));
        }

        itemTransferResponseMessage.SetSourceLocation(ITEM_LOCATION_TYPE_INVENTORY);
        itemTransferResponseMessage.SetSourceId(locationId);
        itemTransferResponseMessage.SetDestinationLocation(ITEM_LOCATION_TYPE_PLAYER);
        itemTransferResponseMessage.SetDestinationId(0);
        itemTransferResponseMessage.SetStackSize(stackSize);
        this->SendMessageToPeer(peerId,itemTransferResponseMessage);

        itemTransferResponseMessage.SetSourceLocation(ITEM_LOCATION_TYPE_PLAYER);
        itemTransferResponseMessage.SetSourceId(0);
        itemTransferResponseMessage.SetDestinationLocation(ITEM_LOCATION_TYPE_INVENTORY);
        itemTransferResponseMessage.SetDestinationId(0);
        itemTransferResponseMessage.SetStackSize(stackSize);
        this->SendMessageToPeer(this->GetUserByCharacterId(characterId),itemTransferResponseMessage);

    }

    size_t ServerSessionManager::FindNextAvailableSlot(std::map<size_t,Item> itemCollection)
    {
        BOOST_FOREACH(ItemPair item,itemCollection)
        {
            if(item.second.GetId()==0)
            {
                return item.first;
            }
        }
        return 0;
    }

    size_t ServerSessionManager::FindSlotWithItem(std::map<size_t,Item> itemCollection,size_t itemId)
    {
        BOOST_FOREACH(ItemPair item,itemCollection)
        {
            if(item.second.GetId()==itemId)
            {
                return item.first;
            }
        }
        return 0;
    }
};