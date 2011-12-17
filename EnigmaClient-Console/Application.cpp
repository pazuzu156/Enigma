/*
Copyright © 2011  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "Application.hpp"

namespace Enigma
{
	Application::Application()
		: mSoundManager(mClientTransmissionManager)
	{
		this->PreInit();
	}

	Application::~Application()
	{
		this->Unload();
	}

	void Application::PreInit()
	{
		this->mIsStopped=true;
		this->mIsUnloaded=true;
		
		this->mGameMode = 0;
	}

	void Application::Init(int argc, Enigma::c8** argv)
	{
		//initialize sub servers.
		this->mSoundManager.Init(argc,argv);


		this->mClientTransmissionManager.Init();
		this->mClientTransmissionManager.GetClientSessionManager().RegisterApplicationEventListener(this);
		this->mClientTransmissionManager.GetClientSessionManager().RegisterChatEventListener(this);
	}

	void Application::Load()
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
	}

	void Application::LoadUnsafely()
	{
		this->mIsUnloaded=false;

		this->mSoundManager.Load(); //another thread is not yet needed.

		//start sub servers.
		this->mClientTransmissionManager.Start();

		//make connection to server.
		while(!this->mClientTransmissionManager.GetIsLoaded())
		{
			boost::posix_time::seconds workTime(1);
			boost::this_thread::sleep(workTime);
		}
		ServerConnectionInformation info = this->mClientTransmissionManager.GetClientSessionManager().GetClientConfigurationManager().GetDefaultServer();
		this->mClientTransmissionManager.Connect(info.Host,info.Port);
	}

	void Application::Unload()
	{
		if(!this->mIsUnloaded)
		{
			this->mIsUnloaded=true;

			//stop sub servers.
			this->mClientTransmissionManager.Stop();
		}
	}

	void Application::Poll()
	{
		DoApplicationEvents(); //make sure initial messages are posted before requesting user input.

		char command[2048];
		std::cin.getline(command, 2048);
		this->mClientTransmissionManager.GetClientSessionManager().ProcessCommand(std::string(command));

		DoApplicationEvents();
		DoChatEvents();
		//DoSceneEvents();
		
		//May make this another thread later.
		DoAudioEvents(); 
		this->mSoundManager.Poll();
	}

	bool Application::PollSafe()
	{
		bool success=false;

		try
		{
			this->Poll();
			success=true;
		}
		catch (Enigma::HardwareException&)
		{
			throw;
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

		return success;
	}

	//Calls poll until stopped.
	void Application::EnterLoop(int argc, Enigma::c8** argv)
	{
		this->Init(argc,argv);
		this->Load();
		while(!this->mIsStopped)
		{
			this->PollSafe();
		}
		this->Unload();
	}

	void Application::Start(int argc, Enigma::c8** argv)
	{
		this->mIsStopped=false;
		this->EnterLoop(argc,argv);
	}

	void Application::Stop()
	{
		if(!this->mIsStopped)
		{
			this->mIsStopped=true;
		}
	}

	void Application::DoApplicationEvents()
	{
		this->mClientTransmissionManager.GetClientSessionManager().DoApplicationEvents();
	}

	void Application::DoAudioEvents()
	{
		this->mClientTransmissionManager.GetClientSessionManager().DoAudioEvents();
	}

	void Application::DoChatEvents()
	{
		this->mClientTransmissionManager.GetClientSessionManager().DoChatEvents();
	}

	void Application::DoSceneEvents()
	{
		this->mClientTransmissionManager.GetClientSessionManager().DoSceneEvents();
	}

	void Application::onExit()
	{
		this->mIsStopped=true; //should cause program to halt.
	}

	void Application::onChangeGameMode(size_t gameMode)
	{
		this->mGameMode = gameMode; //console client doesn't need much here no scenes to load or ui to change.
	}

	void Application::onLog(const std::string& message)
	{
		std::cout << "Log: " << message << std::endl;
	}

	void Application::onInvited(size_t chatType, size_t inviteId, const std::string& organizationName)
	{
		char anwser='n';

		switch(chatType)
		{
			case CHAT_TYPE_GUILD:
				std::cout << "You have been invited to the " << organizationName << " guild. Would you like to join? (y/n)" << std::endl;
				std::cin >> anwser;
				if(anwser=='y' || anwser=='Y')
				{
					this->mClientTransmissionManager.GetClientSessionManager().RespondToGuildInvite(true,inviteId);
				}
				else
				{
					this->mClientTransmissionManager.GetClientSessionManager().RespondToGuildInvite(false,inviteId);
				}
				break;
			case CHAT_TYPE_PARTY:
				std::cout << "You have been invited to " << organizationName  << "'s party. Would you like to join? (y/n)" << std::endl;
				std::cin >> anwser;
				if(anwser=='y' || anwser=='Y')
				{
					this->mClientTransmissionManager.GetClientSessionManager().RespondToPartyInvite(true,inviteId);
				}
				else
				{
					this->mClientTransmissionManager.GetClientSessionManager().RespondToPartyInvite(false,inviteId);
				}
				break;
			default:
				break;
		}
	}

	void Application::onJoined(size_t chatType, const std::string& organizationName)
	{
		switch(chatType)
		{
			case CHAT_TYPE_GUILD:
				std::cout << "You have joined the " << organizationName << " guild." << std::endl;
				break;
			case CHAT_TYPE_PARTY:
				std::cout << "You have joined " << organizationName  << "'s party." << std::endl;
				break;
			default:
				break;
		}
	}

	void Application::onRankModified(size_t chatType, const std::string& rankName, size_t permissions)
	{

	}

	void Application::onModified(size_t chatType, const std::string& playerName, const std::string& rankName)
	{

	}

	void Application::onExpelled(size_t chatType, const std::string& organizationName, const std::string& reason)
	{
		switch(chatType)
		{
			case CHAT_TYPE_GUILD:
				std::cout << "You have been expelled from the " << organizationName << " guild." << std::endl;
				break;
			case CHAT_TYPE_PARTY:
				std::cout << "You have been expelled from " << organizationName  << "'s party." << std::endl;
				break;
			default:
				break;
		}
	}

	void Application::onReceivedMessage(size_t chatType, const std::string& message, const std::string& sender)
	{
		switch(chatType)
		{
			case CHAT_TYPE_BROADCAST:
				std::cout << sender << " broadcast: " << message << std::endl;
				break;
			case CHAT_TYPE_GUILD:
				std::cout << sender << " from your guild said: " << message << std::endl;
				break;
			case CHAT_TYPE_MAP:
				std::cout << sender << " shouted: " << message << std::endl;
				break;
			case CHAT_TYPE_PARTY:
				std::cout << sender << " from your party said: " << message << std::endl;
				break;
			case CHAT_TYPE_SYSTEM:
				std::cout << message << std::endl;
				break;
			case CHAT_TYPE_WHISPER:
				std::cout << sender << " whispered: " << message << std::endl;
				break;
			default:
				std::cout << sender << " spoke strangely: " << message << std::endl; //this shouldn't happen.
				break;
		}
	}

	void Application::onNpcResponse(size_t npcId, const std::string& response, const std::vector<std::string>& playerResponses)
	{

	}
}
