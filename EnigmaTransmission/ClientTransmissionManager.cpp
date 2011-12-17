/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "ClientTransmissionManager.hpp"

namespace Enigma
{
    ClientTransmissionManager::ClientTransmissionManager()
    {
		this->PreInit();
    }

    ClientTransmissionManager::~ClientTransmissionManager()
    {
		this->Unload();
    }

    void ClientTransmissionManager::PreInit()
    {
        this->mIsStopped=false;
        this->mIsUnloaded=false;
        this->mIsConnected=false;
		this->mShouldReconnect=false;

        this->mClientLimit=1;
        this->mUpstreamLimit=0;
        this->mDownstreamLimit=0;
        this->mPollTimeout=0;

        this->mClient=NULL;
        this->mPeer=NULL;

		//Get set later.
		this->mAddress.host = ENET_HOST_ANY;
        this->mAddress.port = 6000;
    }

    void ClientTransmissionManager::Init()
    {
		//currently not used.
    }

    void ClientTransmissionManager::Load()
    {
		try
		{
			this->LoadUnsafely();
		}
		catch (Enigma::HardwareException&)
		{
			throw; //nothing to do but bailout.
		}
		catch (std::overflow_error&)
		{
			throw; //may need to pop some calls off the stack.
		}
		catch(Enigma::EnigmaException& e)
		{
			std::cout << "Enigma Exception: " << e.what() << std::endl;
		}
		catch (std::exception& e)
		{
			std::cout << "Exception: " << e.what() << std::endl;
		}
        this->mIsUnloaded=false;
    }

	void ClientTransmissionManager::LoadUnsafely()
	{
		//enet
        enet_initialize();

		this->mClient = enet_host_create(
		NULL,
		this->mClientLimit,
		this->mDownstreamLimit,
		this->mUpstreamLimit);

		//interal
		this->mClientSessionManager.SetClientTransmissionManager(this);
	}

    void ClientTransmissionManager::Unload()
    {
        if(!this->mIsUnloaded)
        {
            this->mIsUnloaded=true;

			//enet
			if(this->mClient!=NULL)
			{
				enet_host_destroy(this->mClient);
			}
            enet_deinitialize();
        }
    }

    void ClientTransmissionManager::Poll()
    {
        try
        {
            this->PollUnsafely();
        }
		catch(Enigma::EnigmaException& e)
		{
			std::cout << "Enigma Exception: " << e.what() << std::endl;
		}
		catch (Enigma::HardwareException&)
		{
			throw;
		}
		catch (std::overflow_error&)
		{
			throw; //may need to pop some calls off the stack.
		}
		catch (std::exception& e)
		{
			std::cout << "Exception: " << e.what() << std::endl;
		}
    }

	int ClientTransmissionManager::Poll(ENetEvent* event)
	{
		boost::mutex::scoped_lock lock(this->mHostMutex);
		return enet_host_service(this->mClient,event,this->mPollTimeout);
	}

