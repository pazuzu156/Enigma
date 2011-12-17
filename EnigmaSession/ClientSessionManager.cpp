/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "ClientSessionManager.hpp"
#include "GameMode.hpp"
#include <boost/regex.hpp>

namespace Enigma
{
    ClientSessionManager::ClientSessionManager()
		: mThreadBarrier(2)
    {

    }

    ClientSessionManager::~ClientSessionManager()
    {

    }

	void ClientSessionManager::Wait()
	{
		this->mThreadBarrier.wait();
	}

	void ClientSessionManager::DoApplicationEvents()
	{
		boost::shared_lock< boost::shared_mutex > lock(this->mRealmSnapshot.ApplicationMutex);
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
				case enigmaEvent.EVENT_APPLICATION_RECEIVED_CHARACTER_LIST:
						this->RaiseApplicationEventReceivedCharacterList(this->mAvailableCharacters);
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

	void ClientSessionManager::DoSceneEvents()
	{
		boost::shared_lock< boost::shared_mutex > lock(this->mRealmSnapshot.SceneMutex);
		bool isValid=false;
		EnigmaEvent enigmaEvent;

		enigmaEvent = this->PopSceneEvent(isValid);
		while(isValid)
		{
			switch(enigmaEvent.EventType)
			{
				case enigmaEvent.EVENT_SCENE_CHARACTER_ENTERED_MAP:
						this->RaiseSceneEventPlayerEnteredMap(enigmaEvent.EntityId,this->GetCharacter(enigmaEvent.EntityId));
					break;
				case enigmaEvent.EVENT_SCENE_CHARACTER_EXITED_MAP:
						this->RaiseSceneEventPlayerExitedMap(enigmaEvent.EntityId,this->GetCharacter(enigmaEvent.EntityId));
					break;
				case enigmaEvent.EVENT_SCENE_CHARACTER_UPDATED:
						this->RaiseSceneEventPlayerUpdate(enigmaEvent.EntityId,this->GetCharacter(enigmaEvent.EntityId));
					break;
				case enigmaEvent.EVENT_SCENE_PLAYER_MOVED:
						this->RaiseSceneEventPlayerMove(enigmaEvent.EntityId,this->GetCharacter(enigmaEvent.EntityId));
					break;
				case enigmaEvent.EVENT_SCENE_ITEM_ENTERED_MAP:
						this->RaiseSceneEventItemEnteredMap(enigmaEvent.EntityId,this->GetItem(enigmaEvent.EntityId));
					break;
				case enigmaEvent.EVENT_SCENE_ITEM_EXITED_MAP:
						this->RaiseSceneEventItemExitedMap(enigmaEvent.EntityId,this->GetItem(enigmaEvent.EntityId));
					break;
				case enigmaEvent.EVENT_SCENE_ITEM_UPDATED:
						this->RaiseSceneEventItemUpdate(enigmaEvent.EntityId,this->GetItem(enigmaEvent.EntityId));
					break;
				case enigmaEvent.EVENT_SCENE_MONSTER_ENTERED_MAP:
						this->RaiseSceneEventMonsterEnteredMap(enigmaEvent.EntityId,this->GetMonster(enigmaEvent.EntityId));
					break;
				case enigmaEvent.EVENT_SCENE_MONSTER_EXITED_MAP:
						this->RaiseSceneEventMonsterExitedMap(enigmaEvent.EntityId,this->GetMonster(enigmaEvent.EntityId));
					break;
				case enigmaEvent.EVENT_SCENE_MONSTER_UPDATED:
						this->RaiseSceneEventMonsterUpdate(enigmaEvent.EntityId,this->GetMonster(enigmaEvent.EntityId));
					break;
				case enigmaEvent.EVENT_SCENE_NPC_ENTERED_MAP:
						this->RaiseSceneEventNpcEnteredMap(enigmaEvent.EntityId,this->GetNpc(enigmaEvent.EntityId));
					break;
				case enigmaEvent.EVENT_SCENE_NPC_EXITED_MAP:
						this->RaiseSceneEventNpcExitedMap(enigmaEvent.EntityId,this->GetNpc(enigmaEvent.EntityId));
					break;
				case enigmaEvent.EVENT_SCENE_NPC_UPDATED:
						this->RaiseSceneEventNpcUpdate(enigmaEvent.EntityId,this->GetNpc(enigmaEvent.EntityId));
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

	void ClientSessionManager::DoChatEvents()
	{
		boost::shared_lock< boost::shared_mutex > lock(this->mRealmSnapshot.ChatMutex);
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

	void ClientSessionManager::DoAudioEvents()
	{
		boost::shared_lock< boost::shared_mutex > lock(this->mRealmSnapshot.SceneMutex);

		bool isValid=false;
		EnigmaEvent enigmaEvent;

		enigmaEvent = this->PopAudioEvent(isValid);
		while(isValid)
		{
			switch(enigmaEvent.EventType)
			{
				case enigmaEvent.EVENT_AUDIO_VOICE_RECEIVED:
					this->RaiseAudioEventVoiceReceived(enigmaEvent.EntityId,this->GetCharacter(enigmaEvent.EventType),(Enigma::u8*)enigmaEvent.UserData,enigmaEvent.Index);
					//clean up buffer.
					delete[] enigmaEvent.UserData;
					break;
				case enigmaEvent.EVENT_AUDIO_RECORDER_STATE_CHANGED:
						this->RaiseAudioEventRecorderStateChanged((bool)enigmaEvent.Index);
					break;
				case enigmaEvent.EVENT_AUDIO_PLAY_FILE:
					switch(enigmaEvent.ScopeId)
					{
						case TARGETPREFERENCETYPE_NONE:
								//do nothing but for the record this is bad.
							break;
						case TARGETPREFERENCETYPE_CHARACTER:
								this->RaiseAudioEventPlayFile(this->GetCharacter(enigmaEvent.EntityId),enigmaEvent.Index,enigmaEvent.Text);
							break;
						case TARGETPREFERENCETYPE_NPC:
								this->RaiseAudioEventPlayFile(this->GetNpc(enigmaEvent.EntityId),enigmaEvent.Index,enigmaEvent.Text);
							break;
						case TARGETPREFERENCETYPE_MONSTER:
								this->RaiseAudioEventPlayFile(this->GetMonster(enigmaEvent.EntityId),enigmaEvent.Index,enigmaEvent.Text);
							break;
						case TARGETPREFERENCETYPE_ITEM:
								this->RaiseAudioEventPlayFile(this->GetItem(enigmaEvent.EntityId),enigmaEvent.Index,enigmaEvent.Text);
							break;
						case TARGETPREFERENCETYPE_AREA:
								//this would actually work nicely but currently no way to get x,y,z.
							break;
						default:
								//do nothing but for hte record this is bad.
							break;
					}
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

	bool ClientSessionManager::Validate(bool testIsLoggedIn,bool testHasEnteredWorld)
	{
		boost::shared_lock< boost::shared_mutex > lock(this->mRealmSnapshot.ApplicationMutex);
		EnigmaEvent enigmaEvent;

		if(testIsLoggedIn && !this->mRealmSnapshot.GetUser().GetIsLoggedIn())
		{
			enigmaEvent = EnigmaEvent();
			enigmaEvent.EventType = EnigmaEvent::EVENT_CHAT_RECEIVED_MESSAGE;
			enigmaEvent.ScopeId = CHAT_TYPE_SYSTEM;
			enigmaEvent.Text = "You must be logged in to perform this action.";
			this->PushChatEvent(enigmaEvent);
			return false;
		}
		if(testHasEnteredWorld && !this->mRealmSnapshot.GetUser().GetHasEnteredWorld())
		{
			enigmaEvent = EnigmaEvent();
			enigmaEvent.EventType = EnigmaEvent::EVENT_CHAT_RECEIVED_MESSAGE;
			enigmaEvent.ScopeId = CHAT_TYPE_SYSTEM;
			enigmaEvent.Text = "You must have entered the world to perform this action";
			this->PushChatEvent(enigmaEvent);
			return false;
		}

		return true; //if it didn't fail then success.
	}

	bool ClientSessionManager::ProcessChatCommand(const std::string& text,Enigma::s32& commandDivider,Enigma::s32& targetDivider,std::string& command,std::string& target,std::string& message)
	{
		boost::cmatch what;
		Enigma::EnigmaEvent enigmaEvent;

		if(boost::regex_match(command.c_str(),what,boost::regex("[lL][oO][cC][aA][lL]")))
		{
			if(!this->Validate(true,true)) return true;

			message.append(text.substr(commandDivider+1,(text.size()-commandDivider)-1));

			ChatRequestMessage chatRequestMessage;
			chatRequestMessage.SetChatType(CHAT_TYPE_MAP);
			chatRequestMessage.SetChatMessage(message);

			this->mClientTransmissionManager->SendMessageToServer(chatRequestMessage);

			enigmaEvent = EnigmaEvent();
			enigmaEvent.EventType = EnigmaEvent::EVENT_CHAT_RECEIVED_MESSAGE;
			enigmaEvent.ScopeId = CHAT_TYPE_MAP;
			enigmaEvent.Text = message;
			enigmaEvent.Sender = "You";
			this->PushChatEvent(enigmaEvent);
		}
		else if(boost::regex_match(command.c_str(),what,boost::regex("[wW][hH][iI][sS][pP][eE][rR]|[wW]|[tT][eE][lL][lL]|[tT]")))
		{
			if(!this->Validate(true,true)) return true;

			targetDivider = text.find(' ',commandDivider+2);
			target.append(text.substr(commandDivider+1,(targetDivider-commandDivider)-1));
			message.append(text.substr(targetDivider+1,(text.size()-targetDivider)-1));

			ChatRequestMessage chatRequestMessage;
			chatRequestMessage.SetChatType(CHAT_TYPE_WHISPER);
			chatRequestMessage.SetChatTarget(target);
			chatRequestMessage.SetChatMessage(message);

			this->mClientTransmissionManager->SendMessageToServer(chatRequestMessage);

			enigmaEvent = EnigmaEvent();
			enigmaEvent.EventType = EnigmaEvent::EVENT_CHAT_RECEIVED_MESSAGE;
			enigmaEvent.ScopeId = CHAT_TYPE_WHISPER;
			enigmaEvent.Text = message;
			enigmaEvent.Sender = target;
			this->PushChatEvent(enigmaEvent);
		}
		else if(boost::regex_match(command.c_str(),what,boost::regex("[bB][rR][oO][aA][dD][cC][aA][sS][tT]|[bB]")))
		{
			if(!this->Validate(true,true)) return true;

			message.append(text.substr(commandDivider+1,(text.size()-commandDivider)-1));

			ChatRequestMessage chatRequestMessage;
			chatRequestMessage.SetChatType(CHAT_TYPE_BROADCAST);
			chatRequestMessage.SetChatMessage(message);

			this->mClientTransmissionManager->SendMessageToServer(chatRequestMessage);
		}
		else if(boost::regex_match(command.c_str(),what,boost::regex("[pP][aA][rR][tT][yY]|[pP]")))
		{
			if(!this->Validate(true,true)) return true;

			message.append(text.substr(commandDivider+1,(text.size()-commandDivider)-1));

			ChatRequestMessage chatRequestMessage;
			chatRequestMessage.SetChatType(CHAT_TYPE_PARTY);
			chatRequestMessage.SetChatMessage(message);

			this->mClientTransmissionManager->SendMessageToServer(chatRequestMessage);

			enigmaEvent = EnigmaEvent();
			enigmaEvent.EventType = EnigmaEvent::EVENT_CHAT_RECEIVED_MESSAGE;
			enigmaEvent.ScopeId = CHAT_TYPE_PARTY;
			enigmaEvent.Text = message;
			enigmaEvent.Sender = "You";
			this->PushChatEvent(enigmaEvent);
		}
		else if(boost::regex_match(command.c_str(),what,boost::regex("[gG][uU][iI][lL][dD]|[gG]")))
		{
			if(!this->Validate(true,true)) return true;

			message.append(text.substr(commandDivider+1,(text.size()-commandDivider)-1));

			ChatRequestMessage chatRequestMessage;
			chatRequestMessage.SetChatType(CHAT_TYPE_GUILD);
			chatRequestMessage.SetChatMessage(message);

			this->mClientTransmissionManager->SendMessageToServer(chatRequestMessage);

			enigmaEvent = EnigmaEvent();
			enigmaEvent.EventType = EnigmaEvent::EVENT_CHAT_RECEIVED_MESSAGE;
			enigmaEvent.ScopeId = CHAT_TYPE_GUILD;
			enigmaEvent.Text = message;
			enigmaEvent.Sender = "You";
			this->PushChatEvent(enigmaEvent);
		}
		else
		{
			return false;
		}

		return true;
	}

	bool ClientSessionManager::ProcessPlayerOrganizationCommand(const std::string& text,Enigma::s32& commandDivider,Enigma::s32& targetDivider,std::string& command,std::string& target,std::string& message)
	{
		boost::cmatch what;
		Enigma::EnigmaEvent enigmaEvent;

		if(boost::regex_match(command.c_str(),what,boost::regex("[iI][nN][vV][iI][tT][eE]|[pP][aA][rR][tT][yY][iI][nN][vV][iI][tT][eE]")))
		{
			if(!this->Validate(true,true)) return true;

			message.append(text.substr(commandDivider+1,(text.size()-commandDivider)-1));

			InviteRequestMessage inviteRequestMessage;
			inviteRequestMessage.SetTarget(message);
			inviteRequestMessage.SetInviteType(INVITE_TYPE_PARTY);

			this->mClientTransmissionManager->SendMessageToServer(inviteRequestMessage);
		}
		else if(boost::regex_match(command.c_str(),what,boost::regex("[gG][uU][iI][lL][dD][iI][nN][vV][iI][tT][eE]")))
		{
			if(!this->Validate(true,true)) return true;

			message.append(text.substr(commandDivider+1,(text.size()-commandDivider)-1));

			InviteRequestMessage inviteRequestMessage;
			inviteRequestMessage.SetTarget(message);
			inviteRequestMessage.SetInviteType(INVITE_TYPE_GUILD);

			this->mClientTransmissionManager->SendMessageToServer(inviteRequestMessage);
		}
		else if(boost::regex_match(command.c_str(),what,boost::regex("[kK][iI][cC][kK]|[pP][aA][rR][tT][yY][kK][iI][cC][kK]")))
		{
			if(!this->Validate(true,true)) return true;

			message.append(text.substr(commandDivider+1,(text.size()-commandDivider)-1));

			KickRequestMessage kickRequestMessage;
			kickRequestMessage.SetName(message);
			kickRequestMessage.SetKickType(KICK_TYPE_PARTY);

			this->mClientTransmissionManager->SendMessageToServer(kickRequestMessage);
		}
		else if(boost::regex_match(command.c_str(),what,boost::regex("[gG][uU][iI][lL][dD][kK][iI][cC][kK]")))
		{
			if(!this->Validate(true,true)) return true;

			message.append(text.substr(commandDivider+1,(text.size()-commandDivider)-1));

			KickRequestMessage kickRequestMessage;
			kickRequestMessage.SetName(message);
			kickRequestMessage.SetKickType(KICK_TYPE_GUILD);

			this->mClientTransmissionManager->SendMessageToServer(kickRequestMessage);
		}
		else if(boost::regex_match(command.c_str(),what,boost::regex("[lL][eE][aA][vV][eE]")))
		{
			if(!this->Validate(true,true)) return true;

			message.append(text.substr(commandDivider+1,(text.size()-commandDivider)-1));

			KickRequestMessage kickRequestMessage;
			kickRequestMessage.SetName("");

			if(boost::regex_match(message.c_str(),what,boost::regex("[gG][uU][iI][lL][dD]")))
			{
				kickRequestMessage.SetKickType(KICK_TYPE_GUILD);
			}
			else
			{
				kickRequestMessage.SetKickType(KICK_TYPE_PARTY);
			}

			this->mClientTransmissionManager->SendMessageToServer(kickRequestMessage);
		}
		else if(boost::regex_match(command.c_str(),what,boost::regex("[lL][eE][aA][vV][eE][pP][aA][rR][tT][yY]")))
		{
			if(!this->Validate(true,true)) return true;

			message.append(text.substr(commandDivider+1,(text.size()-commandDivider)-1));

			KickRequestMessage kickRequestMessage;
			kickRequestMessage.SetName("");
			kickRequestMessage.SetKickType(KICK_TYPE_PARTY);

			this->mClientTransmissionManager->SendMessageToServer(kickRequestMessage);
		}
		else if(boost::regex_match(command.c_str(),what,boost::regex("[lL][eE][aA][vV][eE][gG][uU][iI][lL][dD]")))
		{
			if(!this->Validate(true,true)) return true;

			message.append(text.substr(commandDivider+1,(text.size()-commandDivider)-1));

			KickRequestMessage kickRequestMessage;
			kickRequestMessage.SetName("");
			kickRequestMessage.SetKickType(KICK_TYPE_GUILD);

			this->mClientTransmissionManager->SendMessageToServer(kickRequestMessage);
		}
		else if(boost::regex_match(command.c_str(),what,boost::regex("[cC][rR][eE][aA][tT][eE]|[fF][oO][rR][mM]|[eE][sS][tT][aA][bB][lL][iI][sS][hH]")))
		{//create
			if(!this->Validate(true,true)) return true;

			targetDivider = text.find(' ',commandDivider+2);
			target.append(text.substr(commandDivider+1,(targetDivider-commandDivider)-1));
			message.append(text.substr(targetDivider+1,(text.size()-targetDivider)-1));

			if(target.length()>0)
			{
				if(boost::regex_match(target.c_str(),what,boost::regex("[gG][uU][iI][lL][dD]")))
				{
					CreatePlayerOrganizationRequestMessage createPlayerOrganizationRequestMessage;

					createPlayerOrganizationRequestMessage.SetName(message);
					createPlayerOrganizationRequestMessage.SetOrganizationType(ORGANIZATION_TYPE_GUILD);
					this->mClientTransmissionManager->SendMessageToServer(createPlayerOrganizationRequestMessage);
				}
				else if(boost::regex_match(target.c_str(),what,boost::regex("[pP][aA][rR][tT][yY]")))
				{
					CreatePlayerOrganizationRequestMessage createPlayerOrganizationRequestMessage;

					createPlayerOrganizationRequestMessage.SetName(message);
					createPlayerOrganizationRequestMessage.SetOrganizationType(ORGANIZATION_TYPE_PARTY);
					this->mClientTransmissionManager->SendMessageToServer(createPlayerOrganizationRequestMessage);
				}
				else
				{
					CreatePlayerOrganizationRequestMessage createPlayerOrganizationRequestMessage;

					createPlayerOrganizationRequestMessage.SetName(message);
					createPlayerOrganizationRequestMessage.SetOrganizationType(ORGANIZATION_TYPE_GUILD);
					this->mClientTransmissionManager->SendMessageToServer(createPlayerOrganizationRequestMessage);
				}
			}
			else
			{
				CreatePlayerOrganizationRequestMessage createPlayerOrganizationRequestMessage;

				createPlayerOrganizationRequestMessage.SetName(message);
				createPlayerOrganizationRequestMessage.SetOrganizationType(ORGANIZATION_TYPE_GUILD);
				this->mClientTransmissionManager->SendMessageToServer(createPlayerOrganizationRequestMessage);
			}
		}
		else if(boost::regex_match(command.c_str(),what,boost::regex("[cC][rR][eE][aA][tT][eE][gG][uU][iI][lL][dD][rR][aA][nN][kK]|[mM][oO][dD][iI][fF][yY][gG][uU][iI][lL][dD][rR][aA][nN][kK]")))
		{//createguildrank
			if(!this->Validate(true,true)) return true;

			targetDivider = text.find(' ',commandDivider+2);
			target.append(text.substr(commandDivider+1,(targetDivider-commandDivider)-1));
			message.append(text.substr(targetDivider+1,(text.size()-targetDivider)-1));

			if(target.length()>0)
			{
				if(message.length()>0 && boost::regex_match(message.c_str(),what,boost::regex("\\d\\d*")))
				{
					ModifyPlayerOrganizationRankRequestMessage modifyPlayerOrganizationRankRequestMessage;
					modifyPlayerOrganizationRankRequestMessage.SetName(target);
					modifyPlayerOrganizationRankRequestMessage.SetPermissions(boost::lexical_cast<Enigma::s32>(message));
					modifyPlayerOrganizationRankRequestMessage.SetOrganizationType(ORGANIZATION_TYPE_GUILD);
					this->mClientTransmissionManager->SendMessageToServer(modifyPlayerOrganizationRankRequestMessage);
				}
				else
				{
					enigmaEvent = EnigmaEvent();
					enigmaEvent.EventType = EnigmaEvent::EVENT_CHAT_RECEIVED_MESSAGE;
					enigmaEvent.ScopeId = CHAT_TYPE_SYSTEM;
					enigmaEvent.Text = "Permission mask required.";
					this->PushChatEvent(enigmaEvent);
				}
			}
			else
			{
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_CHAT_RECEIVED_MESSAGE;
				enigmaEvent.ScopeId = CHAT_TYPE_SYSTEM;
				enigmaEvent.Text = "Rank name required.";
				this->PushChatEvent(enigmaEvent);
			}
		}
		else if(boost::regex_match(command.c_str(),what,boost::regex("[aA][sS][sS][iI][gG][nN][rR][aA][nN][kK]|[aA][sS][sS][iI][gG][nN][gG][uU][iI][lL][dD][rR][aA][nN][kK]")))
		{//assignrank
			if(!this->Validate(true,true)) return true;

			targetDivider = text.find(' ',commandDivider+2);
			target.append(text.substr(commandDivider+1,(targetDivider-commandDivider)-1));
			message.append(text.substr(targetDivider+1,(text.size()-targetDivider)-1));

			if(message.length()>0 && target.length()>0)
			{
				SetPlayerRankRequestMessage setPlayerRankRequestMessage;
				setPlayerRankRequestMessage.SetPlayerName(target);
				setPlayerRankRequestMessage.SetRankName(message);
				setPlayerRankRequestMessage.SetOrganizationType(ORGANIZATION_TYPE_GUILD);
				setPlayerRankRequestMessage.Update();
				this->mClientTransmissionManager->SendMessageToServer(setPlayerRankRequestMessage);
			}
			else
			{
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_CHAT_RECEIVED_MESSAGE;
				enigmaEvent.ScopeId = CHAT_TYPE_SYSTEM;
				enigmaEvent.Text = "Player name and guild rank required.";
				this->PushChatEvent(enigmaEvent);
			}
		}
		else
		{
			return false;
		}

		return true;
	}

	bool ClientSessionManager::ProcessInformationCommand(const std::string& text,Enigma::s32& commandDivider,Enigma::s32& targetDivider,std::string& command,std::string& target,std::string& message)
	{
		boost::cmatch what;
		Enigma::EnigmaEvent enigmaEvent;

		if(boost::regex_match(command.c_str(),what,boost::regex("[bB][rR][oO][wW][sS][eE][rR]|[wW][eE][bB]")))
		{//browser web
			message.append(text.substr(commandDivider+1,(text.size()-commandDivider)-1));
			
			this->OpenBrowserWindow(message);
		}
		else if(boost::regex_match(command.c_str(),what,boost::regex("[aA][uU][dD][iI][oO]|[mM][uU][sS][iI][cC]")))
		{//video movie
			message.append(text.substr(commandDivider+1,(text.size()-commandDivider)-1));
			
			if(message.length()>0)
			{
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_AUDIO_PLAY_FILE;
				enigmaEvent.ScopeId = TARGETPREFERENCETYPE_CHARACTER;
				enigmaEvent.EntityId = this->GetCurrentCharacter().GetId();
				enigmaEvent.Index = 0; //audioid is nothing because we are passing filename.
				enigmaEvent.Text = message;
				this->PushAudioEvent(enigmaEvent);
			}		
		}
		else if(boost::regex_match(command.c_str(),what,boost::regex("[vV][iI][dD][eE][oO]|[mM][oO][vV][iI][eE]")))
		{//video movie
			message.append(text.substr(commandDivider+1,(text.size()-commandDivider)-1));
			
			this->OpenVideoWindow(message);
		}
		else if(boost::regex_match(command.c_str(),what,boost::regex("[cC][oO][mM][mM][aA][nN][dD][sS]")))
		{//commands
			message.append(text.substr(commandDivider+1,(text.size()-commandDivider)-1));

			this->OpenBrowserWindow(this->GetClientConfigurationManager().GetUrl("Commands"));
		}
		else if(boost::regex_match(command.c_str(),what,boost::regex("[sS][eE][aA][rR][cC][hH]")))
		{//search
			message.append(text.substr(commandDivider+1,(text.size()-commandDivider)-1));
			//TODO: revisit search
			//this->OpenBrowserWindow(this->mSearchProvider.CreateSearchString(message));
		}
		else if(boost::regex_match(command.c_str(),what,boost::regex("[tT][iI][mM][eE]|[wW][hH][eE][nN]")))
		{//time when
			ServerTimeRequestMessage serverTimeRequestMessage;
			this->mClientTransmissionManager->SendMessageToServer(serverTimeRequestMessage);
		}
		else if(boost::regex_match(command.c_str(),what,boost::regex("[gG][lL][oO][sS][sS][aA][rR][yY]")))
		{//glossary
			message.append(text.substr(commandDivider+1,(text.size()-commandDivider)-1));

			if(message.length()>0)
			{
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_CHAT_RECEIVED_MESSAGE;
				enigmaEvent.ScopeId = CHAT_TYPE_SYSTEM;
				enigmaEvent.Text = this->GetClientConfigurationManager().GetGlossaryEntry(message);
				this->PushChatEvent(enigmaEvent);
			}
		}
		else if(boost::regex_match(command.c_str(),what,boost::regex("[lL][oO][cC][aA][tT][iI][oO][nN]|[wW][hH][eE][rR][eE]")))
		{//location where
			if(!this->Validate(true,true)) return true;

			std::string location="Location: (";
			location.append(boost::lexical_cast<std::string>(this->GetCurrentCharacter().GetX()));
			location.append(",");
			location.append(boost::lexical_cast<std::string>(this->GetCurrentCharacter().GetY()));
			location.append(",");
			location.append(boost::lexical_cast<std::string>(this->GetCurrentCharacter().GetZ()));
			location.append(").");

			enigmaEvent = EnigmaEvent();
			enigmaEvent.EventType = EnigmaEvent::EVENT_CHAT_RECEIVED_MESSAGE;
			enigmaEvent.ScopeId = CHAT_TYPE_SYSTEM;
			enigmaEvent.Text = location;
			this->PushChatEvent(enigmaEvent);
		}
		else if(boost::regex_match(command.c_str(),what,boost::regex("[lL][iI][sS][tT]|[wW][hH][oO]")))
		{ //listwho
			if(!this->Validate(true,true)) return true;

			message.append(text.substr(commandDivider+1,(text.size()-commandDivider)-1));

			PlayerListRequestMessage playerListRequestMessage;
			if(message.length()>0)
			{
				if(boost::regex_match(message.c_str(),what,boost::regex("[aA][lL][lL]|[wW][oO][rR][lL][dD]")))
				{
					playerListRequestMessage.SetPlayerListType(LIST_TYPE_ALL);
				}
				else if(boost::regex_match(message.c_str(),what,boost::regex("[mM][aA][pP]")))
				{
					playerListRequestMessage.SetPlayerListType(LIST_TYPE_MAP);
				}
				else if(boost::regex_match(message.c_str(),what,boost::regex("[gG][uU][iI][lL][dD]")))
				{
					playerListRequestMessage.SetPlayerListType(LIST_TYPE_GUILD);
				}
				else if(boost::regex_match(message.c_str(),what,boost::regex("[pP][aA][rR][tT][yY]")))
				{
					playerListRequestMessage.SetPlayerListType(LIST_TYPE_PARTY);
				}
				else
				{
					playerListRequestMessage.SetPlayerListType(LIST_TYPE_ALL);
				}
			}
			else
			{
				playerListRequestMessage.SetPlayerListType(LIST_TYPE_ALL);
			}

			this->mClientTransmissionManager->SendMessageToServer(playerListRequestMessage);
		}
		else if(boost::regex_match(command.c_str(),what,boost::regex("[hH][eE][lL][pP]")))
		{//help
			message.append(text.substr(commandDivider+1,(text.size()-commandDivider)-1));

			if(boost::regex_match(message.c_str(),what,boost::regex("[cC][oO][mM][mM][aA][nN][dD][sS]")))
			{
				this->OpenBrowserWindow(this->GetClientConfigurationManager().GetUrl("Commands"));
			}
			else
			if(boost::regex_match(message.c_str(),what,boost::regex("[qQ][uU][eE][sS][tT][sS]|[qQ][uU][eE][sS][tT]")))
			{
				this->OpenBrowserWindow(this->GetClientConfigurationManager().GetUrl("Quests"));
			}
			else
			{
				this->OpenBrowserWindow(this->GetClientConfigurationManager().GetUrl("FAQs"));
			}
		}
		else
		{
			return false;
		}

		return true;
	}

	bool ClientSessionManager::ProcessWhimsicalCommand(const std::string& text,Enigma::s32& commandDivider,Enigma::s32& targetDivider,std::string& command,std::string& target,std::string& message)
	{
		boost::cmatch what;
		Enigma::EnigmaEvent enigmaEvent;

		if(boost::regex_match(command.c_str(),what,boost::regex("[sS][iI][tT]")))
		{//sit
			if(!this->Validate(true,true)) return true;

			//TODO:make player sit.
		}
		else if(boost::regex_match(command.c_str(),what,boost::regex("[sS][tT][aA][nN][dD]")))
		{//stand
			if(!this->Validate(true,true)) return true;

			//TODO:make player stand
		}
		else if(boost::regex_match(command.c_str(),what,boost::regex("[fF][aA][iI][lL]|[eE][pP][iI][cC][fF][aA][iI][lL]")))
		{//fail
			throw EnigmaException("You fail."); //this one is just for fun.
		}
		else if(boost::regex_match(command.c_str(),what,boost::regex("[nN][oO][tT][hH][iI][nN][gG]")))
		{//nothing
			enigmaEvent = EnigmaEvent();
			enigmaEvent.EventType = EnigmaEvent::EVENT_CHAT_RECEIVED_MESSAGE;
			enigmaEvent.ScopeId = CHAT_TYPE_SYSTEM;
			enigmaEvent.Text = "Nothing to see here.";
			this->PushChatEvent(enigmaEvent);
		}
		else if(boost::regex_match(command.c_str(),what,boost::regex("[eE][mM][oO][tT][eE]")))
		{//emote
			if(!this->Validate(true,true)) return true;

			message.append(text.substr(commandDivider+1,(text.size()-commandDivider)-1));

			/*if(boost::regex_match(message.c_str(),what,boost::regex("[jJ][uU][mM][pP]")))
			{
				this->mCameraSceneNodeAnimator->PlayJumpAnimation();
			}
			else if(boost::regex_match(message.c_str(),what,boost::regex("[bB][aA][cC][kK][fF][lL][iI][pP]")))
			{
				this->mCameraSceneNodeAnimator->PlayBackflip();
			}
			else if(boost::regex_match(message.c_str(),what,boost::regex("[wW][hH][iI][sS][tT][lL][eE]")))
			{
				this->mCameraSceneNodeAnimator->PlayWhistle();
			}
			else if(boost::regex_match(message.c_str(),what,boost::regex("[lL][aA][uU][gG][hH]")))
			{
				this->mCameraSceneNodeAnimator->PlayLaugh();
			}
			else if(boost::regex_match(message.c_str(),what,boost::regex("[pP][oO][iI][nN][tT]")))
			{
				this->mCameraSceneNodeAnimator->PlayPoint();
			}
			else if(boost::regex_match(message.c_str(),what,boost::regex("[hH][iI][gG][hH][fF][iI][vV][eE]")))
			{
				this->mCameraSceneNodeAnimator->PlayHighFive();
			}*/
		}
		else if(boost::regex_match(command.c_str(),what,boost::regex("[rR][oO][lL][lL]|[rR][aA][nN][dD][oO][mM]")))
		{//roll random
			if(!this->Validate(true,true)) return true;

			message.append(text.substr(commandDivider+1,(text.size()-commandDivider)-1));

			if(message.length()>0)
			{
				RollRequestMessage rollRequestMessage;
				rollRequestMessage.SetRollLimit(message);
				this->mClientTransmissionManager->SendMessageToServer(rollRequestMessage);
			}
			else
			{
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_CHAT_RECEIVED_MESSAGE;
				enigmaEvent.ScopeId = CHAT_TYPE_SYSTEM;
				enigmaEvent.Text = "Roll limit number required.";
				this->PushChatEvent(enigmaEvent);
			}
		}
		else
		{
			return false;
		}

		return true;
	}

	bool ClientSessionManager::ProcessCombatCommand(const std::string& text,Enigma::s32& commandDivider,Enigma::s32& targetDivider,std::string& command,std::string& target,std::string& message)
	{
		boost::cmatch what;
		Enigma::EnigmaEvent enigmaEvent;

		if(boost::regex_match(command.c_str(),what,boost::regex("[sS][kK][iI][lL][lL]|[cC][aA][sS][tT]")))
		{//skillcast
			if(!this->Validate(true,true)) return true;

			targetDivider = text.find(' ',commandDivider+2);
			target.append(text.substr(commandDivider+1,(targetDivider-commandDivider)-1));
			message.append(text.substr(targetDivider+1,(text.size()-targetDivider)-1));

			this->RequestUseSkill(boost::lexical_cast<Enigma::s32>(target),boost::lexical_cast<Enigma::s32>(message));
		}
		else
		{
			return false;
		}

		return true;
	}

	bool ClientSessionManager::ProcessStateChangeCommand(const std::string& text,Enigma::s32& commandDivider,Enigma::s32& targetDivider,std::string& command,std::string& target,std::string& message)
	{
        boost::cmatch what;
		Enigma::EnigmaEvent enigmaEvent;

		if(boost::regex_match(command.c_str(),what,boost::regex("[lL][oO][gG][iI][nN]|[sS][iI][gG][nN]\\-[iI][nN]")))
		{//login sign-in
			targetDivider = text.find(' ',commandDivider+2);
			target.append(text.substr(commandDivider+1,(targetDivider-commandDivider)-1));
			message.append(text.substr(targetDivider+1,(text.size()-targetDivider)-1));

			this->Login(target,message);
		}
		else if(boost::regex_match(command.c_str(),what,boost::regex("[cC][rR][eE][aA][tT][eE][cC][hH][aA][rR][aA][cC][tT][eE][rR]")))
		{//createcharacter
			message.append(text.substr(commandDivider+1,(text.size()-commandDivider)-1));

			this->RequestCreateCharacter(message);
		}
		else if(boost::regex_match(command.c_str(),what,boost::regex("[rR][eE][qQ][uU][eE][sS][tT][cC][hH][aA][rR][aA][cC][tT][eE][rR][lL][iI][sS][tT]")))
		{//requestcharacterlist
			if(!this->Validate(true,false)) return true;

			//message.append(text.substr(commandDivider+1,(text.size()-commandDivider)-1));

			this->RequestCharacterList();
		}
		else if(boost::regex_match(command.c_str(),what,boost::regex("[sS][hH][oO][wW][cC][hH][aA][rR][aA][cC][tT][eE][rR][lL][iI][sS][tT]")))
		{//showcharacterlist
			if(!this->Validate(true,false)) return true;

			std::stringstream s;
			int i=0;
			BOOST_FOREACH(std::string character,this->mAvailableCharacters)
			{
				s << "(" << i << ") " << character << std::endl;
				i++;
			}
			this->Log(s);

			//this->RequestCharacterList();
		}
		else if(boost::regex_match(command.c_str(),what,boost::regex("[sS][eE][lL][eE][cC][tT][cC][hH][aA][rR][aA][cC][tT][eE][rR]")))
		{//selectcharacter
			if(!this->Validate(true,false)) return true;

			message.append(text.substr(commandDivider+1,(text.size()-commandDivider)-1));

			this->SelectCharacter(boost::lexical_cast<int>(message));
		}
		else if(boost::regex_match(command.c_str(),what,boost::regex("[cC][aA][mM][pP]")))
		{//camp
			if(!this->Validate(true,true)) return true;

			this->RequestCharacterList(); //Requesting character list causes a user to be removed from the world.
		}
		else if(boost::regex_match(command.c_str(),what,boost::regex("[lL][oO][gG][oO][uU][tT]|[sS][iI][gG][nN][oO][uU][tT]")))
		{//logout signout
			this->Logout(); //validation inside login method.
		}
		else if(boost::regex_match(command.c_str(),what,boost::regex("[eE][xX][iI][tT]|[qQ][uU][iI][tT]|[cC][lL][oO][sS][eE]")))
		{//exit
			enigmaEvent = EnigmaEvent();
			enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_EXIT;
			this->PushApplicationEvent(enigmaEvent);
		}
		else if(boost::regex_match(command.c_str(),what,boost::regex("[wW][aA][rR][pP]|[cC][hH][aA][nN][gG][eE][mM][aA][pP]")))
		{//warp changemap
			this->RequestMapChange(boost::lexical_cast<size_t>(message));
		}
		else if(boost::regex_match(command.c_str(),what,boost::regex("[gG][aA][mM][eE][mM][oO][dD][eE]")))
		{//gamemode
			if(this->mRealmSnapshot.GetUser().GetHasEnteredWorld())
			{
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_CHAT_RECEIVED_MESSAGE;
				enigmaEvent.ScopeId = CHAT_TYPE_SYSTEM;
				enigmaEvent.Text = "Character is in the world.";
				this->PushChatEvent(enigmaEvent);
			}
			else
			{
				if(this->mRealmSnapshot.GetUser().GetIsLoggedIn())
				{
					enigmaEvent = EnigmaEvent();
					enigmaEvent.EventType = EnigmaEvent::EVENT_CHAT_RECEIVED_MESSAGE;
					enigmaEvent.ScopeId = CHAT_TYPE_SYSTEM;
					enigmaEvent.Text = "User is signed in.";
					this->PushChatEvent(enigmaEvent);
				}
				else
				{
					enigmaEvent = EnigmaEvent();
					enigmaEvent.EventType = EnigmaEvent::EVENT_CHAT_RECEIVED_MESSAGE;
					enigmaEvent.ScopeId = CHAT_TYPE_SYSTEM;
					enigmaEvent.Text = "Waiting for login.";
					this->PushChatEvent(enigmaEvent);
				}
			}
		}
		else
		{
			return false;
		}

		return true;
	}

	void ClientSessionManager::ProcessCommand(const std::string& text,size_t chatType)
	{
        boost::cmatch what;
		Enigma::EnigmaEvent enigmaEvent;
		Enigma::s32 commandDivider=0;
        Enigma::s32 targetDivider=0;
        std::string command="";
        std::string target="";
        std::string message="";

		if(text.empty())
		{
			//If this is a console client the user may simply be trying to get the latest events.
			return; //nothing to do but not really an error.
		}

		if(text[0]=='/' || text[0]=='\\' || text[0]=='-')
		{
			commandDivider = text.find(' ',1);
			command.append(text.substr(1,commandDivider-1));

			if(ProcessChatCommand(text,commandDivider,targetDivider,command,target,message))
			{
				//Do nothing
			}
			else if(ProcessPlayerOrganizationCommand(text,commandDivider,targetDivider,command,target,message))
			{
				//Do nothing
			}
			else if(ProcessStateChangeCommand(text,commandDivider,targetDivider,command,target,message))
			{
				//Do nothing
			}
			else if(ProcessInformationCommand(text,commandDivider,targetDivider,command,target,message))
			{
				//Do nothing
			}
			else if(ProcessCombatCommand(text,commandDivider,targetDivider,command,target,message))
			{
				//Do nothing
			}
			else if(ProcessWhimsicalCommand(text,commandDivider,targetDivider,command,target,message))
			{
				//Do nothing
			}
			else if(boost::regex_match(command.c_str(),what,boost::regex("[rR][eE][lL][oO][aA][dD][uU][iI]")))
			{
				//this->ReloadUI();
			}
			else if(boost::regex_match(command.c_str(),what,boost::regex("[mM][oO][vV][eE]")))
			{
				std::string x;
				std::string y;
				std::string z;
				Enigma::s32 xDivider=0;
				Enigma::s32 yDivider=0;
				Enigma::s32 zDivider=0;

				message.append(text.substr(commandDivider+1,(text.size()-commandDivider)-1));
				
				xDivider = message.find(',',0);
				yDivider = message.find(',',xDivider+1);
				zDivider = message.find(',',yDivider+1);

				x.append(message.substr(0,xDivider));
				y.append(message.substr(xDivider+1,(message.size()-yDivider)-1));
				z.append(message.substr(yDivider+1,(message.size()-zDivider)-1));

				this->MovePlayer(boost::lexical_cast<Enigma::f32>(x),boost::lexical_cast<Enigma::f32>(y),boost::lexical_cast<Enigma::f32>(z));
			}
			else
			{
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_CHAT_RECEIVED_MESSAGE;
				enigmaEvent.ScopeId = CHAT_TYPE_SYSTEM;
				enigmaEvent.Text = "Unknown Command!";
				this->PushChatEvent(enigmaEvent);
			}
		}
		else
		{
			if(!this->Validate(true,true)) return;

			ChatRequestMessage chatRequestMessage;
			chatRequestMessage.SetChatType(chatType);
			chatRequestMessage.SetChatMessage(text);

			this->mClientTransmissionManager->SendMessageToServer(chatRequestMessage);

			if(chatType!=CHAT_TYPE_BROADCAST)
			{
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_CHAT_RECEIVED_MESSAGE;
				enigmaEvent.ScopeId = chatType;
				enigmaEvent.Text = text;
				enigmaEvent.Sender = "You";
				this->PushChatEvent(enigmaEvent);
			}
		}
	}

	Character& ClientSessionManager::GetCurrentCharacter()
	{
		return this->mRealmSnapshot.GetMap().GetCharacters()[this->mRealmSnapshot.GetUser().GetCurrentCharacterId()];
	}

    Character& ClientSessionManager::GetCharacter(size_t index)
    {
		return this->mRealmSnapshot.GetMap().GetCharacters()[index];
    }

    void ClientSessionManager::CreateCharacter(size_t index)
    {
        if(this->mRealmSnapshot.GetMap().GetCharacters().find(index)!=this->mRealmSnapshot.GetMap().GetCharacters().end())
        {
            this->DeleteCharacter(index);
        }
		Character character;
		//initialization
		this->mRealmSnapshot.GetMap().GetCharacters()[index] = character;
    }

    void ClientSessionManager::DeleteCharacter(size_t index)
    {
		if(this->mRealmSnapshot.GetMap().GetCharacters().find(index)!=this->mRealmSnapshot.GetMap().GetCharacters().end())
        {	
            this->mRealmSnapshot.GetMap().GetCharacters().erase(index);
        }
    }

    Monster& ClientSessionManager::GetMonster(size_t index)
    {
		return this->mRealmSnapshot.GetMap().GetMonsters()[index];
    }

    void ClientSessionManager::CreateMonster(size_t index)
    {
        if(this->mRealmSnapshot.GetMap().GetMonsters().find(index)!=this->mRealmSnapshot.GetMap().GetMonsters().end())
        {
            this->DeleteMonster(index);
        }
		Monster entity;
		//initialization
		this->mRealmSnapshot.GetMap().GetMonsters()[index] = entity;
    }

    void ClientSessionManager::DeleteMonster(size_t index)
    {
		if(this->mRealmSnapshot.GetMap().GetMonsters().find(index)!=this->mRealmSnapshot.GetMap().GetMonsters().end())
        {	
            this->mRealmSnapshot.GetMap().GetMonsters().erase(index);
        }
    }

    Npc& ClientSessionManager::GetNpc(const std::string& index)
    {
		return this->mRealmSnapshot.GetMap().GetNpcs()[index];
    }

    Npc& ClientSessionManager::GetNpc(size_t index)
    {
		return this->mRealmSnapshot.GetMap().GetNpc(index);
    }

    void ClientSessionManager::CreateNpc(const std::string& index)
    {
        if(this->mRealmSnapshot.GetMap().GetNpcs().find(index)!=this->mRealmSnapshot.GetMap().GetNpcs().end())
        {
            this->DeleteNpc(index);
        }
		Npc entity;
		//initialization
		this->mRealmSnapshot.GetMap().GetNpcs()[index] = entity;
    }

    void ClientSessionManager::CreateNpc(size_t index)
    {
        if(!this->mRealmSnapshot.GetMap().FindNpc(index).empty())
        {
            this->DeleteNpc(index);
        }
		
		Npc entity;
		//initialization
		this->mRealmSnapshot.GetMap().GetNpcs()[this->mRealmSnapshot.GetMap().FindNpc(index)] = entity;
    }

    void ClientSessionManager::DeleteNpc(const std::string& index)
    {
		if(this->mRealmSnapshot.GetMap().GetNpcs().find(index)!=this->mRealmSnapshot.GetMap().GetNpcs().end())
        {	
            this->mRealmSnapshot.GetMap().GetNpcs().erase(index);
        }
    }

    void ClientSessionManager::DeleteNpc(size_t index)
    {
		if(!this->mRealmSnapshot.GetMap().FindNpc(index).empty())
        {	
            this->mRealmSnapshot.GetMap().GetNpcs().erase(this->mRealmSnapshot.GetMap().FindNpc(index));
        }
    }

    Item& ClientSessionManager::GetItem(size_t index)
    {
        return this->mRealmSnapshot.GetMap().GetItems()[index];
    }

    void ClientSessionManager::CreateItem(size_t index)
    {
        if(this->mRealmSnapshot.GetMap().GetItems().find(index)!=this->mRealmSnapshot.GetMap().GetItems().end())
        {
            this->DeleteItem(index);
        }
		Item entity;
		//initialization
		this->mRealmSnapshot.GetMap().GetItems()[index] = entity;
    }

    void ClientSessionManager::DeleteItem(size_t index)
    {
		if(this->mRealmSnapshot.GetMap().GetItems().find(index)!=this->mRealmSnapshot.GetMap().GetItems().end())
        {	
            this->mRealmSnapshot.GetMap().GetItems().erase(index);
        }
    }

	void ClientSessionManager::Logout()
	{
		EnigmaEvent enigmaEvent;
		if(!this->Validate(true,false)) return;

		this->mClientTransmissionManager->Reconnect(); //causes the session to be destroyed and a new one created.		
	}

	void ClientSessionManager::onDisconnected()
	{
		this->mRealmSnapshot.Clear(); //this has its own locks.
		
		EnigmaEvent enigmaEvent;
		enigmaEvent = EnigmaEvent();
		enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_GAME_MODE_CHANGED;
		enigmaEvent.Index = GAME_MODE_LOGIN;
		this->PushApplicationEvent(enigmaEvent);
	}

    void ClientSessionManager::Login(const std::string& username,const std::string& password)
    {
		EnigmaEvent enigmaEvent;

		if(this->mRealmSnapshot.GetUser().GetIsLoggedIn())
		{
			enigmaEvent = EnigmaEvent();
			enigmaEvent.EventType = EnigmaEvent::EVENT_CHAT_RECEIVED_MESSAGE;
			enigmaEvent.ScopeId = CHAT_TYPE_SYSTEM;
			enigmaEvent.Text = "You are already logged in.";
			this->PushChatEvent(enigmaEvent);
			return;
		}

		Enigma::LoginRequestMessage loginRequestMessage(username,password);

		this->mClientTransmissionManager->SendMessageToServer(loginRequestMessage);
    }

    void ClientSessionManager::SelectCharacter(int index)
    {
		boost::unique_lock< boost::shared_mutex > lock(this->mRealmSnapshot.ApplicationMutex);
		EnigmaEvent enigmaEvent;

		if(!this->mRealmSnapshot.GetUser().GetIsLoggedIn())
		{
			enigmaEvent = EnigmaEvent();
			enigmaEvent.EventType = EnigmaEvent::EVENT_CHAT_RECEIVED_MESSAGE;
			enigmaEvent.ScopeId = CHAT_TYPE_SYSTEM;
			enigmaEvent.Text = "You must be logged in to create select a character.";
			this->PushChatEvent(enigmaEvent);
			return;
		}

		CharacterSelectionRequestMessage CharacterSelectionRequestMessage;

		CharacterSelectionRequestMessage.SetCharacterIndex(index);

		this->mClientTransmissionManager->SendMessageToServer(CharacterSelectionRequestMessage);

		this->mAvailableCharacters.clear(); //otherwise they will just sit there and waste memory.
    }

    void ClientSessionManager::RequestCharacterList(bool shouldLock)
    {
		if(shouldLock)
		{
			boost::unique_lock< boost::shared_mutex > lock(this->mRealmSnapshot.ApplicationMutex);
		}
		EnigmaEvent enigmaEvent;

		if(!this->mRealmSnapshot.GetUser().GetIsLoggedIn())
		{
			enigmaEvent = EnigmaEvent();
			enigmaEvent.EventType = EnigmaEvent::EVENT_CHAT_RECEIVED_MESSAGE;
			enigmaEvent.ScopeId = CHAT_TYPE_SYSTEM;
			enigmaEvent.Text = "You must be logged in to request a character list.";
			this->PushChatEvent(enigmaEvent);
			return;
		}

		this->mAvailableCharacters.clear();

		CharacterListRequestMessage characterListRequestMessage;

		this->mClientTransmissionManager->SendMessageToServer(characterListRequestMessage);

		if(this->mRealmSnapshot.GetUser().GetHasEnteredWorld())
		{
			this->mRealmSnapshot.GetUser().SetHasEnteredWorld(false);
		}

		enigmaEvent = EnigmaEvent();
		enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_GAME_MODE_CHANGED;
		enigmaEvent.Index = GAME_MODE_CHARACTER_SELECT;
		this->PushApplicationEvent(enigmaEvent);
    }

	void ClientSessionManager::RequestCreateCharacter(const std::string& name)
    {
		if(!this->Validate(true,false)) return;
		if(name.length()==0) return;
		
		Enigma::CharacterCreationRequestMessage characterCreationRequestMessage;
		characterCreationRequestMessage.SetName(name);

		this->mClientTransmissionManager->SendMessageToServer(characterCreationRequestMessage);
    }

	void ClientSessionManager::RequestUseSkill(Enigma::s32 skillId,Enigma::s32 skillLevel)
	{
		if(!this->Validate(true,true)) return;

		boost::shared_lock< boost::shared_mutex > lock1(this->mRealmSnapshot.ApplicationMutex);
		boost::shared_lock< boost::shared_mutex > lock2(this->mRealmSnapshot.SceneMutex);

		SkillRequestMessage skillRequestMessage;
		
		//This is cheating but id's can be replaced with vector when area is selected.
		skillRequestMessage.SetTargetMonsterId(this->GetCurrentCharacter().GetTarget().GetMonsterTarget());
		skillRequestMessage.SetTargetNpcId(this->GetCurrentCharacter().GetTarget().GetNpcTarget());
		skillRequestMessage.SetTargetCharacterId(this->GetCurrentCharacter().GetTarget().GetCharacterTarget());		
		skillRequestMessage.SetTargetPreferenceType(this->GetCurrentCharacter().GetTarget().GetTargetPreference());
		skillRequestMessage.SetSkillId(skillId);
		skillRequestMessage.SetSkillLevel(skillLevel);

		this->mClientTransmissionManager->SendMessageToServer(skillRequestMessage);
	}

	void ClientSessionManager::RequestMapChange(size_t index)
	{
		if(!this->Validate(true,true)) return;

		ChangeMapRequestMessage changeMapRequestMessage;
		changeMapRequestMessage.SetTargetMap(index);
		this->mClientTransmissionManager->SendMessageToServer(changeMapRequestMessage);
	}

    void ClientSessionManager::MovePlayer(Enigma::f32 x1,Enigma::f32 y1, Enigma::f32 z1,Enigma::f32 x2,Enigma::f32 y2, Enigma::f32 z2)
    {
		if(!this->Validate(true,true)) return;

        MovementRequestMessage movementRequestMessage(x1,y1,z1,x2,y2,z2);
        this->mClientTransmissionManager->SendMessageToServer(movementRequestMessage);

		boost::unique_lock< boost::shared_mutex > lock(this->mRealmSnapshot.SceneMutex); //lock after message is sent to reduce change of deadlock.

		this->GetCurrentCharacter().SetX(x1);
		this->GetCurrentCharacter().SetY(y1);
		this->GetCurrentCharacter().SetZ(z1);

		//TODO: Add support for rotation.

		EnigmaEvent enigmaEvent;

		enigmaEvent = EnigmaEvent();
		enigmaEvent.EventType = EnigmaEvent::EVENT_SCENE_PLAYER_MOVED;
		enigmaEvent.EntityId = this->GetCurrentCharacter().GetId();
		this->PushSceneEvent(enigmaEvent);
    }

    void ClientSessionManager::MovePlayer(Enigma::f32 x1,Enigma::f32 y1, Enigma::f32 z1)
    {
		this->MovePlayer(x1,y1,z1,0,0,0);
    }

    void ClientSessionManager::RequestPlayersOnMap()
    {
        PlayerOnMapUpdateRequestMessage request;
        this->mClientTransmissionManager->SendMessageToServer(request);
    }

    void ClientSessionManager::RequestNpcsOnMap()
    {
        NpcOnMapUpdateRequestMessage request;
        this->mClientTransmissionManager->SendMessageToServer(request);
    }

    void ClientSessionManager::RequestMonstersOnMap()
    {
        MonsterOnMapUpdateRequestMessage request;
        this->mClientTransmissionManager->SendMessageToServer(request);
    }

    void ClientSessionManager::RequestItemsOnMap()
    {
        ItemOnMapUpdateRequestMessage request;
        this->mClientTransmissionManager->SendMessageToServer(request);
    }

	void ClientSessionManager::RespondToInvite(bool accepted,size_t inviteId,size_t inviteType)
	{
		InviteRequestMessage request;
		request.SetTarget("");
		request.SetInviteId(inviteId);
		request.SetInviteType(inviteType);

		if(accepted)
		{
			request.SetStatus(STATUS_OK);
		}
		else
		{
			request.SetStatus(STATUS_UNKNOWN_FAIL);
		}

		this->mClientTransmissionManager->SendMessageToServer(request);
	}

	void ClientSessionManager::RespondToGuildInvite(bool accepted,size_t inviteId)
	{
		this->RespondToInvite(accepted,inviteId,CHAT_TYPE_GUILD);
	}

	void ClientSessionManager::RespondToPartyInvite(bool accepted,size_t inviteId)
	{
		this->RespondToInvite(accepted,inviteId,CHAT_TYPE_PARTY);
	}

	void ClientSessionManager::SendVoiceData(Enigma::u8* inputBuffer,size_t inputSize)
	{
		boost::shared_lock< boost::shared_mutex > lock2(this->mRealmSnapshot.ApplicationMutex);

		Enigma::VoiceChatRequestMessage voiceChatRequestMessage;
		voiceChatRequestMessage.SetCombatantId(this->GetCurrentCharacter().GetId());
		voiceChatRequestMessage.SetVoiceChannel(this->mRealmSnapshot.GetUser().GetCurrentVoiceChannel());
		voiceChatRequestMessage.SetAudioData(inputBuffer,inputSize);

		this->mClientTransmissionManager->SendMessageToServer(voiceChatRequestMessage);
	}

//Authentication Start
    void ClientSessionManager::ProcessMessage(LoginResponseMessage& message)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mRealmSnapshot.ApplicationMutex);
		EnigmaEvent enigmaEvent;

		switch(message.GetStatus())
		{
			case STATUS_OK:
				this->mRealmSnapshot.GetUser().SetIsLoggedIn(true);

				this->RequestCharacterList(false); //this changes game mode and gets needed character list.
			break;
			case STATUS_UNKNOWN_FAIL:
				//unknown failure.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "This operation could not be performed because of an unknown failure.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_SECURITY_FAIL:
				//operation not permitted.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				//enigmaEvent.Text = "This operation is not permitted!";
				enigmaEvent.Text = "The username or password you provided does not match our records.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_DATABASE_FAIL:
			//failed to contact database.
				throw EnigmaException("Due to a database outage our service is currently unavailable, we apologize for the inconvenience.");
			break;
			case STATUS_NETWORK_FAIL:
			//request timed out.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "Due to network conditions this message was not recieved by the server in a timely manner.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_VERSION_FAIL:
			//version mismatch.
				throw EnigmaException("The protocol version of the client & server do not match and this session must end.");
			break;
			default:
			//unknown this is bad.
				throw EnigmaException("Unknown failure type, please ensure your client is up to date.");
			break;
		}
	}

    void ClientSessionManager::ProcessMessage(CharacterCreationResponseMessage& message)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mRealmSnapshot.ApplicationMutex);
		EnigmaEvent enigmaEvent;

		switch(message.GetStatus())
		{
			case STATUS_OK:
				this->mRealmSnapshot.GetUser().SetHasEnteredWorld(true);

				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_GAME_MODE_CHANGED;
				enigmaEvent.Index = GAME_MODE_IN_GAME;
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_UNKNOWN_FAIL:
				//unknown failure.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "This operation could not be performed because of an unknown failure.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_SECURITY_FAIL:
				//operation not permitted.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "This operation is not permitted!";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_DATABASE_FAIL:
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "Character name is not allowed or is already in use.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_NETWORK_FAIL:
			//request timed out.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "Due to network conditions this message was not recieved by the server in a timely manner.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_VERSION_FAIL:
			//version mismatch.
				throw EnigmaException("The protocol version of the client & server do not match and this session must end.");
			break;
			default:
			//unknown this is bad.
				throw EnigmaException("Unknown failure type, please ensure your client is up to date.");
			break;
		}
	}

    void ClientSessionManager::ProcessMessage(CharacterListResponseMessage& message)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mRealmSnapshot.ApplicationMutex);
		EnigmaEvent enigmaEvent;

		switch(message.GetStatus())
		{
			case STATUS_OK:
				if(!message.GetName().empty())
				{
					this->mAvailableCharacters.push_back(message.GetName());
				}
				else
				{
					enigmaEvent = EnigmaEvent();
					enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_RECEIVED_CHARACTER_LIST;
					this->PushApplicationEvent(enigmaEvent);
				}

			break;
			case STATUS_UNKNOWN_FAIL:
				//unknown failure.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "This operation could not be performed because of an unknown failure.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_SECURITY_FAIL:
				//operation not permitted.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "This operation is not permitted!";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_DATABASE_FAIL:
			//failed to contact database.
				throw EnigmaException("Due to a database outage our service is currently unavailable, we apologize for the inconvenience.");
			break;
			case STATUS_NETWORK_FAIL:
			//request timed out.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "Due to network conditions this message was not recieved by the server in a timely manner.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_VERSION_FAIL:
			//version mismatch.
				throw EnigmaException("The protocol version of the client & server do not match and this session must end.");
			break;
			default:
			//unknown this is bad.
				throw EnigmaException("Unknown failure type, please ensure your client is up to date.");
			break;
		}
	}

    void ClientSessionManager::ProcessMessage(CharacterSelectionResponseMessage& message)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mRealmSnapshot.ApplicationMutex);
		EnigmaEvent enigmaEvent;

		switch(message.GetStatus())
		{
			case STATUS_OK:
				this->mRealmSnapshot.GetUser().SetHasEnteredWorld(true);

				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_GAME_MODE_CHANGED;
				enigmaEvent.Index = GAME_MODE_IN_GAME;
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_UNKNOWN_FAIL:
				//unknown failure.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "This operation could not be performed because of an unknown failure.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_SECURITY_FAIL:
				//operation not permitted.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "This operation is not permitted!";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_DATABASE_FAIL:
			//failed to contact database.
				throw EnigmaException("Due to a database outage our service is currently unavailable, we apologize for the inconvenience.");
			break;
			case STATUS_NETWORK_FAIL:
			//request timed out.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "Due to network conditions this message was not recieved by the server in a timely manner.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_VERSION_FAIL:
			//version mismatch.
				throw EnigmaException("The protocol version of the client & server do not match and this session must end.");
			break;
			default:
			//unknown this is bad.
				throw EnigmaException("Unknown failure type, please ensure your client is up to date.");
			break;
		}
	}

//Authentication End

//Movement Start
    void ClientSessionManager::ProcessMessage(PlayerOnMapUpdateResponseMessage& message)
	{
		boost::unique_lock< boost::shared_mutex > userLock(this->mRealmSnapshot.ApplicationMutex);
		boost::unique_lock< boost::shared_mutex > mapLock(this->mRealmSnapshot.SceneMutex);
		EnigmaEvent enigmaEvent;

		switch(message.GetStatus())
		{
			case STATUS_OK:
				if(message.GetIsEnteringMap())
				{
					this->CreateCharacter(message.GetId());

					enigmaEvent = EnigmaEvent();
					enigmaEvent.EventType = EnigmaEvent::EVENT_SCENE_CHARACTER_ENTERED_MAP;
					enigmaEvent.EntityId = message.GetId();
					this->PushSceneEvent(enigmaEvent);
				}
				else
				if(message.GetIsLeavingMap())
				{
					this->DeleteCharacter(message.GetId());

					enigmaEvent = EnigmaEvent();
					enigmaEvent.EventType = EnigmaEvent::EVENT_SCENE_CHARACTER_EXITED_MAP;
					enigmaEvent.EntityId = message.GetId();
					this->PushSceneEvent(enigmaEvent);

					return; //if the player is leaving our job is done.
				}

				//if player is sticking around we should probably update status.
				/*if(GetCharacter(message.GetId()).GetGender()!=message.GetGender())
				{
					GetCharacter(message.GetId()).SetGender(message.GetGender());
				}*/

				if(GetCharacter(message.GetId()).GetModelId()!=message.GetModelId())
				{
					GetCharacter(message.GetId()).SetModelId(message.GetModelId());
				}

				/*if(GetCharacter(message.GetId()).GetHealth()!=message.GetHealth())
				{
					GetCharacter(message.GetId()).SetHealth(message.GetHealth());
				}*/

				if(GetCharacter(message.GetId()).GetName()!=message.GetName()) //should be ok I think std:string compares correctly.
				{
					GetCharacter(message.GetId()).SetName(message.GetName());
				}

				GetCharacter(message.GetId()).SetX(message.GetX());
				GetCharacter(message.GetId()).SetY(message.GetY());
				GetCharacter(message.GetId()).SetZ(message.GetZ());

				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_SCENE_CHARACTER_UPDATED;
				enigmaEvent.EntityId = message.GetId();
				this->PushSceneEvent(enigmaEvent);
			break;
			case STATUS_UNKNOWN_FAIL:
				//unknown failure.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "This operation could not be performed because of an unknown failure.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_SECURITY_FAIL:
				//operation not permitted.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "This operation is not permitted!";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_DATABASE_FAIL:
			//failed to contact database.
				throw EnigmaException("Due to a database outage our service is currently unavailable, we apologize for the inconvenience.");
			break;
			case STATUS_NETWORK_FAIL:
			//request timed out.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "Due to network conditions this message was not recieved by the server in a timely manner.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_VERSION_FAIL:
			//version mismatch.
				throw EnigmaException("The protocol version of the client & server do not match and this session must end.");
			break;
			default:
			//unknown this is bad.
				throw EnigmaException("Unknown failure type, please ensure your client is up to date.");
			break;
		}
	}
	
