#ifndef INGAMECHATMANAGER_HPP_INCLUDED
#define INGAMECHATMANAGER_HPP_INCLUDED

/*
Copyright © 2011  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "ScreenManager.hpp"

namespace Enigma
{
	class InGameChatManager : public ScreenManager, public IChatEventListener
    {
        private:

		size_t mHistorySize;

        protected:

        public:

        InGameChatManager(ClientTransmissionManager& clientTransmissionManager);
        ~InGameChatManager();

		void RegisterEvents();

		bool onMainChatTextAccepted(const CEGUI::EventArgs& args);
		bool onPartyChatTextAccepted(const CEGUI::EventArgs& args);
		bool onGuildChatTextAccepted(const CEGUI::EventArgs& args);
		bool onMiscChatTextAccepted(const CEGUI::EventArgs& args);

		void AddChatText(const CEGUI::String& text,const CEGUI::String& name);

		virtual void onInvited(size_t chatType, size_t inviteId, const std::string& organizationName);
		virtual void onJoined(size_t chatType, const std::string& organizationName);
		virtual void onRankModified(size_t chatType, const std::string& rankName, size_t permissions);
		virtual void onModified(size_t chatType, const std::string& playerName, const std::string& rankName);
		virtual void onExpelled(size_t chatType, const std::string& organizationName, const std::string& reason);
		virtual void onReceivedMessage(size_t chatType, const std::string& message, const std::string& sender);
		virtual void onNpcResponse(size_t npcId, const std::string& response, const std::vector<std::string>& playerResponses);
    };
};

#endif // INGAMECHATMANAGER_HPP_INCLUDED