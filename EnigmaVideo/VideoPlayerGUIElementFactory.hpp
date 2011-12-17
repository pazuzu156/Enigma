#ifndef VIDEOPLAYERGUIELEMENTFACTORY_HPP_INCLUDED
#define VIDEOPLAYERGUIELEMENTFACTORY_HPP_INCLUDED

/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "IVideoPlayer.hpp"

using namespace irr;
using namespace gui;
using namespace core;

namespace Enigma
{
    class DllExport VideoPlayerGUIElementFactory
        : public IGUIElementFactory
    {

        private:

        Enigma::c8* mVideoPlayerTypeName;
        s32 mNumberOfSupportedTypes;
        EGUI_ELEMENT_TYPE mDefaultType;

        IGUIEnvironment* mEnvironment;

        protected:

        public:

        VideoPlayerGUIElementFactory(IGUIEnvironment* environment);
        ~VideoPlayerGUIElementFactory();

        //irrlicht interface
        IGUIElement* addGUIElement(EGUI_ELEMENT_TYPE type, IGUIElement* parent=0);
        IGUIElement* addGUIElement(const c8* typeName, IGUIElement* parent=0);
        s32 getCreatableGUIElementTypeCount() const;
        EGUI_ELEMENT_TYPE getCreateableGUIElementType(s32 idx) const;
        const c8* getCreateableGUIElementTypeName(s32 idx) const;
        const c8* getCreateableGUIElementTypeName(EGUI_ELEMENT_TYPE type) const;

    };

    DllExport IGUIElementFactory* CreateVideoPlayerFactory(IGUIEnvironment* environment);
};

#endif // VIDEOPLAYERGUIELEMENTFACTORY_HPP_INCLUDED
