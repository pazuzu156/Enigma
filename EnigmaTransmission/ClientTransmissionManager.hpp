#ifndef CLIENTTRANSMISSIONMANAGER_HPP_INCLUDED
#define CLIENTTRANSMISSIONMANAGER_HPP_INCLUDED

/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "IClientTransmissionManager.hpp"
#include "ClientSessionManager.hpp"
#include "enet.hpp"

#include "ThreadedManager.hpp"
#include <boost/thread.hpp>  

namespace Enigma
{
	class DllExport ClientTransmissionManager : public IClientTransmissionManager, public ThreadedManager
    {
        private:
		bool mShouldReconnect;
        bool mIsStopped;
        bool mIsUnloaded;
        bool mIsConnected;

        int mClientLimit;
        int mUpstreamLimit;
        int mDownstreamLimit;
        int mPollTimeout;

        ClientSessionManager mClientSessionManager;

        ENetHost* mClient;
        ENetAddress mAddress;
        ENetPeer* mPeer;

		std::string mHost;
		int mPort;

		boost::mutex mHostMutex;

		//Send a message to a server.
		virtual void ReallySendMessage(ENetPeer* peer, enet_uint8 channel, ENetPacket* packet);

		//Send a message to a server.
        virtual void ReallySendMessage(MessageContainer& message);

		//Poll Enet for more packets.
		int Poll(ENetEvent* event);

        protected:

        public:

        ClientTransmissionManager();
        ~ClientTransmissionManager();

        //Performs any tasks needed before init can fire.
        void PreInit();

        //Initializes needed variables.
        void Init();

        //Loads resources.
        void Load();

		//Loads resources.
		void LoadUnsafely();

        //Unloads resources.
        void Unload();

        //Does one Iteration of the main loop.
        void Poll();

        //Does one Iteration of the main loop.
        void PollUnsafely();

		//Connect to an Enigma Server.
		void Connect(const std::string& host, int port);

		//Connect to an Enigma Server.
		void Connect();

		//Disconnection from current Enigma Server.
        void Disconnect();

		//Disconect from the current Enigma Server and then connect again.
		virtual void Reconnect();

		//Send a message to a server.
        virtual void SendMessageToServer(MessageContainer& message);

		ClientSessionManager& GetClientSessionManager(){return mClientSessionManager;}
    };
};

#endif // CLIENTTRANSMISSIONMANAGER_HPP_INCLUDED