    //Does one Iteration of the server loop.
    void ClientTransmissionManager::PollUnsafely()
    {
        ENetEvent event;
        //enet_uint8* message;
        Message message;
        int messageLength;
		std::stringstream s;

        if(this->mClient==NULL)
        {
            s << "Null client reference in Client::Poll()";
			this->GetClientSessionManager().Log(s);
            return; //might get set later.
        }

        /* Wait for an event. */
        while(Poll(&event) > 0)
        {
            if(this->mIsStopped)
            {
                return; //this makes sure it actually stops even if it is in the middle of polling.
            }
            switch (event.type)
            {
                case ENET_EVENT_TYPE_RECEIVE:

                    messageLength = event.packet->dataLength;

                    message.unsigned8BitIntegerPointer = static_cast<Enigma::u8*>(event.packet->data);
                    if(message.unsigned8BitIntegerPointer==NULL)
                    {
                        s << "message contains no data.";
						this->GetClientSessionManager().Log(s);
                        enet_packet_destroy(event.packet);
                        continue; //this message is null so skip and try the next one.
                    }

					switch(event.channelID)
					{
						case CHANNEL_AUTHENTICATION:
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==LoginResponseMessage::GetMessageType())
								{
									LoginResponseMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mClientSessionManager.ProcessMessage(messageContainer);
								}
								else
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==CharacterCreationResponseMessage::GetMessageType())
								{
									CharacterCreationResponseMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mClientSessionManager.ProcessMessage(messageContainer);
								}
								else
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==CharacterListResponseMessage::GetMessageType())
								{
									CharacterListResponseMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mClientSessionManager.ProcessMessage(messageContainer);
								}
								else
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==CharacterSelectionResponseMessage::GetMessageType())
								{
									CharacterSelectionResponseMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mClientSessionManager.ProcessMessage(messageContainer);
								}
								else
								{
									//unknown authentication message.
									s << "Error, unknown authentication message.";
									this->GetClientSessionManager().Log(s);
								}
							break;
						case CHANNEL_MOVEMENT:
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==MovementResponseMessage::GetMessageType())
								{
									MovementResponseMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mClientSessionManager.ProcessMessage(messageContainer);
								}
								else
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==ChangeMapResponseMessage::GetMessageType())
								{
									ChangeMapResponseMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mClientSessionManager.ProcessMessage(messageContainer);
								}
								else
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==PlayerOnMapUpdateResponseMessage::GetMessageType())
								{
									PlayerOnMapUpdateResponseMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mClientSessionManager.ProcessMessage(messageContainer);
								}
								else
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==NpcOnMapUpdateResponseMessage::GetMessageType())
								{
									NpcOnMapUpdateResponseMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mClientSessionManager.ProcessMessage(messageContainer);
								}
								else
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==MonsterOnMapUpdateResponseMessage::GetMessageType())
								{
									MonsterOnMapUpdateResponseMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mClientSessionManager.ProcessMessage(messageContainer);
								}
								else
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==ItemOnMapUpdateResponseMessage::GetMessageType())
								{
									ItemOnMapUpdateResponseMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mClientSessionManager.ProcessMessage(messageContainer);
								}
								else
								{
									//unknown movement message.
									s << "Error, unknown movement message.";
									this->GetClientSessionManager().Log(s);
								}
							break;
						case CHANNEL_COMBAT:
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==SkillResponseMessage::GetMessageType())
								{
									SkillResponseMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mClientSessionManager.ProcessMessage(messageContainer);
								}
								else
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==ItemTransferResponseMessage::GetMessageType())
								{
									ItemTransferResponseMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mClientSessionManager.ProcessMessage(messageContainer);
								}
								else
								{
									//unknown combat message.
									s << "Error, unknown combat message.";
									this->GetClientSessionManager().Log(s);
								}
							break;
						case CHANNEL_CHAT:
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==ChatResponseMessage::GetMessageType())
								{
									ChatResponseMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mClientSessionManager.ProcessMessage(messageContainer);
								}
								else
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==InviteResponseMessage::GetMessageType())
								{
									InviteResponseMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mClientSessionManager.ProcessMessage(messageContainer);
								}
								else
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==KickResponseMessage::GetMessageType())
								{
									KickResponseMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mClientSessionManager.ProcessMessage(messageContainer);
								}
								else
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==PlayerListResponseMessage::GetMessageType())
								{
									PlayerListResponseMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mClientSessionManager.ProcessMessage(messageContainer);
								}
								else
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==CreatePlayerOrganizationResponseMessage::GetMessageType())
								{
									CreatePlayerOrganizationResponseMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mClientSessionManager.ProcessMessage(messageContainer);
								}
								else
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==ModifyPlayerOrganizationRankResponseMessage::GetMessageType())
								{
									ModifyPlayerOrganizationRankResponseMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mClientSessionManager.ProcessMessage(messageContainer);
								}
								else
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==SetPlayerRankResponseMessage::GetMessageType())
								{
									SetPlayerRankResponseMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mClientSessionManager.ProcessMessage(messageContainer);
								}
								else
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==NpcChatResponseMessage::GetMessageType())
								{
									NpcChatResponseMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mClientSessionManager.ProcessMessage(messageContainer);
								}
								else
								{
									//unknown chat message.
									s << "Error, unknown chat message.";
									this->GetClientSessionManager().Log(s);
								}
							break;
						case CHANNEL_VOICE_CHAT:
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==VoiceChatResponseMessage::GetMessageType())
								{
									VoiceChatResponseMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mClientSessionManager.ProcessMessage(messageContainer);
								}
								else
								{
									//unknown voice chat message.
									s << "Error, unknown voice chat message.";
									this->GetClientSessionManager().Log(s);
								}
							break;
						default:
							break;
					}

                    /* Clean up the packet now that we're done using it. */
                    enet_packet_destroy (event.packet);
                break;
                case ENET_EVENT_TYPE_CONNECT:
                    this->mIsConnected=true;
					s << "Connected.";
					this->GetClientSessionManager().Log(s);
                break;
                case ENET_EVENT_TYPE_DISCONNECT:
                    event.peer->data = NULL;
                    this->mIsConnected=false;

					//informs event listeners that the client has been disconnected and updates session state.
					this->mClientSessionManager.onDisconnected();

					s << "Disconnected.";
					this->GetClientSessionManager().Log(s);

					if(this->mShouldReconnect)
					{
						//So a new connection can be made cleanly.
						if(this->mClient!=NULL)
						{
							enet_host_destroy(this->mClient);
							this->mClient=NULL;
						}

						//Finally attempt to get the connection back.
						this->Connect();
					}
                break;
                case ENET_EVENT_TYPE_NONE:
                    //nothing happened.
					s << "Nothing happened.";
					this->GetClientSessionManager().Log(s);
                break;
                default:
                    //api change unknown event type.
					s << "Warning Enet API may have changed.";
					this->GetClientSessionManager().Log(s);
                break;
            }
        }

		//All message have been processed give the thread a short break.
		//Any messages that come in while on said break will be processed next pass.
		boost::this_thread::sleep(boost::posix_time::milliseconds(7));
    }

	void ClientTransmissionManager::Connect(const std::string& host, int port)
    {
		std::stringstream s;

		if(host.empty())
		{
			s << "Invalid Host.";
			this->GetClientSessionManager().Log(s);
			return;
		}
		this->mHost = host;

		if(port <= 0)
		{
			s << "Invalid Port.";
			this->GetClientSessionManager().Log(s);
			return;
		}
		this->mPort = port;

		this->Connect();
    }

	void ClientTransmissionManager::Connect()
	{
		boost::mutex::scoped_lock lock(this->mHostMutex);

		std::stringstream s;

		this->mAddress.host = ENET_HOST_ANY;
		this->mAddress.port = this->mPort;

        if(enet_address_set_host(&this->mAddress, this->mHost.c_str())==0)
        {
			s << "Resolved hostname.";
			this->GetClientSessionManager().Log(s);

			if(this->mClient==NULL)
			{
				this->mClient = enet_host_create(
				NULL,
				this->mClientLimit,
				this->mDownstreamLimit,
				this->mUpstreamLimit);
			}

			if(this->mClient!=NULL)
			{
				s << "Connected to network.";
				this->GetClientSessionManager().Log(s);

				this->mPeer = enet_host_connect(this->mClient,&this->mAddress, 5);
				if (this->mPeer == NULL)
				{
					s << "Server not found.";
					this->GetClientSessionManager().Log(s);
				}
				else
				{
					this->mPeer->data=NULL;
					s << "Server found.";
					this->GetClientSessionManager().Log(s);
				}
			}
			else
			{
				s << "Unable to connect to network.";
				this->GetClientSessionManager().Log(s);
			}
        }
        else
        {
			s << "Couldn't resolve hostname.";
			this->GetClientSessionManager().Log(s);
        }
	}

    void ClientTransmissionManager::Disconnect()
    {
		boost::mutex::scoped_lock lock(this->mHostMutex);

		std::stringstream s;

		if(this->mPeer!=NULL)
		{
			enet_peer_disconnect(this->mPeer,0);
		}
    }

	void ClientTransmissionManager::Reconnect()
	{
		this->mShouldReconnect=true; //makes sure it attempts reconnect once disconnected.

		this->Disconnect(); //disconnect from server.
	}

    void ClientTransmissionManager::SendMessageToServer(MessageContainer& message)
	{
		this->ReallySendMessage(message);
	}

    void ClientTransmissionManager::ReallySendMessage(MessageContainer& message)
	{
		std::stringstream s;

        if(this->mIsConnected && this->mClient!=NULL)
        {
		    ENetPacket* packet=NULL;
			bool reliable=message.GetReliable();
			bool sent=false;
			message.UpdateMessageData(); //Only needed for some messages but it is safe to call because it is not purely virtual in base class.
            if(message.GetMessageUnion().unsigned8BitIntegerPointer!=NULL)
            {
				try
				{
					size_t messageType = message.GetType();
					size_t messageChannel = message.GetChannel();
					size_t messageLength = message.GetLength();

					//for debugging mostly.
					if(messageChannel!=CHANNEL_VOICE_CHAT)
					{
						s << "Sending message of type " << messageType << " across channel " << messageChannel << " that was " << messageLength << " long.";
						this->GetClientSessionManager().Log(s);
					}

					packet = enet_packet_create(
						message.GetMessageUnion().unsigned8BitIntegerPointer,
						messageLength,
						reliable?ENET_PACKET_FLAG_RELIABLE:ENET_PACKET_FLAG_UNSEQUENCED);

					if(packet!=NULL)
					{
						this->ReallySendMessage(this->mPeer,messageChannel,packet);
						sent=true;
						enet_host_flush(this->mClient);
					} 
				}
				catch(...)
				{
					if(!sent && packet!=NULL)
					{
						enet_packet_destroy(packet);
					}
					throw;
				}
            }
        }
	}

	void ClientTransmissionManager::ReallySendMessage(ENetPeer* peer, enet_uint8 channel, ENetPacket* packet)
	{
		boost::mutex::scoped_lock lock(this->mHostMutex);
		enet_peer_send(peer,channel,packet);
		if(channel==CHANNEL_VOICE_CHAT)
		{
			enet_host_flush(this->mClient);
		}
	}
};