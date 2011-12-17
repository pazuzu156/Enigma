#ifndef CHARACTERSELECTSCREENMANAGER_HPP_INCLUDED
#define CHARACTERSELECTSCREENMANAGER_HPP_INCLUDED

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
	class CharacterSelectScreenManager : public ScreenManager
    {
        private:

        protected:

        public:

        CharacterSelectScreenManager(ClientTransmissionManager& clientTransmissionManager);
        ~CharacterSelectScreenManager();

		void RegisterEvents();

		/*
		 * This function is called when the application has received all of the characters in the character list.
		 * This function should add the elements provided to a list and make it visible to the player for selection.
		 */
		void BindCharacterList(const std::vector<std::string>& characters);

		//CEGUI
		bool onKeyDown(const CEGUI::EventArgs &args);
		bool onOkButtonClicked(const CEGUI::EventArgs& e);
		bool onCreateButtonClicked(const CEGUI::EventArgs& e);
		bool onLogoutButtonClicked(const CEGUI::EventArgs& e);
	};
};

#endif // CHARACTERSELECTSCREENMANAGER_HPP_INCLUDED