    void ClientSessionManager::ProcessMessage(NpcOnMapUpdateResponseMessage& message)
	{
		boost::unique_lock< boost::shared_mutex > userLock(this->mRealmSnapshot.ApplicationMutex);
		boost::unique_lock< boost::shared_mutex > mapLock(this->mRealmSnapshot.SceneMutex);
		EnigmaEvent enigmaEvent;

		switch(message.GetStatus())
		{
			case STATUS_OK:
				if(message.GetIsEnteringMap())
				{
					this->CreateNpc(message.GetId());

					enigmaEvent = EnigmaEvent();
					enigmaEvent.EventType = EnigmaEvent::EVENT_SCENE_NPC_ENTERED_MAP;
					enigmaEvent.EntityId = message.GetId();
					this->PushSceneEvent(enigmaEvent);
				}
				else
				if(message.GetIsLeavingMap())
				{
					this->DeleteNpc(message.GetId());

					enigmaEvent = EnigmaEvent();
					enigmaEvent.EventType = EnigmaEvent::EVENT_SCENE_NPC_EXITED_MAP;
					enigmaEvent.EntityId = message.GetId();
					this->PushSceneEvent(enigmaEvent);

					return; //if the npc is leaving our job is done.
				}

				//if Npc is sticking around we should probably update status.
				/*if(GetNpc(message.GetId()).GetGender()!=message.GetGender())
				{
					GetNpc(message.GetId()).SetGender(message.GetGender());
				}*/

				if(GetNpc(message.GetId()).GetModelId()!=message.GetModelId())
				{
					GetNpc(message.GetId()).SetModelId(message.GetModelId());
				}

				/*if(GetNpc(message.GetId()).GetHealth()!=message.GetHealth())
				{
					GetNpc(message.GetId()).SetHealth(message.GetHealth());
				}*/

				if(GetNpc(message.GetId()).GetName()!=message.GetName()) //should be ok I think std:string compares correctly.
				{
					GetNpc(message.GetId()).SetName(message.GetName());
				}

				GetNpc(message.GetId()).SetX(message.GetX());
				GetNpc(message.GetId()).SetY(message.GetY());
				GetNpc(message.GetId()).SetZ(message.GetZ());

				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_SCENE_NPC_UPDATED;
				enigmaEvent.EntityId = message.GetId();
				this->PushSceneEvent(enigmaEvent);
			break;
			case STATUS_UNKNOWN_FAIL:
				//unknown failure.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "This operation could not be performed because of an unknown failure.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_SECURITY_FAIL:
				//operation not permitted.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "This operation is not permitted!";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_DATABASE_FAIL:
			//failed to contact database.
				throw EnigmaException("Due to a database outage our service is currently unavailable, we apologize for the inconvenience.");
			break;
			case STATUS_NETWORK_FAIL:
			//request timed out.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "Due to network conditions this message was not recieved by the server in a timely manner.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_VERSION_FAIL:
			//version mismatch.
				throw EnigmaException("The protocol version of the client & server do not match and this session must end.");
			break;
			default:
			//unknown this is bad.
				throw EnigmaException("Unknown failure type, please ensure your client is up to date.");
			break;
		}
	}
	
