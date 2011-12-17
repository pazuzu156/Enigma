/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "VideoPlayerGUIElementFactory.hpp"

#ifdef ENIGMA_PLATFORM_WINDOWS
#include "DirectShowVideoPlayer.hpp"
#endif

namespace Enigma
{

    VideoPlayerGUIElementFactory::VideoPlayerGUIElementFactory(IGUIEnvironment* environment)
        : mVideoPlayerTypeName("EnimgaVideoPlayer")
        , mNumberOfSupportedTypes(1)
        , mDefaultType(EGUIET_ELEMENT)
    {
        this->mEnvironment=environment;
    }

    VideoPlayerGUIElementFactory::~VideoPlayerGUIElementFactory()
    {

    }

    //irrlicht interface
    IGUIElement* VideoPlayerGUIElementFactory::addGUIElement(const c8* typeName, IGUIElement* parent)
    {
        IGUIElement* player=NULL;

        #ifdef ENIGMA_PLATFORM_WINDOWS
        if(strcmp(typeName,this->mVideoPlayerTypeName)==0)
        {
            player = new DirectShowVideoPlayer(this->mEnvironment,parent,-1,rect<s32>(0,0,800,600));
        }
        #endif

        return player;
    }

    IGUIElement* VideoPlayerGUIElementFactory::addGUIElement(EGUI_ELEMENT_TYPE type, IGUIElement* parent)
    {
        return NULL;
    }

    s32 VideoPlayerGUIElementFactory::getCreatableGUIElementTypeCount() const
    {
        return this->mNumberOfSupportedTypes;
    }

    EGUI_ELEMENT_TYPE VideoPlayerGUIElementFactory::getCreateableGUIElementType(s32 idx) const
    {
        if(idx==0)
        {
            return this->mDefaultType;
        }
        else
        {
            return this->mDefaultType;
        }
    }

    const c8* VideoPlayerGUIElementFactory::getCreateableGUIElementTypeName(EGUI_ELEMENT_TYPE type) const
    {
        if(type==EGUIET_ELEMENT)
        {
             return this->mVideoPlayerTypeName;
        }
        else
        {
            return "";
        }
    }

    const c8* VideoPlayerGUIElementFactory::getCreateableGUIElementTypeName(s32 idx) const
    {
        if(idx==0)
        {
            return this->mVideoPlayerTypeName;
        }
        else
        {
            return "";
        }
    }

    IGUIElementFactory* CreateVideoPlayerFactory(IGUIEnvironment* environment)
    {
        VideoPlayerGUIElementFactory* value=NULL;
        value = new VideoPlayerGUIElementFactory(environment);
        return value;
    }
};
