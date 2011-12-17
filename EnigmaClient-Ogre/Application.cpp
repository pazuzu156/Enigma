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
		: mLoginScreenManager(mClientTransmissionManager), 
		  mCharacterCreateScreenManager(mClientTransmissionManager),
		  mCharacterSelectScreenManager(mClientTransmissionManager),
		  mInGameScreenManager(mClientTransmissionManager),
		  mSoundManager(mClientTransmissionManager)
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
		
		//Start OGRE PreInit
		this->mRoot=NULL;
		this->mCamera=NULL;
		this->mSceneMgr=NULL;
		this->mWindow=NULL;
		//End OGRE PreInit

		//Start OIS PreInit
		this->mInputManager=NULL;
		this->mKeyboard=NULL;
		this->mMouse=NULL;
		//End OIS PreInit

		//Start CEGUI PreInit
		this->mCEGUIRenderer=NULL;
		this->mLoginWindow=NULL;
		this->mCharacterCreateWindow=NULL;
		this->mCharacterSelectWindow=NULL;
		this->mInGameWindow=NULL;
		//End CEGUI PreInit	

		this->mGameMode = GAME_MODE_NONE;
	}

	void Application::Init(int argc, Enigma::c8** argv)
	{
		//initialize sub servers.
		this->mSoundManager.Init(argc,argv);
		this->mClientTransmissionManager.GetClientSessionManager().RegisterAudioEventListener(&mSoundManager);

		this->mClientTransmissionManager.Init();
		this->mClientTransmissionManager.GetClientSessionManager().RegisterApplicationEventListener(this);
		
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
		this->mSoundManager.Start();
		while(!this->mSoundManager.GetIsLoaded())
		{
			boost::posix_time::seconds workTime(1);
			boost::this_thread::sleep(workTime);
		}

		this->mClientTransmissionManager.Start();
		while(!this->mClientTransmissionManager.GetIsLoaded())
		{
			boost::posix_time::seconds workTime(1);
			boost::this_thread::sleep(workTime);
		}
		//make connection to server.
		ServerConnectionInformation info = this->mClientTransmissionManager.GetClientSessionManager().GetClientConfigurationManager().GetDefaultServer();
		this->mClientTransmissionManager.Connect(info.Host,info.Port);

		this->LoadGraphicsEnigine();

		this->LoadUI();

		this->LoadInput();

		this->ChangeGameMode(GAME_MODE_LOGIN);
	}

	void Application::LoadGraphicsEnigine()
	{
		//OGRE
		#ifdef _DEBUG
		mResourcesCfg = "../Configuration/resources_d.cfg";
		mPluginsCfg = "../Configuration/plugins_d.cfg";
		#else
		mResourcesCfg = "../Configuration/resources.cfg";
		mPluginsCfg = "../Configuration/plugins.cfg";
		#endif

		mRoot = new Ogre::Root(mPluginsCfg);

		//load OGRE configuration stuff if any.
		Ogre::ConfigFile cf;
		cf.load(mResourcesCfg);
	 
		Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
	 
		Ogre::String secName, typeName, archName;
		while (seci.hasMoreElements())
		{
			secName = seci.peekNextKey();
			Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
			Ogre::ConfigFile::SettingsMultiMap::iterator i;
			for (i = settings->begin(); i != settings->end(); ++i)
			{
				typeName = i->first;
				archName = i->second;
				Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
			}
		}

		if(/*mRoot->restoreConfig() ||*/ mRoot->showConfigDialog())
		{
			mWindow = mRoot->initialise(true, "EnigmaClient-OGRE");
		}
		else
		{
			this->mIsStopped=true;
			return;
		}

		mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);

		mCamera = mSceneMgr->createCamera("PlayerCam");
		mCamera->setPosition(Ogre::Vector3(0,0,80));
		mCamera->lookAt(Ogre::Vector3(0,0,-300));
		mCamera->setNearClipDistance(5);

		Ogre::Viewport* vp = mWindow->addViewport(mCamera);
		vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
	 
		mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

		Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

		mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

		//TODO: remove default light.
		Ogre::Light* l = mSceneMgr->createLight("MainLight");
		l->setPosition(20,80,50);
	}

	void Application::LoadUI()
	{
		//CEGUI
		Ogre::ResourceGroupManager& resourceGroupManager = Ogre::ResourceGroupManager::getSingleton();

		resourceGroupManager.createResourceGroup("imagesets");
		resourceGroupManager.createResourceGroup("fonts");
		resourceGroupManager.createResourceGroup("layouts");
		resourceGroupManager.createResourceGroup("schemes");
		resourceGroupManager.createResourceGroup("looknfeels");
		resourceGroupManager.createResourceGroup("lua_scripts");
		resourceGroupManager.createResourceGroup("schemas");

		resourceGroupManager.addResourceLocation("../UI/schemes/", "FileSystem", "schemes");
		resourceGroupManager.addResourceLocation("../UI/imagesets/", "FileSystem", "imagesets");
		resourceGroupManager.addResourceLocation("../UI/fonts/", "FileSystem", "fonts");
		resourceGroupManager.addResourceLocation("../UI/layouts/", "FileSystem", "layouts");
		resourceGroupManager.addResourceLocation("../UI/looknfeel/", "FileSystem", "looknfeel");
		resourceGroupManager.addResourceLocation("../UI/lua_scripts/", "FileSystem", "lua_scripts");

		CEGUI::Imageset::setDefaultResourceGroup("imagesets");
		CEGUI::Font::setDefaultResourceGroup("fonts");
		CEGUI::Scheme::setDefaultResourceGroup("schemes");
		CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeel");
		CEGUI::WindowManager::setDefaultResourceGroup("layouts");
		CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");

		resourceGroupManager.initialiseAllResourceGroups();

		this->mCEGUIRenderer = &CEGUI::OgreRenderer::bootstrapSystem();

		CEGUI::System::getSingleton().setScriptingModule(&CEGUI::LuaScriptModule::create());
		
		//Loads a lua script that sets defaults and such.
		CEGUI::System::getSingleton().executeScriptFile("init.lua","lua_scripts");
	}

	void Application::LoadInput()
	{
		Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
		OIS::ParamList pl;
		size_t windowHnd = 0;
		std::ostringstream windowHndStr;
	 
		mWindow->getCustomAttribute("WINDOW", &windowHnd);
		windowHndStr << windowHnd;
		pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
	 
		mInputManager = OIS::InputManager::createInputSystem( pl );
	 
		mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
		mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));
	 
		mMouse->setEventCallback(this);
		mKeyboard->setEventCallback(this);
	 
		//Set initial mouse clipping size
		windowResized(mWindow);
	 
		//Register as a Window listener
		Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
	}

	void Application::windowResized(Ogre::RenderWindow* rw)
	{
		unsigned int width, height, depth;
		int left, top;
		rw->getMetrics(width, height, depth, left, top);
	 
		const OIS::MouseState &ms = mMouse->getMouseState();
		ms.width = width;
		ms.height = height;
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
		this->mKeyboard->capture();
		this->mMouse->capture();
		//this->mSoundManager.Capture();

		DoApplicationEvents();
		DoChatEvents();
		//DoAudioEvents();
		//DoSceneEvents();

		//OGRE
		Ogre::WindowEventUtilities::messagePump();
		if(mWindow->isClosed() || !mRoot->renderOneFrame())
		{
			this->mIsStopped=true;
		}
		else
		{
			DoApplicationEvents();
			DoChatEvents();
			//DoAudioEvents();
			//DoSceneEvents();
		}

		this->mClientTransmissionManager.GetClientSessionManager().Wait();
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

	CEGUI::uint Application::ConvertKeyCode(const OIS::KeyEvent &arg)
	{
		switch(arg.key)
		{
		case OIS::KeyCode::KC_A:
			return CEGUI::Key::A;
		case OIS::KeyCode::KC_B:
			return CEGUI::Key::B;
		case OIS::KeyCode::KC_C:
			return CEGUI::Key::C;
		case OIS::KeyCode::KC_D:
			return CEGUI::Key::D;
		case OIS::KeyCode::KC_E:
			return CEGUI::Key::E;
		case OIS::KeyCode::KC_F:
			return CEGUI::Key::F;
		case OIS::KeyCode::KC_G:
			return CEGUI::Key::H;
		case OIS::KeyCode::KC_H:
			return CEGUI::Key::H;
		case OIS::KeyCode::KC_I:
			return CEGUI::Key::I;
		case OIS::KeyCode::KC_J:
			return CEGUI::Key::J;
		case OIS::KeyCode::KC_K:
			return CEGUI::Key::K;
		case OIS::KeyCode::KC_L:
			return CEGUI::Key::L;
		case OIS::KeyCode::KC_M:
			return CEGUI::Key::M;
		case OIS::KeyCode::KC_N:
			return CEGUI::Key::N;
		case OIS::KeyCode::KC_O:
			return CEGUI::Key::O;
		case OIS::KeyCode::KC_P:
			return CEGUI::Key::P;
		case OIS::KeyCode::KC_Q:
			return CEGUI::Key::Q;
		case OIS::KeyCode::KC_R:
			return CEGUI::Key::R;
		case OIS::KeyCode::KC_S:
			return CEGUI::Key::S;
		case OIS::KeyCode::KC_T:
			return CEGUI::Key::T;
		case OIS::KeyCode::KC_U:
			return CEGUI::Key::U;
		case OIS::KeyCode::KC_V:
			return CEGUI::Key::V;
		case OIS::KeyCode::KC_W:
			return CEGUI::Key::W;
		case OIS::KeyCode::KC_X:
			return CEGUI::Key::X;
		case OIS::KeyCode::KC_Y:
			return CEGUI::Key::Y;
		case OIS::KeyCode::KC_Z:
			return CEGUI::Key::Z;
		case OIS::KeyCode::KC_SPACE:
			return CEGUI::Key::Space;
		case OIS::KeyCode::KC_BACK:
			return CEGUI::Key::Backspace;
		case OIS::KeyCode::KC_RSHIFT:
			return CEGUI::Key::RightShift;
		case OIS::KeyCode::KC_LSHIFT:
			return CEGUI::Key::LeftShift;
		case OIS::KeyCode::KC_NUMPAD0:
			return CEGUI::Key::Numpad0;
		case OIS::KeyCode::KC_NUMPAD1:
			return CEGUI::Key::Numpad1;
		case OIS::KeyCode::KC_NUMPAD2:
			return CEGUI::Key::Numpad2;
		case OIS::KeyCode::KC_NUMPAD3:
			return CEGUI::Key::Numpad3;
		case OIS::KeyCode::KC_NUMPAD4:
			return CEGUI::Key::Numpad4;
		case OIS::KeyCode::KC_NUMPAD5:
			return CEGUI::Key::Numpad5;
		case OIS::KeyCode::KC_NUMPAD6:
			return CEGUI::Key::Numpad6;
		case OIS::KeyCode::KC_NUMPAD7:
			return CEGUI::Key::Numpad7;
		case OIS::KeyCode::KC_NUMPAD8:
			return CEGUI::Key::Numpad8;
		case OIS::KeyCode::KC_NUMPAD9:
			return CEGUI::Key::Numpad9;
		case OIS::KeyCode::KC_0:
			return CEGUI::Key::Zero;
		case OIS::KeyCode::KC_1:
			return CEGUI::Key::One;
		case OIS::KeyCode::KC_2:
			return CEGUI::Key::Two;
		case OIS::KeyCode::KC_3:
			return CEGUI::Key::Three;
		case OIS::KeyCode::KC_4:
			return CEGUI::Key::Four;
		case OIS::KeyCode::KC_5:
			return CEGUI::Key::Five;
		case OIS::KeyCode::KC_6:
			return CEGUI::Key::Six;
		case OIS::KeyCode::KC_7:
			return CEGUI::Key::Seven;
		case OIS::KeyCode::KC_8:
			return CEGUI::Key::Eight;
		case OIS::KeyCode::KC_9:
			return CEGUI::Key::Nine;
		case OIS::KeyCode::KC_DECIMAL:
			return CEGUI::Key::Decimal;
		case OIS::KeyCode::KC_PERIOD:
			return CEGUI::Key::Period;
		case OIS::KeyCode::KC_COMMA:
			return CEGUI::Key::Comma;
		case OIS::KeyCode::KC_SLASH:
			return CEGUI::Key::Slash;
		case OIS::KeyCode::KC_BACKSLASH:
			return CEGUI::Key::Backslash;
		case OIS::KeyCode::KC_ADD:
			return CEGUI::Key::Add;
		case OIS::KeyCode::KC_SUBTRACT:
			return CEGUI::Key::Subtract;
		case OIS::KeyCode::KC_LCONTROL:
			return CEGUI::Key::LeftControl;
		case OIS::KeyCode::KC_RCONTROL:
			return CEGUI::Key::RightControl;
		case OIS::KeyCode::KC_LMENU:
			return CEGUI::Key::LeftAlt;
		case OIS::KeyCode::KC_RMENU:
			return CEGUI::Key::RightAlt;
		case OIS::KeyCode::KC_ESCAPE:
			return CEGUI::Key::Escape;
		case OIS::KeyCode::KC_UP:
			return CEGUI::Key::ArrowUp;
		case OIS::KeyCode::KC_DOWN:
			return CEGUI::Key::ArrowDown;
		case OIS::KeyCode::KC_LEFT:
			return CEGUI::Key::ArrowLeft;
		case OIS::KeyCode::KC_RIGHT:
			return CEGUI::Key::ArrowRight;
		case OIS::KeyCode::KC_RETURN:
			return CEGUI::Key::Return;
		case OIS::KeyCode::KC_F1:
			return CEGUI::Key::F1;
		case OIS::KeyCode::KC_F2:
			return CEGUI::Key::F2;
		case OIS::KeyCode::KC_F3:
			return CEGUI::Key::F3;
		case OIS::KeyCode::KC_F4:
			return CEGUI::Key::F4;
		case OIS::KeyCode::KC_F5:
			return CEGUI::Key::F5;
		case OIS::KeyCode::KC_F6:
			return CEGUI::Key::F6;
		case OIS::KeyCode::KC_F7:
			return CEGUI::Key::F7;
		case OIS::KeyCode::KC_F8:
			return CEGUI::Key::F8;
		case OIS::KeyCode::KC_F9:
			return CEGUI::Key::F9;
		case OIS::KeyCode::KC_F10:
			return CEGUI::Key::F10;
		case OIS::KeyCode::KC_F11:
			return CEGUI::Key::F11;
		case OIS::KeyCode::KC_F12:
			return CEGUI::Key::F12;
		case OIS::KeyCode::KC_TAB:
			return CEGUI::Key::Tab;
		default:
			return CEGUI::Key::WebHome;
		}
	}

	bool Application::keyPressed(const OIS::KeyEvent &arg)
	{
		CEGUI::uint code = ConvertKeyCode(arg);
		if(code==CEGUI::Key::Tab || code==CEGUI::Key::Return || code==CEGUI::Key::Escape || !CEGUI::System::getSingleton().injectChar(arg.text))
		{
			if(!CEGUI::System::getSingleton().injectKeyDown(code))
			{
				//Event was unhandled lets roll with defaults.
				switch(this->mGameMode)
				{
					case GAME_MODE_NONE:
						break;
					case GAME_MODE_LOGIN:
						if(code==CEGUI::Key::Escape)
						{
							//in login screen there is not past state so escape means exit.
							EnigmaEvent enigmaEvent;
							enigmaEvent.EventType = EnigmaEvent::EVENT_APPLICATION_EXIT;
							this->mClientTransmissionManager.GetClientSessionManager().PushApplicationEvent(enigmaEvent);								
						}
						break;
					case GAME_MODE_SERVER_SELECT:
							//Currently not used.
						break;
					case GAME_MODE_CHARACTER_SELECT:
						if(code==CEGUI::Key::Escape)
						{
							this->mClientTransmissionManager.GetClientSessionManager().Logout();
						}
						break;
					case GAME_MODE_CHARACTER_CREATE:
						if(code==CEGUI::Key::Escape)
						{
							//create is accessed from select so return to select.
							this->mClientTransmissionManager.GetClientSessionManager().RequestCharacterList();
						}
						break;
					case GAME_MODE_IN_GAME:
						if(code==CEGUI::Key::Escape)
						{
							this->mClientTransmissionManager.GetClientSessionManager().RequestCharacterList();
						}
						if(code==CEGUI::Key::LeftShift || code==CEGUI::Key::RightShift)
						{
							this->mSoundManager.StartRecording();
						}
						break;
					case GAME_MODE_MINI_GAME:
							//Currently not used.
						break;
					default:
							//If this path is hit something has gone wrong.
						break;
				}
			}
		}	

		return true;
	}

	bool Application::keyReleased(const OIS::KeyEvent &arg)
	{
		CEGUI::uint code = ConvertKeyCode(arg);

		if(!CEGUI::System::getSingleton().injectKeyUp(code))
		{
			//Event was unhandled lets roll with defaults.
			switch(this->mGameMode)
			{
				case GAME_MODE_NONE:
					break;
				case GAME_MODE_LOGIN:
					break;
				case GAME_MODE_SERVER_SELECT:
					break;
				case GAME_MODE_CHARACTER_SELECT:
					break;
				case GAME_MODE_CHARACTER_CREATE:
					break;
				case GAME_MODE_IN_GAME:
					if(code==CEGUI::Key::LeftShift || code==CEGUI::Key::RightShift)
					{
						this->mSoundManager.StopRecording();
					}
					break;
				case GAME_MODE_MINI_GAME:
					break;
				default:
					break;
			}
		}

		return true;
	}

	bool Application::mouseMoved(const OIS::MouseEvent &arg)
	{
		CEGUI::System::getSingleton().injectMouseMove(arg.state.X.rel, arg.state.Y.rel);

		return true;
	}

	bool Application::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
	{
		switch(id)
		{
		case OIS::MouseButtonID::MB_Left:
			CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::MouseButton::LeftButton);
			break;
		case OIS::MouseButtonID::MB_Middle:
			CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::MouseButton::MiddleButton);
			break;
		case OIS::MouseButtonID::MB_Right:
			CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::MouseButton::RightButton);
			break;
		}	

		return true;
	}

	bool Application::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
	{
		switch(id)
		{
		case OIS::MouseButtonID::MB_Left:
			CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::MouseButton::LeftButton);
			break;
		case OIS::MouseButtonID::MB_Middle:
			CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::MouseButton::MiddleButton);
			break;
		case OIS::MouseButtonID::MB_Right:
			CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::MouseButton::RightButton);
			break;
		}

		return true;
	}

	void Application::onExit()
	{
		this->mIsStopped=true; //should cause program to halt.
	}

	void Application::ChangeGameMode(size_t gameMode)
	{
		CEGUI::WindowManager::getSingleton().destroyAllWindows(); //clear the old UI.

		this->mGameMode = gameMode;

		switch(gameMode)
		{
		case GAME_MODE_NONE:
			break;
		case GAME_MODE_LOGIN:
				CEGUI::System::getSingleton().executeScriptFile("login.lua","lua_scripts");
				this->mLoginWindow = CEGUI::System::getSingleton().getGUISheet();
				this->mLoginScreenManager.RegisterEvents();
			break;
		case GAME_MODE_SERVER_SELECT:
			break;
		case GAME_MODE_CHARACTER_SELECT:
				CEGUI::System::getSingleton().executeScriptFile("character_select.lua","lua_scripts");
				this->mCharacterSelectWindow = CEGUI::System::getSingleton().getGUISheet();
				this->mCharacterSelectScreenManager.RegisterEvents();
			break;
		case GAME_MODE_CHARACTER_CREATE:
				CEGUI::System::getSingleton().executeScriptFile("character_create.lua","lua_scripts");
				this->mCharacterCreateWindow = CEGUI::System::getSingleton().getGUISheet();
				this->mCharacterCreateScreenManager.RegisterEvents();
			break;
		case GAME_MODE_IN_GAME:
				CEGUI::System::getSingleton().executeScriptFile("in_game.lua","lua_scripts");
				this->mInGameWindow = CEGUI::System::getSingleton().getGUISheet();
				this->mInGameScreenManager.RegisterEvents();
			break;
		case GAME_MODE_MINI_GAME:
			break;
		default:
			break;
		}
	}

	void Application::onChangeGameMode(size_t gameMode)
	{
		this->ChangeGameMode(gameMode);
	}

	void Application::onLog(const std::string& message)
	{
		std::cout << "Log: " << message << std::endl;
	}

	void Application::onReceivedCharacterList(const std::vector<std::string>& characters)
	{
		this->mCharacterSelectScreenManager.BindCharacterList(characters);
	}
}