    void ClientSessionManager::ProcessMessage(MonsterOnMapUpdateResponseMessage& message)
	{
		boost::unique_lock< boost::shared_mutex > userLock(this->mRealmSnapshot.ApplicationMutex);
		boost::unique_lock< boost::shared_mutex > mapLock(this->mRealmSnapshot.SceneMutex);
		EnigmaEvent enigmaEvent;

		switch(message.GetStatus())
		{
			case STATUS_OK:
				if(message.GetIsEnteringMap())
				{
					this->CreateMonster(message.GetId());

					enigmaEvent = EnigmaEvent();
					enigmaEvent.EventType = EnigmaEvent::EVENT_SCENE_MONSTER_ENTERED_MAP;
					enigmaEvent.EntityId = message.GetId();
					this->PushSceneEvent(enigmaEvent);
				}
				else
				if(message.GetIsLeavingMap())
				{
					this->DeleteMonster(message.GetId());
					
					enigmaEvent = EnigmaEvent();
					enigmaEvent.EventType = EnigmaEvent::EVENT_SCENE_MONSTER_EXITED_MAP;
					enigmaEvent.EntityId = message.GetId();
					this->PushSceneEvent(enigmaEvent);

					return; //if the monster is leaving our job is done.
				}

				//if Monster is sticking around we should probably update status.
				/*if(GetMonster(message.GetId()).GetGender()!=message.GetGender())
				{
					GetMonster(message.GetId()).SetGender(message.GetGender());
				}*/

				if(GetMonster(message.GetId()).GetModelId()!=message.GetModelId())
				{
					GetMonster(message.GetId()).SetModelId(message.GetModelId());
				}

				/*if(GetMonster(message.GetId()).GetHealth()!=message.GetHealth())
				{
					GetMonster(message.GetId()).SetHealth(message.GetHealth());
				}*/

				if(GetMonster(message.GetId()).GetName()!=message.GetName()) //should be ok I think std:string compares correctly.
				{
					GetMonster(message.GetId()).SetName(message.GetName());
				}

				GetMonster(message.GetId()).SetX(message.GetX());
				GetMonster(message.GetId()).SetY(message.GetY());
				GetMonster(message.GetId()).SetZ(message.GetZ());

				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_SCENE_MONSTER_UPDATED;
				enigmaEvent.EntityId = message.GetId();
				this->PushSceneEvent(enigmaEvent);
			break;
			case STATUS_UNKNOWN_FAIL:
				//unknown failure.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "This operation could not be performed because of an unknown failure.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_SECURITY_FAIL:
				//operation not permitted.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "This operation is not permitted!";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_DATABASE_FAIL:
			//failed to contact database.
				throw EnigmaException("Due to a database outage our service is currently unavailable, we apologize for the inconvenience.");
			break;
			case STATUS_NETWORK_FAIL:
			//request timed out.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "Due to network conditions this message was not recieved by the server in a timely manner.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_VERSION_FAIL:
			//version mismatch.
				throw EnigmaException("The protocol version of the client & server do not match and this session must end.");
			break;
			default:
			//unknown this is bad.
				throw EnigmaException("Unknown failure type, please ensure your client is up to date.");
			break;
		}
	}

