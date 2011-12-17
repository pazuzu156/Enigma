/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "ServerTransmissionManager.hpp"

#include "Channels.hpp"
#include "boost_foreach.hpp"

namespace Enigma
{
    ServerTransmissionManager::ServerTransmissionManager()
    {
		this->PreInit();
    }

    ServerTransmissionManager::~ServerTransmissionManager()
    {
		this->Unload();
    }

	void ServerTransmissionManager::PreInit()
	{
        this->mIsStopped=false;
        this->mIsUnloaded=false;
		//

		this->mClientLimit=1000;
		this->mUpstreamLimit=0;
		this->mDownstreamLimit=0;
		this->mPollTimeout=0;

        this->mServer=NULL;
	}

    void ServerTransmissionManager::Init()
	{
        this->mServerSessionManager.Init(this);
	}

    void ServerTransmissionManager::Load()
	{
        this->mIsUnloaded=false;

        //enet
		enet_initialize();

        this->mAddress.host = ENET_HOST_ANY;
        this->mAddress.port = 6000;	

		this->mServer = enet_host_create(
            &this->mAddress,
            this->mClientLimit,
            this->mDownstreamLimit,
            this->mUpstreamLimit);

		//session
		this->mServerSessionManager.Load();

		std::stringstream s;
		s << "Server Loaded.";
		this->GetServerSessionManager().Log(s);
	}

    void ServerTransmissionManager::Unload()
	{
        if(!this->mIsUnloaded)
        {
            this->mIsUnloaded=true;

            //enet
			if(this->mServer!=NULL)
			{
				enet_host_destroy(this->mServer);
			}
            enet_deinitialize();
        }
	}

	int ServerTransmissionManager::Poll(ENetEvent* event)
	{
		boost::mutex::scoped_lock lock(this->mHostMutex);
		return enet_host_service(this->mServer,event,this->mPollTimeout);
	}

