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

#include "ClientTransmissionManager.hpp"
#include "ClientConfigurationManager.hpp"
#include "SoundManager.hpp"

namespace Enigma
{
	class Application : public IApplicationEventListener, public IChatEventListener
    {
        private:

        bool mIsStopped;
        bool mIsUnloaded;
		size_t mGameMode;
		ClientTransmissionManager mClientTransmissionManager;
		SoundManager mSoundManager;

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
        void LoadUnsafely();

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
		void DoAudioEvents();
		void DoChatEvents();
		void DoSceneEvents();

		virtual void onExit();
		virtual void onChangeGameMode(size_t gameMode);
		virtual void onLog(const std::string& message);
		virtual void onOpenBrowserWindow(const std::string& url){}
		virtual void onOpenVideoWindow(const std::string& url){}
		virtual void onReceivedCharacterList(const std::vector<std::string>& characters){}

		virtual void onInvited(size_t chatType, size_t inviteId, const std::string& organizationName);
		virtual void onJoined(size_t chatType, const std::string& organizationName);
		virtual void onRankModified(size_t chatType, const std::string& rankName, size_t permissions);
		virtual void onModified(size_t chatType, const std::string& playerName, const std::string& rankName);
		virtual void onExpelled(size_t chatType, const std::string& organizationName, const std::string& reason);
		virtual void onReceivedMessage(size_t chatType, const std::string& message, const std::string& sender);
		virtual void onNpcResponse(size_t npcId, const std::string& response, const std::vector<std::string>& playerResponses);
    };
};

#endif // APPLICATION_HPP_INCLUDED