    void ClientSessionManager::ProcessMessage(ItemOnMapUpdateResponseMessage& message)
	{
		boost::unique_lock< boost::shared_mutex > userLock(this->mRealmSnapshot.ApplicationMutex);
		boost::unique_lock< boost::shared_mutex > mapLock(this->mRealmSnapshot.SceneMutex);
		EnigmaEvent enigmaEvent;

		switch(message.GetStatus())
		{
			case STATUS_OK:
				if(message.GetIsEnteringMap())
				{
					this->CreateItem(message.GetId());

					enigmaEvent = EnigmaEvent();
					enigmaEvent.EventType = EnigmaEvent::EVENT_SCENE_ITEM_ENTERED_MAP;
					enigmaEvent.EntityId = message.GetId();
					this->PushSceneEvent(enigmaEvent);
				}
				else
				if(message.GetIsLeavingMap())
				{
					this->DeleteItem(message.GetId());
					
					enigmaEvent = EnigmaEvent();
					enigmaEvent.EventType = EnigmaEvent::EVENT_SCENE_ITEM_EXITED_MAP;
					enigmaEvent.EntityId = message.GetId();
					this->PushSceneEvent(enigmaEvent);

					return; //if the item is leaving our job is done.
				}

				//if Item is sticking around we should probably update status.
				/*if(GetItem(message.GetId()).GetGender()!=message.GetGender())
				{
					GetItem(message.GetId()).SetGender(message.GetGender());
				}*/

				if(GetItem(message.GetId()).GetModelId()!=message.GetModelId())
				{
					GetItem(message.GetId()).SetModelId(message.GetModelId());
				}

				/*if(GetItem(message.GetId()).GetHealth()!=message.GetHealth())
				{
					GetItem(message.GetId()).SetHealth(message.GetHealth());
				}*/

				if(GetItem(message.GetId()).GetName()!=message.GetName()) //should be ok I think std:string compares correctly.
				{
					GetItem(message.GetId()).SetName(message.GetName());
				}

				GetItem(message.GetId()).SetX(message.GetX());
				GetItem(message.GetId()).SetY(message.GetY());
				GetItem(message.GetId()).SetZ(message.GetZ());

				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_SCENE_ITEM_UPDATED;
				enigmaEvent.EntityId = message.GetId();
				this->PushSceneEvent(enigmaEvent);
			break;
			case STATUS_UNKNOWN_FAIL:
				//unknown failure.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "This operation could not be performed because of an unknown failure.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_SECURITY_FAIL:
				//operation not permitted.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "This operation is not permitted!";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_DATABASE_FAIL:
			//failed to contact database.
				throw EnigmaException("Due to a database outage our service is currently unavailable, we apologize for the inconvenience.");
			break;
			case STATUS_NETWORK_FAIL:
			//request timed out.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "Due to network conditions this message was not recieved by the server in a timely manner.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_VERSION_FAIL:
			//version mismatch.
				throw EnigmaException("The protocol version of the client & server do not match and this session must end.");
			break;
			default:
			//unknown this is bad.
				throw EnigmaException("Unknown failure type, please ensure your client is up to date.");
			break;
		}
	}

