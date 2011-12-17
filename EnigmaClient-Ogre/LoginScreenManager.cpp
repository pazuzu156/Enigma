/*
Copyright © 2011  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "LoginScreenManager.hpp"

namespace Enigma
{
    LoginScreenManager::LoginScreenManager(ClientTransmissionManager& clientTransmissionManager)
		: ScreenManager(clientTransmissionManager)
    {

    }

    LoginScreenManager::~LoginScreenManager()
    {

    }

	void LoginScreenManager::RegisterEvents()
	{
		CEGUI::Window* window = NULL;

		window = CEGUI::WindowManager::getSingleton().getWindow("LoginButton");
		if(window!=NULL)
		{
			window->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&LoginScreenManager::onLoginButtonClicked, this));
		}

		window = CEGUI::WindowManager::getSingleton().getWindow("ExitButton");
		if(window!=NULL)
		{
			window->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&LoginScreenManager::onExitButtonClicked, this));
		}

		window = CEGUI::System::getSingleton().getGUISheet();
		if(window!=NULL)
		{
			window->subscribeEvent(CEGUI::PushButton::EventKeyDown, CEGUI::Event::Subscriber(&LoginScreenManager::onKeyDown, this));
		}
	}

	bool LoginScreenManager::onKeyDown(const CEGUI::EventArgs &args)
	{
		const CEGUI::KeyEventArgs& keyEvent = static_cast<const CEGUI::KeyEventArgs&>(args);

		if(keyEvent.scancode == CEGUI::Key::Escape)
		{
			//in login screen there is not past state so escape means exit.
			EnigmaEvent enigmaEvent;
			enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_EXIT;
			this->mClientTransmissionManager.GetClientSessionManager().PushApplicationEvent(enigmaEvent);	

			return true;
		}
		else
		{

			return false;
		}
	}

	bool LoginScreenManager::onLoginButtonClicked(const CEGUI::EventArgs& e)
	{
		std::string username;
		std::string password;

		CEGUI::Editbox* usernameEditbox = static_cast<CEGUI::Editbox*> (CEGUI::WindowManager::getSingleton().getWindow("UsernameEditbox"));
		if(usernameEditbox!=NULL)
		{
			username = usernameEditbox->getText().c_str();
		}

		CEGUI::Editbox* passwordEditbox = static_cast<CEGUI::Editbox*> (CEGUI::WindowManager::getSingleton().getWindow("PasswordEditbox"));
		if(passwordEditbox!=NULL)
		{
			password = passwordEditbox->getText().c_str();
		}

		this->mClientTransmissionManager.GetClientSessionManager().Login(username,password);

		return true;
	}

	bool LoginScreenManager::onExitButtonClicked(const CEGUI::EventArgs& e)
	{
		EnigmaEvent enigmaEvent;
		enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_EXIT;
		this->mClientTransmissionManager.GetClientSessionManager().PushApplicationEvent(enigmaEvent);

		return true;
	}
};