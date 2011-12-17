/*
Copyright © 2011  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "CharacterSelectScreenManager.hpp"

namespace Enigma
{
    CharacterSelectScreenManager::CharacterSelectScreenManager(ClientTransmissionManager& clientTransmissionManager)
		: ScreenManager(clientTransmissionManager)
    {

    }

    CharacterSelectScreenManager::~CharacterSelectScreenManager()
    {

    }

	void CharacterSelectScreenManager::RegisterEvents()
	{
		CEGUI::Window* window = NULL;

		window = CEGUI::WindowManager::getSingleton().getWindow("OkButton");
		if(window!=NULL)
		{
			window->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CharacterSelectScreenManager::onOkButtonClicked, this));
		}

		window = CEGUI::WindowManager::getSingleton().getWindow("CreateButton");
		if(window!=NULL)
		{
			window->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CharacterSelectScreenManager::onCreateButtonClicked, this));
		}

		window = CEGUI::WindowManager::getSingleton().getWindow("LogoutButton");
		if(window!=NULL)
		{
			window->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CharacterSelectScreenManager::onLogoutButtonClicked, this));
		}

		window = CEGUI::System::getSingleton().getGUISheet();
		if(window!=NULL)
		{
			window->subscribeEvent(CEGUI::PushButton::EventKeyDown, CEGUI::Event::Subscriber(&CharacterSelectScreenManager::onKeyDown, this));
		}
	}

	void CharacterSelectScreenManager::BindCharacterList(const std::vector<std::string>& characters)
	{
		CEGUI::Listbox* characterListbox = static_cast<CEGUI::Listbox*> (CEGUI::WindowManager::getSingleton().getWindow("CharacterListbox"));
		if(characterListbox!=NULL && !characters.empty())
		{
			BOOST_FOREACH(std::string character,characters)
			{
				CEGUI::ListboxTextItem* item = new CEGUI::ListboxTextItem(character.c_str());
				characterListbox->addItem(item);
			}
		}
	}

	bool CharacterSelectScreenManager::onKeyDown(const CEGUI::EventArgs &args)
	{
		const CEGUI::KeyEventArgs& keyEvent = static_cast<const CEGUI::KeyEventArgs&>(args);

		if(keyEvent.scancode == CEGUI::Key::Escape)
		{
			//return to previous state.
			this->mClientTransmissionManager.GetClientSessionManager().Logout();

			return true;
		}
		else
		{

			return false;
		}
	}

	bool CharacterSelectScreenManager::onOkButtonClicked(const CEGUI::EventArgs& e)
	{
		CEGUI::Listbox* characterListbox = static_cast<CEGUI::Listbox*> (CEGUI::WindowManager::getSingleton().getWindow("CharacterListbox"));
		if(characterListbox!=NULL && characterListbox->getItemCount()>0)
		{
			CEGUI::ListboxItem* selectedItem = characterListbox->getFirstSelectedItem();
			if(selectedItem!=NULL)
			{
				this->mClientTransmissionManager.GetClientSessionManager().SelectCharacter(characterListbox->getItemIndex(selectedItem));
			}
		}

		return true;
	}

	bool CharacterSelectScreenManager::onCreateButtonClicked(const CEGUI::EventArgs& e)
	{
		EnigmaEvent enigmaEvent;
		enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_GAME_MODE_CHANGED;
		enigmaEvent.Index = GAME_MODE_CHARACTER_CREATE;
		this->mClientTransmissionManager.GetClientSessionManager().PushApplicationEvent(enigmaEvent);

		return true;
	}

	bool CharacterSelectScreenManager::onLogoutButtonClicked(const CEGUI::EventArgs& e)
	{
		this->mClientTransmissionManager.GetClientSessionManager().Logout();

		return true;
	}
};