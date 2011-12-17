/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "ThreadedManager.hpp"

namespace Enigma
{
	ThreadedManager::ThreadedManager()
	{
		this->mIsRunning=false;
		this->mIsLoaded=false;
	}

	ThreadedManager::~ThreadedManager()
	{

	}

	bool ThreadedManager::GetIsRunning() const
	{
		return this->mIsRunning;
	}

	bool ThreadedManager::GetIsLoaded() const
	{
		return this->mIsLoaded;
	}

	void ThreadedManager::Start()
	{
		this->mIsRunning=true;
		this->mWorkerThread = boost::thread(&ThreadedManager::EntryPoint,this);
	}

	void ThreadedManager::Stop()
	{
		this->mIsRunning=false;
	}

	void ThreadedManager::EntryPoint()
	{
		this->Load();
		this->mIsLoaded=true; //if it got this far it is loaded.
		while(this->mIsRunning)
		{
			this->Poll();
		}
		this->Unload();
	}

	void ThreadedManager::Join()
	{
		this->mWorkerThread.join();
	}
};