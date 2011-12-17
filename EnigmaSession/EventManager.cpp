/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "EventManager.hpp"
#include "EnigmaException.hpp"

#include "boost_foreach.hpp"

namespace Enigma
{

	EventManager::EventManager()
	{

	}

	EventManager::~EventManager()
	{

	}

	EnigmaEvent EventManager::PopApplicationEvent(bool& isValid)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mApplicationMutex);
		EnigmaEvent result;

		if(!this->mApplicationEvents.empty())
		{
			result = this->mApplicationEvents.front();
			this->mApplicationEvents.pop();
			isValid=true;
		}
		else
		{
			isValid=false;
		}

		return result;
	}

	void EventManager::PushApplicationEvent(const EnigmaEvent& value)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mApplicationMutex);
		this->mApplicationEvents.push(value);
	}

	EnigmaEvent EventManager::PopSceneEvent(bool& isValid)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mSceneMutex);
		EnigmaEvent result;

		if(!this->mSceneEvents.empty())
		{
			result = this->mSceneEvents.front();
			this->mSceneEvents.pop();
			isValid=true;
		}
		else
		{
			isValid=false;
		}

		return result;
	}

	void EventManager::PushSceneEvent(const EnigmaEvent& value)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mSceneMutex);
		this->mSceneEvents.push(value);
	}

	EnigmaEvent EventManager::PopChatEvent(bool& isValid)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mChatMutex);
		EnigmaEvent result;

		if(!this->mChatEvents.empty())
		{
			result = this->mChatEvents.front();
			this->mChatEvents.pop();
			isValid=true;
		}
		else
		{
			isValid=false;
		}

		return result;
	}

	void EventManager::PushChatEvent(const EnigmaEvent& value)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mChatMutex);
		this->mChatEvents.push(value);
	}

	EnigmaEvent EventManager::PopAudioEvent(bool& isValid)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mAudioMutex);
		EnigmaEvent result;

		if(!this->mAudioEvents.empty())
		{
			result = this->mAudioEvents.front();
			this->mAudioEvents.pop();
			isValid=true;
		}
		else
		{
			isValid=false;
		}

		return result;
	}

	void EventManager::PushAudioEvent(const EnigmaEvent& value)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mAudioMutex);
		this->mAudioEvents.push(value);
	}

	void EventManager::RaiseApplicationEventExit()
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mApplicationMutex);
		BOOST_FOREACH(IApplicationEventListener* listener,this->mApplicationListeners)
		{
			listener->onExit();
		}
	}

	void EventManager::RaiseApplicationEventChangeGameMode(size_t gameMode)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mApplicationMutex);
		BOOST_FOREACH(IApplicationEventListener* listener,this->mApplicationListeners)
		{
			listener->onChangeGameMode(gameMode);
		}
	}

	void EventManager::RaiseApplicationEventLog(const std::string& message)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mApplicationMutex);
		BOOST_FOREACH(IApplicationEventListener* listener,this->mApplicationListeners)
		{
			listener->onLog(message);
		}
	}

	void EventManager::RaiseApplicationEventOpenBrowserWindow(const std::string& url)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mApplicationMutex);
		BOOST_FOREACH(IApplicationEventListener* listener,this->mApplicationListeners)
		{
			listener->onOpenBrowserWindow(url);
		}
	}

	void EventManager::RaiseApplicationEventOpenVideoWindow(const std::string& url)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mApplicationMutex);
		BOOST_FOREACH(IApplicationEventListener* listener,this->mApplicationListeners)
		{
			listener->onOpenVideoWindow(url);
		}
	}

	void EventManager::RaiseApplicationEventReceivedCharacterList(const std::vector<std::string>& characters)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mApplicationMutex);
		BOOST_FOREACH(IApplicationEventListener* listener,this->mApplicationListeners)
		{
			listener->onReceivedCharacterList(characters);
		}
	}

	void EventManager::RaiseSceneEventPlayerEnteredMap(Enigma::s32 characterId,const Enigma::Character& character)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mSceneMutex);
		BOOST_FOREACH(ISceneEventListener* listener,this->mSceneListeners)
		{
			listener->onPlayerEnteredMap(characterId,character);
		}
	}

	void EventManager::RaiseSceneEventNpcEnteredMap(Enigma::s32 npcId,const Enigma::Npc& npc)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mSceneMutex);
		BOOST_FOREACH(ISceneEventListener* listener,this->mSceneListeners)
		{
			listener->onNpcEnteredMap(npcId,npc);
		}
	}

	void EventManager::RaiseSceneEventMonsterEnteredMap(Enigma::s32 monsterId,const Enigma::Monster& monster)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mSceneMutex);
		BOOST_FOREACH(ISceneEventListener* listener,this->mSceneListeners)
		{
			listener->onMonsterEnteredMap(monsterId,monster);
		}
	}

	void EventManager::RaiseSceneEventItemEnteredMap(Enigma::s32 itemId,const Enigma::Item& item)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mSceneMutex);
		BOOST_FOREACH(ISceneEventListener* listener,this->mSceneListeners)
		{
			listener->onItemEnteredMap(itemId,item);
		}
	}

	void EventManager::RaiseSceneEventPlayerExitedMap(Enigma::s32 characterId,const Enigma::Character& character)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mSceneMutex);
		BOOST_FOREACH(ISceneEventListener* listener,this->mSceneListeners)
		{
			listener->onPlayerExitedMap(characterId,character);
		}
	}

	void EventManager::RaiseSceneEventNpcExitedMap(Enigma::s32 npcId,const Enigma::Npc& npc)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mSceneMutex);
		BOOST_FOREACH(ISceneEventListener* listener,this->mSceneListeners)
		{
			listener->onNpcExitedMap(npcId,npc);
		}
	}

	void EventManager::RaiseSceneEventMonsterExitedMap(Enigma::s32 monsterId,const Enigma::Monster& monster)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mSceneMutex);
		BOOST_FOREACH(ISceneEventListener* listener,this->mSceneListeners)
		{
			listener->onMonsterExitedMap(monsterId,monster);
		}
	}

	void EventManager::RaiseSceneEventItemExitedMap(Enigma::s32 itemId,const Enigma::Item& item)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mSceneMutex);
		BOOST_FOREACH(ISceneEventListener* listener,this->mSceneListeners)
		{
			listener->onItemExitedMap(itemId,item);
		}
	}

	void EventManager::RaiseSceneEventPlayerUpdate(Enigma::s32 characterId,const Enigma::Character& character)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mSceneMutex);
		BOOST_FOREACH(ISceneEventListener* listener,this->mSceneListeners)
		{
			listener->onPlayerUpdate(characterId,character);
		}
	}

	void EventManager::RaiseSceneEventNpcUpdate(Enigma::s32 npcId,const Enigma::Npc& npc)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mSceneMutex);
		BOOST_FOREACH(ISceneEventListener* listener,this->mSceneListeners)
		{
			listener->onNpcUpdate(npcId,npc);
		}
	}

	void EventManager::RaiseSceneEventMonsterUpdate(Enigma::s32 monsterId,const Enigma::Monster& monster)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mSceneMutex);
		BOOST_FOREACH(ISceneEventListener* listener,this->mSceneListeners)
		{
			listener->onMonsterUpdate(monsterId,monster);
		}
	}

	void EventManager::RaiseSceneEventItemUpdate(Enigma::s32 itemId,const Enigma::Item& item)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mSceneMutex);
		BOOST_FOREACH(ISceneEventListener* listener,this->mSceneListeners)
		{
			listener->onItemUpdate(itemId,item);
		}
	}

	void EventManager::RaiseSceneEventPlayerMove(Enigma::s32 characterId,const Enigma::Character& character)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mSceneMutex);
		BOOST_FOREACH(ISceneEventListener* listener,this->mSceneListeners)
		{
			listener->onPlayerMove(characterId,character);
		}
	}

	void EventManager::RaiseChatEventInvited(size_t chatType,size_t inviteId, const std::string& organizationName)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mChatMutex);
		BOOST_FOREACH(IChatEventListener* listener,this->mChatListeners)
		{
			listener->onInvited(chatType,inviteId,organizationName);
		}
	}

	void EventManager::RaiseChatEventJoined(size_t chatType, const std::string& organizationName)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mChatMutex);
		BOOST_FOREACH(IChatEventListener* listener,this->mChatListeners)
		{
			listener->onJoined(chatType,organizationName);
		}
	}

	void EventManager::RaiseChatEventModified(size_t chatType, const std::string& playerName, const std::string& rankName)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mChatMutex);
		BOOST_FOREACH(IChatEventListener* listener,this->mChatListeners)
		{
			listener->onModified(chatType,playerName,rankName);
		}
	}

	void EventManager::RaiseChatEventRankModified(size_t chatType, const std::string& rankName, size_t permissions)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mChatMutex);
		BOOST_FOREACH(IChatEventListener* listener,this->mChatListeners)
		{
			listener->onRankModified(chatType,rankName,permissions);
		}
	}

	void EventManager::RaiseChatEventExpelled(size_t chatType, const std::string& organizationName, const std::string& reason)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mChatMutex);
		BOOST_FOREACH(IChatEventListener* listener,this->mChatListeners)
		{
			listener->onExpelled(chatType,organizationName,reason);
		}
	}

	void EventManager::RaiseChatEventReceivedMessage(size_t chatType, const std::string& message, const std::string& sender)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mChatMutex);
		BOOST_FOREACH(IChatEventListener* listener,this->mChatListeners)
		{
			listener->onReceivedMessage(chatType,message,sender);
		}
	}

	void EventManager::RaiseChatEventNpcResponse(size_t npcId, const std::string& response, const std::vector<std::string>& playerResponses)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mChatMutex);
		BOOST_FOREACH(IChatEventListener* listener,this->mChatListeners)
		{
			listener->onNpcResponse(npcId,response,playerResponses);
		}
	}

	void EventManager::RaiseAudioEventVoiceReceived(Enigma::s32 characterId, const Enigma::Character& character, Enigma::u8* buffer,size_t length)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mAudioMutex);
		BOOST_FOREACH(IAudioEventListener* listener,this->mAudioListeners)
		{
			listener->onVoiceReceived(characterId,character,buffer,length);
		}
	}

	void EventManager::RaiseAudioEventRecorderStateChanged(bool isRecording)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mAudioMutex);
		BOOST_FOREACH(IAudioEventListener* listener,this->mAudioListeners)
		{
			listener->onRecorderStateChanged(isRecording);
		}
	}

	void EventManager::RaiseAudioEventPlayFile(const Enigma::Entity& entity, size_t audioId, const std::string& filename)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mAudioMutex);
		BOOST_FOREACH(IAudioEventListener* listener,this->mAudioListeners)
		{
			listener->onPlayFile(entity,audioId,filename);
		}
	}

	void EventManager::RegisterApplicationEventListener(IApplicationEventListener* listener)
	{
		//Make sure listener is not null and is valid.
		if(listener!=NULL)
		{
			boost::unique_lock< boost::shared_mutex > lock(this->mApplicationMutex);
			this->mApplicationListeners.push_back(listener);
		}
	}

	void EventManager::UnregisterApplicationEventListener(IApplicationEventListener* listener)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mApplicationMutex);
		this->mApplicationListeners.erase(std::remove(this->mApplicationListeners.begin(), this->mApplicationListeners.end(), listener), this->mApplicationListeners.end());
		this->mApplicationListeners.resize(std::remove(this->mApplicationListeners.begin(), this->mApplicationListeners.end(), listener) - this->mApplicationListeners.begin());
	}

	void EventManager::RegisterSceneEventListener(ISceneEventListener* listener)
	{
		//Make sure listener is not null and is valid.
		if(listener!=NULL)
		{
			boost::unique_lock< boost::shared_mutex > lock(this->mSceneMutex);
			this->mSceneListeners.push_back(listener);
		}
	}

	void EventManager::UnregisterSceneEventListener(ISceneEventListener* listener)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mSceneMutex);
		this->mSceneListeners.erase(std::remove(this->mSceneListeners.begin(), this->mSceneListeners.end(), listener), this->mSceneListeners.end());
		this->mSceneListeners.resize(std::remove(this->mSceneListeners.begin(), this->mSceneListeners.end(), listener) - this->mSceneListeners.begin());
	}

	void EventManager::RegisterChatEventListener(IChatEventListener* listener)
	{
		//Make sure listener is not null and is valid.
		if(listener!=NULL)
		{
			boost::unique_lock< boost::shared_mutex > lock(this->mChatMutex);
			this->mChatListeners.push_back(listener);
		}
	}

	void EventManager::UnregisterChatEventListener(IChatEventListener* listener)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mChatMutex);
		this->mChatListeners.erase(std::remove(this->mChatListeners.begin(), this->mChatListeners.end(), listener), this->mChatListeners.end());
		this->mChatListeners.resize(std::remove(this->mChatListeners.begin(), this->mChatListeners.end(), listener) - this->mChatListeners.begin());
	}

	void EventManager::RegisterAudioEventListener(IAudioEventListener* listener)
	{
		//Make sure listener is not null and is valid.
		if(listener!=NULL)
		{
			boost::unique_lock< boost::shared_mutex > lock(this->mAudioMutex);
			this->mAudioListeners.push_back(listener);
		}
	}

	void EventManager::UnregisterAudioEventListener(IAudioEventListener* listener)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mAudioMutex);
		this->mAudioListeners.erase(std::remove(this->mAudioListeners.begin(), this->mAudioListeners.end(), listener), this->mAudioListeners.end());
		this->mAudioListeners.resize(std::remove(this->mAudioListeners.begin(), this->mAudioListeners.end(), listener) - this->mAudioListeners.begin());
	}

	void EventManager::Log(const std::string& message)
	{
		if(!message.empty())
		{
			EnigmaEvent enigmaEvent;

			enigmaEvent = EnigmaEvent();
			enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
			enigmaEvent.Text = message;
			this->PushApplicationEvent(enigmaEvent);
		}
	}

	void EventManager::Log(std::stringstream& message)
	{
		if(message.good())
		{
			EnigmaEvent enigmaEvent;

			enigmaEvent = EnigmaEvent();
			enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_LOG;
			enigmaEvent.Text = message.str();
			this->PushApplicationEvent(enigmaEvent);
			message.str(""); //reset stream so it can be used again.
		}
		else
		{
			throw EnigmaException("Invalid string stream!");
		}
	}

	void EventManager::OpenBrowserWindow(const std::string& url)
	{
		EnigmaEvent enigmaEvent;

		enigmaEvent = EnigmaEvent();
		enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_OPEN_BROWSER_WINDOW;
		enigmaEvent.Text = url;
		this->PushApplicationEvent(enigmaEvent);
	}

	void EventManager::OpenVideoWindow(const std::string& url)
	{
		EnigmaEvent enigmaEvent;

		enigmaEvent = EnigmaEvent();
		enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_OPEN_VIDEO_WINDOW;
		enigmaEvent.Text = url;
		this->PushApplicationEvent(enigmaEvent);
	}
};