    void ClientSessionManager::ProcessMessage(MovementResponseMessage& message)
	{
		boost::unique_lock< boost::shared_mutex > userLock(this->mRealmSnapshot.ApplicationMutex);
		boost::unique_lock< boost::shared_mutex > mapLock(this->mRealmSnapshot.SceneMutex);
		EnigmaEvent enigmaEvent;

		switch(message.GetStatus())
		{
			case STATUS_OK:
				this->GetCharacter(message.GetCombatantId()).SetX(message.GetX1());
				this->GetCharacter(message.GetCombatantId()).SetY(message.GetY1());
				this->GetCharacter(message.GetCombatantId()).SetZ(message.GetZ1());

				/* Rotation (not implemented!)
				this->GetCharacter(message.GetCombatantId()).SetXRotation(message.GetX2());
				this->GetCharacter(message.GetCombatantId()).SetYRotation(message.GetY2());
				this->GetCharacter(message.GetCombatantId()).SetZRotation(message.GetZ2());
				*/

				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_SCENE_PLAYER_MOVED;
				enigmaEvent.EntityId = message.GetCombatantId();
				this->PushSceneEvent(enigmaEvent);
			break;
			case STATUS_UNKNOWN_FAIL:
				//unknown failure.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "This operation could not be performed because of an unknown failure.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_SECURITY_FAIL:
				//operation not permitted.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "This operation is not permitted!";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_DATABASE_FAIL:
			//failed to contact database.
				throw EnigmaException("Due to a database outage our service is currently unavailable, we apologize for the inconvenience.");
			break;
			case STATUS_NETWORK_FAIL:
			//request timed out.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "Due to network conditions this message was not recieved by the server in a timely manner.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_VERSION_FAIL:
			//version mismatch.
				throw EnigmaException("The protocol version of the client & server do not match and this session must end.");
			break;
			default:
			//unknown this is bad.
				throw EnigmaException("Unknown failure type, please ensure your client is up to date.");
			break;
		}
	}

