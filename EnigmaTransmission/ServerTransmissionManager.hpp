#ifndef SERVERTRANSMISSIONMANAGER_HPP_INCLUDED
#define SERVERTRANSMISSIONMANAGER_HPP_INCLUDED

/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "IServerTransmissionManager.hpp"
#include "ServerSessionManager.hpp"
#include "std_map.hpp"
#include "std_vector.hpp"
#include "enet.hpp"

#include "ThreadedManager.hpp"
#include <boost/thread.hpp>  

namespace Enigma
{
	class DllExport ServerTransmissionManager : public IServerTransmissionManager, public ThreadedManager
    {
        private:
		
		bool mIsStopped;
		bool mIsUnloaded;

        int mClientLimit;
        int mUpstreamLimit;
        int mDownstreamLimit;
        int mPollTimeout;

		std::map<std::string,_ENetPeer*> mPeers;
        ENetAddress mAddress;
        ENetHost* mServer;

		boost::mutex mHostMutex;

		//Send a message to a client.
        virtual void ReallySendMessageToPeer(const std::string& peerId, MessageContainer& message);

		//Send a message to a client.
		virtual void ReallySendMessageToPeer(ENetPeer* peer, enet_uint8 channel, ENetPacket* packet);

		//Send a message to all clients.
        virtual void ReallySendMessageToWorld(const std::string& peerId, MessageContainer& message);

		//Poll Enet for more packets.
		int Poll(ENetEvent* event);

        protected:

		ServerSessionManager mServerSessionManager;

        public:

        ServerTransmissionManager();
        ~ServerTransmissionManager();

		//Performs any tasks needed before init can fire.
		void PreInit();

		//Initializes needed variables.
        void Init();

		//Loads resources.
        void Load();

        void Unload();

		//Does one Iteration of the main loop.
		void PollUnsafely();

		//Does one Iteration of the main loop.
		void Poll();

		//Send a message to a client.
        virtual void SendMessageToPeer(const std::string& peerId, MessageContainer& message);

		//Send a message to a client.
        virtual void SendMessageToPeers(std::vector< std::string > peers, MessageContainer& message);

		//Send a message to all clients.
        virtual void SendMessageToWorld(const std::string& peerId, MessageContainer& message);

		ServerSessionManager& GetServerSessionManager(){return mServerSessionManager;}
    };
};

#endif // SERVERTRANSMISSIONMANAGER_HPP_INCLUDED