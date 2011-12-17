#ifndef APPLICATION_HPP_INCLUDED
#define APPLICATION_HPP_INCLUDED

/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "ServerTransmissionManager.hpp"
#include "IApplicationEventListener.hpp"
#include "DataAccessListener.hpp"

namespace Enigma
{
	class Application : public IApplicationEventListener
    {
        private:

		bool mIsUnloaded;
		bool mIsStopped;
		ServerTransmissionManager mServerTransmissionManager;
		DataAccessListener mDataAccessListener;

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

		/*
		 * Start execution of the application by stepping through object life cycle and calling the entry point.
		 * May result in an exception at multiple stages.
		 */
		void Start(int argc, Enigma::c8** argv);

		/*
		 * Stop execution of the application by halting the progression through the object life cycle and skipping to unloading.
		 * May result in an exception if execution cannot be stopped or unloading results in an exception.
		 */
		void Stop();

		void DoApplicationEvents();
		void DoAudioEvents();
		void DoChatEvents();
		void DoSceneEvents();

		virtual void onExit();
		virtual void onChangeGameMode(size_t gameMode);
		virtual void onLog(const std::string& message);
		virtual void onOpenBrowserWindow(const std::string& url);
		virtual void onOpenVideoWindow(const std::string& url);
		virtual void onReceivedCharacterList(const std::vector<std::string>& characters){}
    };
};

#endif // APPLICATION_HPP_INCLUDED