    void ClientSessionManager::ProcessMessage(ChangeMapResponseMessage& message)
	{
		boost::unique_lock< boost::shared_mutex > userLock(this->mRealmSnapshot.ApplicationMutex);
		boost::unique_lock< boost::shared_mutex > mapLock(this->mRealmSnapshot.SceneMutex);
		EnigmaEvent enigmaEvent;

		switch(message.GetStatus())
		{
			case STATUS_OK:
				this->mRealmSnapshot.GetMap().ClearMap(); //prevent duplicate entities.
				this->mRealmSnapshot.GetUser().SetCurrentMapId(message.GetTargetMap()); //now client knows the new map Id.

				//Cause reload of map which will cause client to load new map because id has changed.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_GAME_MODE_CHANGED;
				enigmaEvent.Index = GAME_MODE_IN_GAME;
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_UNKNOWN_FAIL:
				//unknown failure.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "This operation could not be performed because of an unknown failure.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_SECURITY_FAIL:
				//operation not permitted.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "This operation is not permitted!";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_DATABASE_FAIL:
			//failed to contact database.
				throw EnigmaException("Due to a database outage our service is currently unavailable, we apologize for the inconvenience.");
			break;
			case STATUS_NETWORK_FAIL:
			//request timed out.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "Due to network conditions this message was not recieved by the server in a timely manner.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_VERSION_FAIL:
			//version mismatch.
				throw EnigmaException("The protocol version of the client & server do not match and this session must end.");
			break;
			default:
			//unknown this is bad.
				throw EnigmaException("Unknown failure type, please ensure your client is up to date.");
			break;
		}
	}
//Movement End

//Chat Start
    void ClientSessionManager::ProcessMessage(ChatResponseMessage& message)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mRealmSnapshot.ChatMutex);
		EnigmaEvent enigmaEvent;

		switch(message.GetStatus())
		{
			case STATUS_OK:
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_CHAT_RECEIVED_MESSAGE;
				enigmaEvent.Index = message.GetChatType();
				enigmaEvent.ScopeId = message.GetChatType();
				enigmaEvent.Text = message.GetChatMessage();
				enigmaEvent.Sender = message.GetChatTarget(); //in this case target is sender because the server swaps it before sending the message to the client(s).
				this->PushChatEvent(enigmaEvent);
			break;
			case STATUS_UNKNOWN_FAIL:
				//unknown failure.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "This operation could not be performed because of an unknown failure.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_SECURITY_FAIL:
				//operation not permitted.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "This operation is not permitted!";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_DATABASE_FAIL:
			//failed to contact database.
				throw EnigmaException("Due to a database outage our service is currently unavailable, we apologize for the inconvenience.");
			break;
			case STATUS_NETWORK_FAIL:
			//request timed out.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "Due to network conditions this message was not recieved by the server in a timely manner.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_VERSION_FAIL:
			//version mismatch.
				throw EnigmaException("The protocol version of the client & server do not match and this session must end.");
			break;
			default:
			//unknown this is bad.
				throw EnigmaException("Unknown failure type, please ensure your client is up to date.");
			break;
		}
	}

    void ClientSessionManager::ProcessMessage(KickResponseMessage& message)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mRealmSnapshot.ChatMutex);
		EnigmaEvent enigmaEvent;

		switch(message.GetStatus())
		{
			case STATUS_OK:
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_CHAT_EXPELLED;
				enigmaEvent.Index = message.GetKickType();
				enigmaEvent.ScopeId = message.GetKickType();
				enigmaEvent.Text = message.GetName();
				enigmaEvent.Sender = message.GetName();
				this->PushChatEvent(enigmaEvent);
			break;
			case STATUS_UNKNOWN_FAIL:
				//unknown failure.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "This operation could not be performed because of an unknown failure.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_SECURITY_FAIL:
				//operation not permitted.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "This operation is not permitted!";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_DATABASE_FAIL:
			//failed to contact database.
				throw EnigmaException("Due to a database outage our service is currently unavailable, we apologize for the inconvenience.");
			break;
			case STATUS_NETWORK_FAIL:
			//request timed out.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "Due to network conditions this message was not recieved by the server in a timely manner.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_VERSION_FAIL:
			//version mismatch.
				throw EnigmaException("The protocol version of the client & server do not match and this session must end.");
			break;
			default:
			//unknown this is bad.
				throw EnigmaException("Unknown failure type, please ensure your client is up to date.");
			break;
		}
	}

    void ClientSessionManager::ProcessMessage(InviteResponseMessage& message)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mRealmSnapshot.ChatMutex);
		EnigmaEvent enigmaEvent;

		switch(message.GetStatus())
		{
			case STATUS_OK:
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_CHAT_INVITED;
				enigmaEvent.Index = message.GetInviteType();
				enigmaEvent.ScopeId = message.GetInviteType();
				enigmaEvent.EntityId = message.GetInviteId();
				enigmaEvent.Text = message.GetName();
				enigmaEvent.Sender = message.GetName();
				this->PushChatEvent(enigmaEvent);
			break;
			case STATUS_UNKNOWN_FAIL:
				//unknown failure.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "This operation could not be performed because of an unknown failure.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_SECURITY_FAIL:
				//operation not permitted.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "This operation is not permitted!";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_DATABASE_FAIL:
			//failed to contact database.
				throw EnigmaException("Due to a database outage our service is currently unavailable, we apologize for the inconvenience.");
			break;
			case STATUS_NETWORK_FAIL:
			//request timed out.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "Due to network conditions this message was not recieved by the server in a timely manner.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_VERSION_FAIL:
			//version mismatch.
				throw EnigmaException("The protocol version of the client & server do not match and this session must end.");
			break;
			default:
			//unknown this is bad.
				throw EnigmaException("Unknown failure type, please ensure your client is up to date.");
			break;
		}
	}

    void ClientSessionManager::ProcessMessage(PlayerListResponseMessage& message)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mRealmSnapshot.ChatMutex);
		EnigmaEvent enigmaEvent;

		switch(message.GetStatus())
		{
			case STATUS_OK:
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_CHAT_RECEIVED_MESSAGE;
				enigmaEvent.Index = CHAT_TYPE_SYSTEM;
				enigmaEvent.ScopeId = CHAT_TYPE_SYSTEM;
				enigmaEvent.Text = message.GetName();
				this->PushChatEvent(enigmaEvent);
			break;
			case STATUS_UNKNOWN_FAIL:
				//unknown failure.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "This operation could not be performed because of an unknown failure.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_SECURITY_FAIL:
				//operation not permitted.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "This operation is not permitted!";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_DATABASE_FAIL:
			//failed to contact database.
				throw EnigmaException("Due to a database outage our service is currently unavailable, we apologize for the inconvenience.");
			break;
			case STATUS_NETWORK_FAIL:
			//request timed out.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "Due to network conditions this message was not recieved by the server in a timely manner.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_VERSION_FAIL:
			//version mismatch.
				throw EnigmaException("The protocol version of the client & server do not match and this session must end.");
			break;
			default:
			//unknown this is bad.
				throw EnigmaException("Unknown failure type, please ensure your client is up to date.");
			break;
		}
	}

    void ClientSessionManager::ProcessMessage(CreatePlayerOrganizationResponseMessage& message)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mRealmSnapshot.ChatMutex);
		EnigmaEvent enigmaEvent;

		switch(message.GetStatus())
		{
			case STATUS_OK:
				//user creating an organization still results in them joining it.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_CHAT_JOINED;
				enigmaEvent.Index = message.GetOrganizationType();
				enigmaEvent.ScopeId = message.GetOrganizationType();
				enigmaEvent.Text = message.GetName();
				enigmaEvent.Sender = message.GetName();
				this->PushChatEvent(enigmaEvent);
			break;
			case STATUS_UNKNOWN_FAIL:
				//unknown failure.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "This operation could not be performed because of an unknown failure.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_SECURITY_FAIL:
				//operation not permitted.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "This operation is not permitted!";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_DATABASE_FAIL:
			//failed to contact database.
				throw EnigmaException("Due to a database outage our service is currently unavailable, we apologize for the inconvenience.");
			break;
			case STATUS_NETWORK_FAIL:
			//request timed out.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "Due to network conditions this message was not recieved by the server in a timely manner.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_VERSION_FAIL:
			//version mismatch.
				throw EnigmaException("The protocol version of the client & server do not match and this session must end.");
			break;
			default:
			//unknown this is bad.
				throw EnigmaException("Unknown failure type, please ensure your client is up to date.");
			break;
		}
	}

    void ClientSessionManager::ProcessMessage(ModifyPlayerOrganizationRankResponseMessage& message)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mRealmSnapshot.ChatMutex);
		EnigmaEvent enigmaEvent;

		switch(message.GetStatus())
		{
			case STATUS_OK:
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_CHAT_RANK_MODIFIED;
				enigmaEvent.Index = message.GetPermissions();
				enigmaEvent.ScopeId = message.GetOrganizationType();
				enigmaEvent.Text = message.GetName();
				enigmaEvent.Sender = message.GetName();
				this->PushChatEvent(enigmaEvent);
			break;
			case STATUS_UNKNOWN_FAIL:
				//unknown failure.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "This operation could not be performed because of an unknown failure.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_SECURITY_FAIL:
				//operation not permitted.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "This operation is not permitted!";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_DATABASE_FAIL:
			//failed to contact database.
				throw EnigmaException("Due to a database outage our service is currently unavailable, we apologize for the inconvenience.");
			break;
			case STATUS_NETWORK_FAIL:
			//request timed out.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "Due to network conditions this message was not recieved by the server in a timely manner.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_VERSION_FAIL:
			//version mismatch.
				throw EnigmaException("The protocol version of the client & server do not match and this session must end.");
			break;
			default:
			//unknown this is bad.
				throw EnigmaException("Unknown failure type, please ensure your client is up to date.");
			break;
		}
	}

    void ClientSessionManager::ProcessMessage(SetPlayerRankResponseMessage& message)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mRealmSnapshot.ChatMutex);
		EnigmaEvent enigmaEvent;

		switch(message.GetStatus())
		{
			case STATUS_OK:
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_CHAT_MODIFIED;
				enigmaEvent.Index = message.GetOrganizationType();
				enigmaEvent.ScopeId = message.GetOrganizationType();
				enigmaEvent.Text = message.GetRankName();
				enigmaEvent.Sender = message.GetPlayerName();
				this->PushChatEvent(enigmaEvent);
			break;
			case STATUS_UNKNOWN_FAIL:
				//unknown failure.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "This operation could not be performed because of an unknown failure.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_SECURITY_FAIL:
				//operation not permitted.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "This operation is not permitted!";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_DATABASE_FAIL:
			//failed to contact database.
				throw EnigmaException("Due to a database outage our service is currently unavailable, we apologize for the inconvenience.");
			break;
			case STATUS_NETWORK_FAIL:
			//request timed out.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "Due to network conditions this message was not recieved by the server in a timely manner.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_VERSION_FAIL:
			//version mismatch.
				throw EnigmaException("The protocol version of the client & server do not match and this session must end.");
			break;
			default:
			//unknown this is bad.
				throw EnigmaException("Unknown failure type, please ensure your client is up to date.");
			break;
		}
	}

    void ClientSessionManager::ProcessMessage(NpcChatResponseMessage& message)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mRealmSnapshot.ChatMutex);
		EnigmaEvent enigmaEvent;

		switch(message.GetStatus())
		{
			case STATUS_OK:
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_CHAT_NPC_RESPONSE;
				enigmaEvent.Text = message.GetChatResponse();
				enigmaEvent.Values = message.GetPossiblePlayerResponses();
				enigmaEvent.EntityId = message.GetNpcId();
				this->PushChatEvent(enigmaEvent);
			break;
			case STATUS_UNKNOWN_FAIL:
				//unknown failure.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "This operation could not be performed because of an unknown failure.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_SECURITY_FAIL:
				//operation not permitted.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "This operation is not permitted!";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_DATABASE_FAIL:
			//failed to contact database.
				throw EnigmaException("Due to a database outage our service is currently unavailable, we apologize for the inconvenience.");
			break;
			case STATUS_NETWORK_FAIL:
			//request timed out.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "Due to network conditions this message was not recieved by the server in a timely manner.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_VERSION_FAIL:
			//version mismatch.
				throw EnigmaException("The protocol version of the client & server do not match and this session must end.");
			break;
			default:
			//unknown this is bad.
				throw EnigmaException("Unknown failure type, please ensure your client is up to date.");
			break;
		}
	}
