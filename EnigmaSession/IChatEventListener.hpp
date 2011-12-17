#ifndef ICHATEVENTLISTENER_HPP_INCLUDED
#define ICHATEVENTLISTENER_HPP_INCLUDED

/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "TypeDefs.hpp"
#include "ChatType.hpp"

namespace Enigma
{
    class DllExport IChatEventListener
    {
        private:

        protected:

        public:

        virtual ~IChatEventListener(){};

		/*
		 * This method is called when the invited event occurs.
		 * When this event is received one or more of the listeners should be responsible for updating any related client elements.
		 * Player has been invited to join something. Guild, party, etc.
		 */
		virtual void onInvited(size_t chatType, size_t inviteId, const std::string& organizationName)=0;

		/*
		 * This method is called when the invited event occurs.
		 * When this event is received one or more of the listeners should be responsible for updating any related client elements.
		 * Player accepted invitation and joined something. Guild, party, etc.
		 */
		virtual void onJoined(size_t chatType, const std::string& organizationName)=0;

		/*
		 * This method is called when the invited event occurs.
		 * When this event is received one or more of the listeners should be responsible for updating any related client elements.
		 * 
		 */
		virtual void onRankModified(size_t chatType, const std::string& rankName, size_t permissions)=0;

		/*
		 * This method is called when the invited event occurs.
		 * When this event is received one or more of the listeners should be responsible for updating any related client elements.
		 * 
		 */
		virtual void onModified(size_t chatType, const std::string& playerName, const std::string& rankName)=0;

		/*
		 * This method is called when the invited event occurs.
		 * When this event is received one or more of the listeners should be responsible for updating any related client elements.
		 * Player was expelled/removed from something. Guild, party, etc.
		 */
		virtual void onExpelled(size_t chatType, const std::string& organizationName, const std::string& reason)=0;

		/*
		 * This method is called when the invited event occurs.
		 * When this event is received one or more of the listeners should be responsible for updating any related client elements.
		 * Player has received a message from something. Guild, party, player, world, map, etc.
		 */
		virtual void onReceivedMessage(size_t chatType, const std::string& message, const std::string& sender)=0;

		/*
		 * This method is called when the invited event occurs.
		 * When this event is received one or more of the listeners should be responsible for updating any related client elements.
		 * Player has received a response from an npc.
		 */
		virtual void onNpcResponse(size_t npcId, const std::string& response, const std::vector<std::string>& playerResponses)=0;
    };
};

#endif // ICHATEVENTLISTENER_HPP_INCLUDED