#ifndef SCREENMANAGER_HPP_INCLUDED
#define SCREENMANAGER_HPP_INCLUDED

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
#include "GameMode.hpp"

//Start OGRE Includes
#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <SdkTrays.h>
#include <SdkCameraMan.h>
//End OGRE Includes

//Start CEGUI Includes
#include "CEGUI.h"
#include "RendererModules/Ogre/CEGUIOgreRenderer.h"
#include "ScriptingModules/LuaScriptModule/CEGUILua.h"
//End CEGUI Includes

namespace Enigma
{
    class ScreenManager
    {
        private:

        protected:

		ClientTransmissionManager& mClientTransmissionManager;

        public:

        ScreenManager(ClientTransmissionManager& clientTransmissionManager);
        ~ScreenManager();

    };
};

#endif // LOGINSCREENMANAGER_HPP_INCLUDED