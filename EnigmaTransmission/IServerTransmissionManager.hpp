#ifndef ISERVERTRANSMISSIONMANAGER_HPP_INCLUDED
#define ISERVERTRANSMISSIONMANAGER_HPP_INCLUDED

/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "TypeDefs.hpp"
#include "Messages/MessageContainer.hpp"
#include "std_vector.hpp"

namespace Enigma
{
	class DllExport IServerTransmissionManager
    {
        private:
 
        protected:

        public:

			~IServerTransmissionManager(){};

		//Send a message to a client.
        virtual void SendMessageToPeer(const std::string& peerId, MessageContainer& message)=0;

		//Send a message to a list of clients.
		virtual void SendMessageToPeers(std::vector< std::string > peers, MessageContainer& message)=0;

		//Send a message to all clients.
        virtual void SendMessageToWorld(const std::string& peerId, MessageContainer& message)=0;
    };
};

#endif // SERVERTRANSMISSIONMANAGER_HPP_INCLUDED