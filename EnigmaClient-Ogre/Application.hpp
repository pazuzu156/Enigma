#ifndef APPLICATION_HPP_INCLUDED
#define APPLICATION_HPP_INCLUDED

/*
Copyright © 2011  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "LoginScreenManager.hpp"
#include "CharacterCreateScreenManager.hpp"
#include "CharacterSelectScreenManager.hpp"
#include "InGameScreenManager.hpp"

namespace Enigma
{
	class Application : public IApplicationEventListener, public OIS::KeyListener, public OIS::MouseListener, public Ogre::WindowEventListener
    {
        private:

//Start Misc Var
        bool mIsStopped;
        bool mIsUnloaded;
		size_t mGameMode;
		ClientTransmissionManager mClientTransmissionManager;
		SoundManager mSoundManager;
//End Misc Var

//Start Screen Var
		LoginScreenManager mLoginScreenManager;
		CharacterCreateScreenManager mCharacterCreateScreenManager;
		CharacterSelectScreenManager mCharacterSelectScreenManager;
		InGameScreenManager mInGameScreenManager;
//End Screen Var

//Start OGRE Var
		Ogre::Root *mRoot;
		Ogre::Camera* mCamera;
		Ogre::SceneManager* mSceneMgr;
		Ogre::RenderWindow* mWindow;
		Ogre::String mResourcesCfg;
		Ogre::String mPluginsCfg;
//End OGRE Var

//Start OIS Var
		OIS::InputManager* mInputManager;
		OIS::Keyboard* mKeyboard;
		OIS::Mouse* mMouse;
//End OIS Var

//Start CEGUI Var
		CEGUI::OgreRenderer* mCEGUIRenderer;
		CEGUI::Window* mLoginWindow;
		CEGUI::Window* mCharacterCreateWindow;
		CEGUI::Window* mCharacterSelectWindow;
		CEGUI::Window* mInGameWindow;
//End CEGUI Var

        protected:

		/*
		 * Initialize variables to null or other default values.
		 * This should not result in an exception.
		 */
        void PreInit();

		/*
		 * Initialize variables to sane and useable values.
		 * Can cause exceptions.
		 */
        void Init(int argc, Enigma::c8** argv);

		/*
		 * Load all required resources for execution.
		 * Can result in an exception if resources cannot be loaded.
		 */
        void Load();

		/*
		 * Load all required resources for execution.
		 * Can result in an exception if resources cannot be loaded.
		 */
        void LoadUnsafely();

		/*
		 * Load all resources required by the graphics engine including the engine itself.
		 * Can result in an exception if resources cannot be loaded.
		 */
		void LoadGraphicsEnigine();

		/*
		 * Load all resources required by the UI engine including the engine itself.
		 * Can result in an exception if resources cannot be loaded.
		 */
		void LoadUI();

		/*
		 * Load all resources required by the Input(keyboard/mouse) engine including the engine itself.
		 * Can result in an exception if resources cannot be loaded.
		 */
		void LoadInput();

		/*
		 * Unload or release all resources loaded during the load part of the life cycle.
		 *
		 */
        void Unload();

		/*
		 * Steps through the object life cycle can starts calling poll until stopped. (This is a thread entry point if applicable.)
		 * May result in an exception if any portion of the object life cycle results in an exception.
		 */
        void EnterLoop(int argc, Enigma::c8** argv);

		/*
		 * Performs executions processes.
		 * May result in an exception if any of those processes fail.
		 */
        void Poll();
		bool PollSafe();

		void ChangeGameMode(size_t gameMode);

		void windowResized(Ogre::RenderWindow* rw);
		CEGUI::uint ConvertKeyCode(const OIS::KeyEvent &arg);

        public:

        Application();
        ~Application();

        //returns true if the application has recieved a halt command else false.
        bool GetIsStopped();

        //Starts the application.
        void Start(int argc, Enigma::c8** argv);

        //Stops the application.
        void Stop();

		void DoApplicationEvents();

		//fires audio events (OpenAL claims thread safety)
		void DoAudioEvents();

		void DoChatEvents();
		void DoSceneEvents();

		//OIS events
		virtual bool keyPressed(const OIS::KeyEvent &arg);
		virtual bool keyReleased(const OIS::KeyEvent &arg);	
		virtual bool mouseMoved(const OIS::MouseEvent &arg);
		virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
		virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

		//Enigma events
		virtual void onExit();
		virtual void onChangeGameMode(size_t gameMode);
		virtual void onLog(const std::string& message);
		virtual void onOpenBrowserWindow(const std::string& url){}
		virtual void onOpenVideoWindow(const std::string& url){}
		virtual void onReceivedCharacterList(const std::vector<std::string>& characters);
    };
};

#endif // APPLICATION_HPP_INCLUDED