//Chat End

//Voice Start
	void ClientSessionManager::ProcessMessage(VoiceChatResponseMessage& message)
	{
		//boost::unique_lock< boost::shared_mutex > lock(this->mRealmSnapshot.AudioMutex);
		EnigmaEvent enigmaEvent;

		switch(message.GetStatus())
		{
			case STATUS_OK:
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_AUDIO_VOICE_RECEIVED;
				enigmaEvent.EntityId = message.GetCombatantId();
				
				{
				size_t size=0;
				Enigma::u8* buffer=message.GetAudioData(size);
				enigmaEvent.UserData = new Enigma::u8[size];
				memcpy(enigmaEvent.UserData,buffer,size); //Enigma::u8*
				enigmaEvent.Index = size;
				}

				this->PushAudioEvent(enigmaEvent);	
			break;
			case STATUS_UNKNOWN_FAIL:
				//unknown failure.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "This operation could not be performed because of an unknown failure.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_SECURITY_FAIL:
				//operation not permitted.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "This operation is not permitted!";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_DATABASE_FAIL:
			//failed to contact database.
				throw EnigmaException("Due to a database outage our service is currently unavailable, we apologize for the inconvenience.");
			break;
			case STATUS_NETWORK_FAIL:
			//request timed out.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "Due to network conditions this message was not recieved by the server in a timely manner.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_VERSION_FAIL:
			//version mismatch.
				throw EnigmaException("The protocol version of the client & server do not match and this session must end.");
			break;
			default:
			//unknown this is bad.
				throw EnigmaException("Unknown failure type, please ensure your client is up to date.");
			break;
		}
	}
