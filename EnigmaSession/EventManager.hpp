#ifndef EVENTMANAGER_HPP_INCLUDED
#define EVENTMANAGER_HPP_INCLUDED

/*
Copyright © 2011  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "Entities\Object.hpp"
#include "EnigmaEvent.hpp"
#include "IApplicationEventListener.hpp"
#include "ISceneEventListener.hpp"
#include "IChatEventListener.hpp"
#include "IAudioEventListener.hpp"
#include "std_queue.hpp"
#include "std_vector.hpp"
#include <sstream>

namespace Enigma
{
    class DllExport EventManager : public Object
    {
        private:

		std::vector<IApplicationEventListener*> mApplicationListeners;
		std::vector<ISceneEventListener*> mSceneListeners;
		std::vector<IChatEventListener*> mChatListeners;
		std::vector<IAudioEventListener*> mAudioListeners;

		std::queue<EnigmaEvent> mApplicationEvents;
		std::queue<EnigmaEvent> mSceneEvents;
		std::queue<EnigmaEvent> mChatEvents;
		std::queue<EnigmaEvent> mAudioEvents;

		boost::shared_mutex mApplicationMutex;
		boost::shared_mutex mSceneMutex;
		boost::shared_mutex mChatMutex;
		boost::shared_mutex mAudioMutex;

        protected:

		void RaiseApplicationEventExit();
		void RaiseApplicationEventChangeGameMode(size_t gameMode);
		void RaiseApplicationEventLog(const std::string& message);
		void RaiseApplicationEventOpenBrowserWindow(const std::string& url);
		void RaiseApplicationEventOpenVideoWindow(const std::string& url);
		void RaiseApplicationEventReceivedCharacterList(const std::vector<std::string>& characters);

		void RaiseSceneEventPlayerEnteredMap(Enigma::s32 characterId,const Enigma::Character& character);
		void RaiseSceneEventNpcEnteredMap(Enigma::s32 npcId,const Enigma::Npc& npc);
		void RaiseSceneEventMonsterEnteredMap(Enigma::s32 monsterId,const Enigma::Monster& monster);
		void RaiseSceneEventItemEnteredMap(Enigma::s32 itemId,const Enigma::Item& item);

		void RaiseSceneEventPlayerExitedMap(Enigma::s32 characterId,const Enigma::Character& character);
		void RaiseSceneEventNpcExitedMap(Enigma::s32 npcId,const Enigma::Npc& npc);
		void RaiseSceneEventMonsterExitedMap(Enigma::s32 monsterId,const Enigma::Monster& monster);
		void RaiseSceneEventItemExitedMap(Enigma::s32 itemId,const Enigma::Item& item);

		void RaiseSceneEventPlayerUpdate(Enigma::s32 characterId,const Enigma::Character& character);
		void RaiseSceneEventNpcUpdate(Enigma::s32 npcId,const Enigma::Npc& npc);
		void RaiseSceneEventMonsterUpdate(Enigma::s32 monsterId,const Enigma::Monster& monster);
		void RaiseSceneEventItemUpdate(Enigma::s32 itemId,const Enigma::Item& item);

		void RaiseSceneEventPlayerMove(Enigma::s32 characterId,const Enigma::Character& character);

		void RaiseChatEventInvited(size_t chatType, size_t inviteId, const std::string& organizationName);
		void RaiseChatEventJoined(size_t chatType, const std::string& organizationName);
		void RaiseChatEventModified(size_t chatType, const std::string& playerName, const std::string& rankName);
		void RaiseChatEventRankModified(size_t chatType, const std::string& rankName, size_t permissions);
		void RaiseChatEventExpelled(size_t chatType, const std::string& organizationName, const std::string& reason);
		void RaiseChatEventReceivedMessage(size_t chatType, const std::string& message, const std::string& sender);
		void RaiseChatEventNpcResponse(size_t npcId, const std::string& response, const std::vector<std::string>& playerResponses);

		void RaiseAudioEventVoiceReceived(Enigma::s32 characterId, const Enigma::Character& character, Enigma::u8* buffer,size_t length);
		void RaiseAudioEventRecorderStateChanged(bool isRecording);
		void RaiseAudioEventPlayFile(const Enigma::Entity& entity, size_t audioId, const std::string& filename);

        public:

		EventManager();
        ~EventManager();

		EnigmaEvent PopApplicationEvent(bool& isValid);
		void PushApplicationEvent(const EnigmaEvent& value);

		EnigmaEvent PopSceneEvent(bool& isValid);
		void PushSceneEvent(const EnigmaEvent& value);

		EnigmaEvent PopChatEvent(bool& isValid);
		void PushChatEvent(const EnigmaEvent& value);

		EnigmaEvent PopAudioEvent(bool& isValid);
		void PushAudioEvent(const EnigmaEvent& value);

		void RegisterApplicationEventListener(IApplicationEventListener* listener);
		void UnregisterApplicationEventListener(IApplicationEventListener* listener);

		void RegisterSceneEventListener(ISceneEventListener* listener);
		void UnregisterSceneEventListener(ISceneEventListener* listener);

		void RegisterChatEventListener(IChatEventListener* listener);
		void UnregisterChatEventListener(IChatEventListener* listener);

		void RegisterAudioEventListener(IAudioEventListener* listener);
		void UnregisterAudioEventListener(IAudioEventListener* listener);

		void Log(const std::string& message);
		void Log(std::stringstream& message);
		void OpenBrowserWindow(const std::string& url);
		void OpenVideoWindow(const std::string& url);
    };
};

#endif // EVENTMANAGER_HPP_INCLUDED