	void ServerTransmissionManager::PollUnsafely()
	{
		ENetEvent event;
		Message message;
		size_t messageLength=0;
		std::string peerId;
		std::stringstream s;

		/* Wait up to 1000 milliseconds for an event. */
		while(Poll(&event) > 0)
		{
			if(this->mIsStopped)
			{
				return; //this makes sure it actually stops even if it is in the middle of polling.
			}

			if(event.peer==NULL)
			{
				s << "Packet has no peer.";
				this->GetServerSessionManager().Log(s);
				continue; //skip this packet it is corrupt.
			}

			switch (event.type)
			{
				case ENET_EVENT_TYPE_RECEIVE:
					if(event.packet->data==NULL)
					{
						s << "Packet data is missing.";
						this->GetServerSessionManager().Log(s);
						enet_packet_destroy(event.packet);
						continue; //skip this packet it is corrupt.
					}
					//message = event.packet->data;
					messageLength = event.packet->dataLength;

					message.unsigned8BitIntegerPointer = static_cast<Enigma::u8*>(event.packet->data);

					if(event.peer->data!=NULL)
					{
						peerId = static_cast<char*>(event.peer->data);
					}

					switch(event.channelID)
					{
						case CHANNEL_AUTHENTICATION:
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==LoginRequestMessage::GetMessageType())
								{
									LoginRequestMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.

									peerId = messageContainer.GetUsername(); //breaking a rule a little.
									
									if(this->mPeers.find(peerId)==this->mPeers.end())
									{			
										this->mPeers.insert(std::pair<std::string,_ENetPeer*>(peerId,event.peer));
									}
									else
									{
										this->mPeers[peerId] = event.peer;
									}

									//this will keep the peerId from going out of scope.
									char* buffer;
									buffer = new char[strlen(peerId.c_str())];
									strcpy(buffer,peerId.c_str());
									event.peer->data = buffer;

									this->mServerSessionManager.ProcessMessage(messageContainer,peerId);
								}
								else
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==CharacterCreationRequestMessage::GetMessageType())
								{
									CharacterCreationRequestMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mServerSessionManager.ProcessMessage(messageContainer,peerId);
								}
								else
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==CharacterListRequestMessage::GetMessageType())
								{
									CharacterListRequestMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mServerSessionManager.ProcessMessage(messageContainer,peerId);
								}
								else
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==CharacterSelectionRequestMessage::GetMessageType())
								{
									CharacterSelectionRequestMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mServerSessionManager.ProcessMessage(messageContainer,peerId);
								}
								else
								{
									//unknown authentication message.
									s << "Error, unknown authentication message.";
									this->GetServerSessionManager().Log(s);
								}
							break;
						case CHANNEL_MOVEMENT:
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==MovementRequestMessage::GetMessageType())
								{
									MovementRequestMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mServerSessionManager.ProcessMessage(messageContainer,peerId);
								}
								else
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==ChangeMapRequestMessage::GetMessageType())
								{
									ChangeMapRequestMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mServerSessionManager.ProcessMessage(messageContainer,peerId);
								}
								else
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==PlayerOnMapUpdateRequestMessage::GetMessageType())
								{
									PlayerOnMapUpdateRequestMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mServerSessionManager.ProcessMessage(messageContainer,peerId);
								}
								else
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==NpcOnMapUpdateRequestMessage::GetMessageType())
								{
									NpcOnMapUpdateRequestMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mServerSessionManager.ProcessMessage(messageContainer,peerId);
								}
								else
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==MonsterOnMapUpdateRequestMessage::GetMessageType())
								{
									MonsterOnMapUpdateRequestMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mServerSessionManager.ProcessMessage(messageContainer,peerId);
								}
								else
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==ItemOnMapUpdateRequestMessage::GetMessageType())
								{
									ItemOnMapUpdateRequestMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mServerSessionManager.ProcessMessage(messageContainer,peerId);
								}
								else
								{
									//unknown movement message.
									s << "Error, unknown movement message.";
									this->GetServerSessionManager().Log(s);
								}
							break;
						case CHANNEL_COMBAT:
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==SkillRequestMessage::GetMessageType())
								{
									SkillRequestMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mServerSessionManager.ProcessMessage(messageContainer,peerId);
								}
								else
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==ItemTransferRequestMessage::GetMessageType())
								{
									ItemTransferRequestMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mServerSessionManager.ProcessMessage(messageContainer,peerId);
								}
								else
								{
									//unknown combat message.
									s << "Error, unknown combat message.";
									this->GetServerSessionManager().Log(s);
								}
							break;
						case CHANNEL_CHAT:
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==ChatRequestMessage::GetMessageType())
								{
									ChatRequestMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mServerSessionManager.ProcessMessage(messageContainer,peerId);
								}
								else
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==InviteRequestMessage::GetMessageType())
								{
									InviteRequestMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mServerSessionManager.ProcessMessage(messageContainer,peerId);
								}
								else
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==KickRequestMessage::GetMessageType())
								{
									KickRequestMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mServerSessionManager.ProcessMessage(messageContainer,peerId);
								}
								else
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==PlayerListRequestMessage::GetMessageType())
								{
									PlayerListRequestMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mServerSessionManager.ProcessMessage(messageContainer,peerId);
								}
								else
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==CreatePlayerOrganizationRequestMessage::GetMessageType())
								{
									CreatePlayerOrganizationRequestMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mServerSessionManager.ProcessMessage(messageContainer,peerId);
								}
								else
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==ModifyPlayerOrganizationRankRequestMessage::GetMessageType())
								{
									ModifyPlayerOrganizationRankRequestMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mServerSessionManager.ProcessMessage(messageContainer,peerId);
								}
								else
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==SetPlayerRankRequestMessage::GetMessageType())
								{
									SetPlayerRankRequestMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mServerSessionManager.ProcessMessage(messageContainer,peerId);
								}
								else
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==RollRequestMessage::GetMessageType())
								{
									RollRequestMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mServerSessionManager.ProcessMessage(messageContainer,peerId);
								}
								else
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==ServerTimeRequestMessage::GetMessageType())
								{
									ServerTimeRequestMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mServerSessionManager.ProcessMessage(messageContainer,peerId);
								}
								else
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==NpcChatRequestMessage::GetMessageType())
								{
									NpcChatRequestMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mServerSessionManager.ProcessMessage(messageContainer,peerId);
								}
								else
								{
									//unknown chat message.
									s << "Error, unknown chat message.";
									this->GetServerSessionManager().Log(s);
								}
							break;
						case CHANNEL_VOICE_CHAT:
								if(message.signed32BitIntegerPointer[MESSAGE_TYPE]==VoiceChatRequestMessage::GetMessageType())
								{
									VoiceChatRequestMessage messageContainer(message);
									messageContainer.GetMessageData();//Only needed for some messages but it is safe to call because it is not purely virtual in base class.
									this->mServerSessionManager.ProcessMessage(messageContainer,peerId);
								}
								else
								{
									//unknown voice chat message.
									s << "Error, unknown voice chat message.";
									this->GetServerSessionManager().Log(s);
								}
							break;
						default:
							break;
					}

					/* Clean up the packet now that we're done using it. */
					enet_packet_destroy (event.packet);
				break;
				case ENET_EVENT_TYPE_CONNECT:
					s << "A new connection has been established to " << event.peer->address.host << " : " << event.peer->address.port << ".";
					this->GetServerSessionManager().Log(s);
				break;
				case ENET_EVENT_TYPE_DISCONNECT:
					if(event.peer!=NULL && event.peer->data!=NULL)
					{
						peerId = static_cast<char*>(event.peer->data);		
						if(!peerId.empty())
						{
							s << peerId << " disconnected.";
							this->GetServerSessionManager().Log(s);

							this->mServerSessionManager.DisconnectUser(peerId);

							this->mPeers.erase(peerId);
							//delete[] event.peer->data; //this causes heap corruption now because it is actually cleaned up when the peer is erased.
							event.peer->data=NULL;
						}
						else
						{
							s << "Peer disconnected.";
							this->GetServerSessionManager().Log(s);
						}
					}
					else
					{
						s << "Peer disconnected.";
						this->GetServerSessionManager().Log(s);
					}
				break;
				case ENET_EVENT_TYPE_NONE:
					//nothing happened.
				break;
				default:
					//api change unknown event type.
					s << "Error, unknown transmission type.";
					this->GetServerSessionManager().Log(s);
				break;
			}
		}

		//All message have been processed give the thread a short break.
		//Any messages that come in while on said break will be processed next pass.
		boost::this_thread::sleep(boost::posix_time::milliseconds(3));
	}

	void ServerTransmissionManager::Poll()
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
			throw; //can't do anything might as well bail out before something bad happens.
		}
		catch (std::overflow_error&)
		{
			throw; //may need to pop some calls off the stack.
		}
		catch (std::exception& e)
		{
			std::cout << "Exception: " << e.what() << std::endl; //might be ok to continue.
		}
	}

	//Send a message to a client.
	void ServerTransmissionManager::SendMessageToPeer(const std::string& peerId, MessageContainer& message)
	{
		//boost::mutex::scoped_lock lock(this->mHostMutex);
		this->ReallySendMessageToPeer(peerId, message);
	}

	//Send a message to a client.
    void ServerTransmissionManager::SendMessageToPeers(std::vector< std::string > peers, MessageContainer& message)
	{
		//boost::mutex::scoped_lock lock(this->mHostMutex);
		BOOST_FOREACH(std::string peer,peers)
		{
			this->ReallySendMessageToPeer(peer,message);
		}
	}

	//Send a message to all clients.
	void ServerTransmissionManager::SendMessageToWorld(const std::string& peerId, MessageContainer& message)
	{
		boost::mutex::scoped_lock lock(this->mHostMutex);
		this->ReallySendMessageToWorld(peerId, message);
	}

	//Send a message to a client.
	void ServerTransmissionManager::ReallySendMessageToPeer(const std::string& peerId, MessageContainer& message)
	{
		if(peerId.empty())
		{
			return; //keep null(0) user from causing a crash.
		}
        if(this->mServer!=NULL)
        {
		    ENetPacket* packet=NULL;
			bool sent=false;
			message.UpdateMessageData(); //Only needed for some messages but it is safe to call because it is not purely virtual in base class.
            if(message.GetMessageUnion().unsigned8BitIntegerPointer!=NULL)
            {
				try
				{
					bool reliable=message.GetReliable();
					ENetPeer* peer = NULL;
					if(this->mPeers.find(peerId)!=this->mPeers.end())
					{
						peer = this->mPeers[peerId];
					}
					size_t messageType = message.GetType();
					size_t messageChannel = message.GetChannel();
					size_t messageLength = message.GetLength();

					//for debugging mostly.
					if(messageChannel!=CHANNEL_VOICE_CHAT)
					{
						std::stringstream s;
						s << "Sending message of type " << messageType << " across channel " << messageChannel << " that was " << messageLength << " long.";
						this->GetServerSessionManager().Log(s);
					}

					packet = enet_packet_create(
						message.GetMessageUnion().unsigned8BitIntegerPointer,
						messageLength,
						reliable?ENET_PACKET_FLAG_RELIABLE:ENET_PACKET_FLAG_UNSEQUENCED);

					if(packet!=NULL)
					{
						if(peer!=NULL)
						{
							ReallySendMessageToPeer(peer,messageChannel,packet);
						}
						else
						{
							throw EnigmaException("Invalid Peer!");
						}
						sent=true;
						//enet_host_flush(this->mServer);
					}
					else
					{
						throw EnigmaException("Unable to create transmission packet!");
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

	void ServerTransmissionManager::ReallySendMessageToPeer(ENetPeer* peer, enet_uint8 channel, ENetPacket* packet)
	{
		boost::mutex::scoped_lock lock(this->mHostMutex);
		enet_peer_send(peer,channel,packet);
		//enet_host_flush(this->mClient);
	}

	//Send a message to all clients.
	void ServerTransmissionManager::ReallySendMessageToWorld(const std::string& peerId, MessageContainer& message)
	{
		if(peerId.empty())
		{
			return; //keep null(0) user from causing a crash.
		}
        if(this->mServer!=NULL)
        {
		    ENetPacket* packet=NULL;
			bool sent=false;
			message.UpdateMessageData(); //Only needed for some messages but it is safe to call because it is not purely virtual in base class.
            if(message.GetMessageUnion().unsigned8BitIntegerPointer!=NULL)
            {
				try
				{
					bool reliable=message.GetReliable();
					ENetPeer* peer = NULL;
					if(this->mPeers.find(peerId)!=this->mPeers.end())
					{
						peer = this->mPeers[peerId];
					}
					size_t messageType = message.GetType();
					size_t messageChannel = message.GetChannel();
					size_t messageLength = message.GetLength();

					//for debugging mostly.
					if(messageChannel!=CHANNEL_VOICE_CHAT)
					{
						std::stringstream s;
						s << "Sending message of type " << messageType << " across channel " << messageChannel << " that was " << messageLength << " long.";
						this->GetServerSessionManager().Log(s);
					}

					packet = enet_packet_create(
						message.GetMessageUnion().unsigned8BitIntegerPointer,
						messageLength,
						reliable?ENET_PACKET_FLAG_RELIABLE:ENET_PACKET_FLAG_UNSEQUENCED);

					if(packet!=NULL)
					{
						enet_host_broadcast(this->mServer,messageChannel,packet);
						sent=true;
						enet_host_flush(this->mServer);
					}
					else
					{
						throw EnigmaException("Unable to create transmission packet!");
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
};