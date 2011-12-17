/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

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
	{
		this->PreInit();
	}

	Application::~Application()
	{
		this->Unload();
	}

	void Application::PreInit()
	{
		this->mIsUnloaded=true;
		this->mIsStopped=false;
	}

	void Application::Init(int argc, Enigma::c8** argv)
	{
		//initialize sub servers.
		this->mServerTransmissionManager.Init();

		//Allows logs and other application level events to function.
		this->mServerTransmissionManager.GetServerSessionManager().RegisterApplicationEventListener(this);
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

		//start sub servers.
		this->mServerTransmissionManager.Start();

	}

	void Application::Unload()
	{
		if(!this->mIsUnloaded)
		{
			this->mIsUnloaded=true;

			//stop sub servers.
			this->mServerTransmissionManager.Stop();
		}
	}

    void Application::Start(int argc, Enigma::c8** argv)
    {
		//enter main loop.
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
		this->mServerTransmissionManager.GetServerSessionManager().DoApplicationEvents();
	}

	void Application::DoAudioEvents()
	{
		this->mServerTransmissionManager.GetServerSessionManager().DoAudioEvents();
	}

	void Application::DoChatEvents()
	{
		this->mServerTransmissionManager.GetServerSessionManager().DoChatEvents();
	}

	void Application::DoSceneEvents()
	{
		this->mServerTransmissionManager.GetServerSessionManager().DoSceneEvents();
	}

	void Application::onExit()
	{
		this->mIsStopped=true; //should cause program to halt.
	}

	void Application::onChangeGameMode(size_t gameMode)
	{
		//shouldn't ever fire on server.
	}

	void Application::onLog(const std::string& message)
	{
		std::cout << "Log: " << message << std::endl;
	}

	void Application::onOpenBrowserWindow(const std::string& url)
	{
		//Shouldn't ever fire on server.
	}

	void Application::onOpenVideoWindow(const std::string& url)
	{
		//Shouldn't ever fire on server.
	}

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

	void Application::Poll()
	{
		DoApplicationEvents();
		DoChatEvents();
		DoSceneEvents();
		DoAudioEvents();

		boost::posix_time::milliseconds workTime(100);
		boost::this_thread::sleep(workTime);
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
}
