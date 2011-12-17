/*
Copyright © 2011  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "InGameChatManager.hpp"

namespace Enigma
{
    InGameChatManager::InGameChatManager(ClientTransmissionManager& clientTransmissionManager)
		: ScreenManager(clientTransmissionManager)
    {
		this->mHistorySize=4096; //controls the number of history entries that are kept.
		this->mClientTransmissionManager.GetClientSessionManager().RegisterChatEventListener(this);
    }

    InGameChatManager::~InGameChatManager()
    {

    }

	void InGameChatManager::RegisterEvents()
	{
		CEGUI::Window* window = NULL;

		window = CEGUI::WindowManager::getSingleton().getWindow("ChatWindow/ChatTabControl/MainChatWindow/MainChatEditbox");
		if(window!=NULL)
		{
			window->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&InGameChatManager::onMainChatTextAccepted, this));
		}

		window = CEGUI::WindowManager::getSingleton().getWindow("ChatWindow/ChatTabControl/PartyChatWindow/PartyChatEditbox");
		if(window!=NULL)
		{
			window->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&InGameChatManager::onPartyChatTextAccepted, this));
		}

		window = CEGUI::WindowManager::getSingleton().getWindow("ChatWindow/ChatTabControl/GuildChatWindow/GuildChatEditbox");
		if(window!=NULL)
		{
			window->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&InGameChatManager::onGuildChatTextAccepted, this));
		}

		window = CEGUI::WindowManager::getSingleton().getWindow("ChatWindow/ChatTabControl/MiscChatWindow/MiscChatEditbox");
		if(window!=NULL)
		{
			window->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&InGameChatManager::onMiscChatTextAccepted, this));
		}
	}

	bool InGameChatManager::onMainChatTextAccepted(const CEGUI::EventArgs& args)
	{
		using namespace CEGUI;
 
		CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();
		CEGUI::Editbox* chatText = static_cast<CEGUI::Editbox*> (winMgr.getWindow("ChatWindow/ChatTabControl/MainChatWindow/MainChatEditbox"));

		
		
		std::string text = chatText->getText().c_str();
		this->mClientTransmissionManager.GetClientSessionManager().ProcessCommand(text,CHAT_TYPE_MAP);

		// Clear the text in the Editbox
		chatText->setText("");
		return true;
	}

	bool InGameChatManager::onPartyChatTextAccepted(const CEGUI::EventArgs& args)
	{
		using namespace CEGUI;
 
		CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();
		CEGUI::Editbox* chatText = static_cast<CEGUI::Editbox*> (winMgr.getWindow("ChatWindow/ChatTabControl/PartyChatWindow/PartyChatEditbox"));

		std::string text = chatText->getText().c_str();
		this->mClientTransmissionManager.GetClientSessionManager().ProcessCommand(text,CHAT_TYPE_PARTY);
 
		// Clear the text in the Editbox
		chatText->setText("");
		return true;
	}

	bool InGameChatManager::onGuildChatTextAccepted(const CEGUI::EventArgs& args)
	{
		using namespace CEGUI;
 
		CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();
		CEGUI::Editbox* chatText = static_cast<CEGUI::Editbox*> (winMgr.getWindow("ChatWindow/ChatTabControl/GuildChatWindow/GuildChatEditbox"));

		std::string text = chatText->getText().c_str();
		this->mClientTransmissionManager.GetClientSessionManager().ProcessCommand(text,CHAT_TYPE_GUILD);
 
		// Clear the text in the Editbox
		chatText->setText("");
		return true;
	}

	bool InGameChatManager::onMiscChatTextAccepted(const CEGUI::EventArgs& args)
	{
		using namespace CEGUI;
 
		CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();
		CEGUI::Editbox* chatText = static_cast<CEGUI::Editbox*> (winMgr.getWindow("ChatWindow/ChatTabControl/MiscChatWindow/MiscChatEditbox"));

		std::string text = chatText->getText().c_str();
		this->mClientTransmissionManager.GetClientSessionManager().ProcessCommand(text,CHAT_TYPE_BROADCAST);
 
		// Clear the text in the Editbox
		chatText->setText("");
		return true;
	}

	void InGameChatManager::AddChatText(const CEGUI::String& text,const CEGUI::String& name)
	{
		CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();
		CEGUI::Listbox* chatHistory = static_cast<CEGUI::Listbox*> (winMgr.getWindow(name));
 
		if(!text.empty()) 
		{
			CEGUI::ListboxTextItem* chatItem;
			if(chatHistory->getItemCount() == mHistorySize)
			{
				chatItem = static_cast<CEGUI::ListboxTextItem*>(chatHistory->getListboxItemFromIndex(0));
				chatItem->setAutoDeleted(false);
				chatHistory->removeItem(chatItem);
				chatItem->setAutoDeleted(true);
				chatItem->setText(text);
			}
			else
			{
				chatItem = new CEGUI::ListboxTextItem(text);
			}
			chatHistory->addItem(chatItem);
			chatHistory->ensureItemIsVisible(chatHistory->getItemCount());
		}
	}

	void InGameChatManager::onInvited(size_t chatType, size_t inviteId, const std::string& organizationName)
	{

	}

	void InGameChatManager::onJoined(size_t chatType, const std::string& organizationName)
	{
		std::stringstream s;
		std::string text;
		switch(chatType)
		{
			case CHAT_TYPE_GUILD:
				s << "You have joined the " << organizationName << " guild." << std::endl;
				text=s.str();
				this->AddChatText(text.c_str(),"ChatWindow/ChatTabControl/GuildChatWindow/GuildChatListBox");
				break;
			case CHAT_TYPE_PARTY:
				s << "You have joined " << organizationName  << "'s party." << std::endl;
				text=s.str();
				this->AddChatText(text.c_str(),"ChatWindow/ChatTabControl/PartyChatWindow/PartyChatListBox");
				break;
			default:
				break;
		}
	}

	void InGameChatManager::onRankModified(size_t chatType, const std::string& rankName, size_t permissions)
	{

	}

	void InGameChatManager::onModified(size_t chatType, const std::string& playerName, const std::string& rankName)
	{

	}

	void InGameChatManager::onExpelled(size_t chatType, const std::string& organizationName, const std::string& reason)
	{
		std::stringstream s;
		std::string text;
		switch(chatType)
		{
			case CHAT_TYPE_GUILD:
				s << "You have been expelled from the " << organizationName << " guild." << std::endl;
				text=s.str();
				this->AddChatText(text.c_str(),"ChatWindow/ChatTabControl/GuildChatWindow/GuildChatListBox");
				break;
			case CHAT_TYPE_PARTY:
				s << "You have been expelled from " << organizationName  << "'s party." << std::endl;
				text=s.str();
				this->AddChatText(text.c_str(),"ChatWindow/ChatTabControl/PartyChatWindow/PartyChatListBox");
				break;
			default:
				break;
		}
	}

	void InGameChatManager::onReceivedMessage(size_t chatType, const std::string& message, const std::string& sender)
	{
		std::stringstream s;
		std::string text;

		switch(chatType)
		{
			case CHAT_TYPE_BROADCAST:
				s << sender << " broadcast: " << message << std::endl;
				text=s.str();
				this->AddChatText(text.c_str(),"ChatWindow/ChatTabControl/MiscChatWindow/MiscChatListBox");
				break;
			case CHAT_TYPE_GUILD:
				s << sender << " from your guild said: " << message << std::endl;
				text=s.str();
				this->AddChatText(text.c_str(),"ChatWindow/ChatTabControl/GuildChatWindow/GuildChatListBox");
				break;
			case CHAT_TYPE_MAP:
				s << sender << " shouted: " << message << std::endl;
				text=s.str();
				this->AddChatText(text.c_str(),"ChatWindow/ChatTabControl/MainChatWindow/MainChatListBox");
				break;
			case CHAT_TYPE_PARTY:
				s << sender << " from your party said: " << message << std::endl;
				text=s.str();
				this->AddChatText(text.c_str(),"ChatWindow/ChatTabControl/PartyChatWindow/PartyChatListBox");
				break;
			case CHAT_TYPE_SYSTEM:
				s << message << std::endl;
				text=s.str();
				this->AddChatText(text.c_str(),"ChatWindow/ChatTabControl/MiscChatWindow/MiscChatListBox");
				break;
			case CHAT_TYPE_WHISPER:
				s << sender << " whispered: " << message << std::endl;
				text=s.str();
				this->AddChatText(text.c_str(),"ChatWindow/ChatTabControl/MainChatWindow/MainChatListBox");
				this->AddChatText(text.c_str(),"ChatWindow/ChatTabControl/PartyChatWindow/PartyChatListBox");
				this->AddChatText(text.c_str(),"ChatWindow/ChatTabControl/GuildChatWindow/GuildChatListBox");
				this->AddChatText(text.c_str(),"ChatWindow/ChatTabControl/MiscChatWindow/MiscChatListBox");
				break;
			default:
				s << sender << " spoke strangely: " << message << std::endl; //this shouldn't happen.
				text=s.str();
				this->AddChatText(text.c_str(),"ChatWindow/ChatTabControl/MiscChatWindow/MiscChatListBox");
				break;
		}
	}

	void InGameChatManager::onNpcResponse(size_t npcId, const std::string& response, const std::vector<std::string>& playerResponses)
	{

	}
};