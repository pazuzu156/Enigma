#ifndef IAPPLICATIONEVENTLISTENER_HPP_INCLUDED
#define IAPPLICATIONEVENTLISTENER_HPP_INCLUDED

/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "TypeDefs.hpp"

namespace Enigma
{
    class DllExport IApplicationEventListener
    {
        private:

        protected:

        public:

        virtual ~IApplicationEventListener(){};

		/*
		 * This method is called when the Exit event occurs.
		 * When this event is received one of the listeners should attempt to close the client.
		 */
		virtual void onExit()=0;

		/*
		 * This method is called when the change game mode event occurs.
		 * When this event is received one or more of the listeners should attempt to change any client display or audio elements to reflect the new state.
		 */
		virtual void onChangeGameMode(size_t gameMode)=0;

		/*
		 * This method is called when the log event occurs.
		 * When this event is received the message should be displayed and or recorded by one or more of the listeners.
		 */
		virtual void onLog(const std::string& message)=0;

		/*
		 * This method is called when the open browser window event occurs.
		 * When this event is received one of the listeners should be responsible for opening a browser window inside the client.
		 */
		virtual void onOpenBrowserWindow(const std::string& url)=0;

		/*
		 * This method is called when the open video window event occurs.
		 * When this event is received one of the listeners should be responsible for opening a video window inside the client.
		 */
		virtual void onOpenVideoWindow(const std::string& url)=0;

		/*
		 * This method is called when the received character list event occurs.
		 * When this event is received one of the listeners should be responsible for displaying available characters to the user.
		 */
		virtual void onReceivedCharacterList(const std::vector<std::string>& characters)=0;
    };
};

#endif // IAPPLICATIONEVENTLISTENER_HPP_INCLUDED