//Voice End

//Combat Start
    void ClientSessionManager::ProcessMessage(SkillResponseMessage& message)
	{
		//boost::unique_lock< boost::shared_mutex > lock(this->mRealmSnapshot.Mutex);
		EnigmaEvent enigmaEvent;

		throw EnigmaException("Not Implemented!");
		switch(message.GetStatus())
		{
			case STATUS_OK:

			break;
			case STATUS_UNKNOWN_FAIL:
				//unknown failure.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "This operation could not be performed because of an unknown failure.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_SECURITY_FAIL:
				//operation not permitted.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "This operation is not permitted!";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_DATABASE_FAIL:
			//failed to contact database.
				throw EnigmaException("Due to a database outage our service is currently unavailable, we apologize for the inconvenience.");
			break;
			case STATUS_NETWORK_FAIL:
			//request timed out.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "Due to network conditions this message was not recieved by the server in a timely manner.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_VERSION_FAIL:
			//version mismatch.
				throw EnigmaException("The protocol version of the client & server do not match and this session must end.");
			break;
			default:
			//unknown this is bad.
				throw EnigmaException("Unknown failure type, please ensure your client is up to date.");
			break;
		}
	}

	void ClientSessionManager::ProcessMessage(ItemTransferResponseMessage& message)
	{
		//boost::unique_lock< boost::shared_mutex > lock(this->mRealmSnapshot.Mutex);
		EnigmaEvent enigmaEvent;

		throw EnigmaException("Not Implemented!");
		switch(message.GetStatus())
		{
			case STATUS_OK:

			break;
			case STATUS_UNKNOWN_FAIL:
				//unknown failure.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "This operation could not be performed because of an unknown failure.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_SECURITY_FAIL:
				//operation not permitted.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "This operation is not permitted!";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_DATABASE_FAIL:
			//failed to contact database.
				throw EnigmaException("Due to a database outage our service is currently unavailable, we apologize for the inconvenience.");
			break;
			case STATUS_NETWORK_FAIL:
			//request timed out.
				enigmaEvent = EnigmaEvent();
				enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
				enigmaEvent.Text = "Due to network conditions this message was not recieved by the server in a timely manner.";
				this->PushApplicationEvent(enigmaEvent);
			break;
			case STATUS_VERSION_FAIL:
			//version mismatch.
				throw EnigmaException("The protocol version of the client & server do not match and this session must end.");
			break;
			default:
			//unknown this is bad.
				throw EnigmaException("Unknown failure type, please ensure your client is up to date.");
			break;
		}
	}
//Combat End
};