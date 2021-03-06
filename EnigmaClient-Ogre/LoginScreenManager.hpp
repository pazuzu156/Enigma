#ifndef LOGINSCREENMANAGER_HPP_INCLUDED
#define LOGINSCREENMANAGER_HPP_INCLUDED

/*
Copyright � 2011  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "ScreenManager.hpp"

namespace Enigma
{
	class LoginScreenManager : public ScreenManager
    {
        private:

        protected:

        public:

        LoginScreenManager(ClientTransmissionManager& clientTransmissionManager);
        ~LoginScreenManager();

		void RegisterEvents();

		//CEGUI
		bool onKeyDown(const CEGUI::EventArgs &args);
		bool onLoginButtonClicked(const CEGUI::EventArgs& e);
		bool onExitButtonClicked(const CEGUI::EventArgs& e);
    };
};

#endif // LOGINSCREENMANAGER_HPP_INCLUDED