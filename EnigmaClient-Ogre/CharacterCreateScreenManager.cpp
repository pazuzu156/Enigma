/*
Copyright © 2011  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "CharacterCreateScreenManager.hpp"

namespace Enigma
{
    CharacterCreateScreenManager::CharacterCreateScreenManager(ClientTransmissionManager& clientTransmissionManager)
		: ScreenManager(clientTransmissionManager)
    {

    }

    CharacterCreateScreenManager::~CharacterCreateScreenManager()
    {

    }

	void CharacterCreateScreenManager::RegisterEvents()
	{
		CEGUI::Window* window = NULL;

		window = CEGUI::WindowManager::getSingleton().getWindow("CreateButton");
		if(window!=NULL)
		{
			window->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CharacterCreateScreenManager::onCreateButtonClicked, this));
		}

		window = CEGUI::WindowManager::getSingleton().getWindow("CancelButton");
		if(window!=NULL)
		{
			window->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CharacterCreateScreenManager::onCancelButtonClicked, this));
		}

		window = CEGUI::System::getSingleton().getGUISheet();
		if(window!=NULL)
		{
			window->subscribeEvent(CEGUI::PushButton::EventKeyDown, CEGUI::Event::Subscriber(&CharacterCreateScreenManager::onKeyDown, this));
		}
	}

	bool CharacterCreateScreenManager::onKeyDown(const CEGUI::EventArgs &args)
	{
		const CEGUI::KeyEventArgs& keyEvent = static_cast<const CEGUI::KeyEventArgs&>(args);

		if(keyEvent.scancode == CEGUI::Key::Escape)
		{
			//return to previous state.
			this->mClientTransmissionManager.GetClientSessionManager().RequestCharacterList();

			return true;
		}
		else
		{

			return false;
		}
	}

	bool CharacterCreateScreenManager::onCreateButtonClicked(const CEGUI::EventArgs& e)
	{
		std::string name;

		CEGUI::Editbox* nameEditbox = static_cast<CEGUI::Editbox*> (CEGUI::WindowManager::getSingleton().getWindow("NameEditbox"));
		if(nameEditbox!=NULL)
		{
			name = nameEditbox->getText().c_str();
			this->mClientTransmissionManager.GetClientSessionManager().RequestCreateCharacter(name);
		}

		return true;
	}

	bool CharacterCreateScreenManager::onCancelButtonClicked(const CEGUI::EventArgs& e)
	{
		//return to previous state.
		this->mClientTransmissionManager.GetClientSessionManager().RequestCharacterList();

		return true;
	}
};