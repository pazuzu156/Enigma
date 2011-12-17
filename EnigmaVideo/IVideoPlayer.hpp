#ifndef IVIDEOPLAYER_HPP_INCLUDED
#define IVIDEOPLAYER_HPP_INCLUDED

/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "string.hpp"
#include "iostream.hpp"

#include "Irrlicht.hpp"

#include "TypeDefs.hpp"
#include "EnigmaException.hpp"

namespace Enigma
{
    class DllExport IVideoPlayer
        : public irr::gui::IGUIElement
    {

        private:

        protected:

        public:

        IVideoPlayer(
            irr::gui::IGUIEnvironment *environment,
            irr::gui::IGUIElement *parent,
            Enigma::s32 id,
            irr::core::rect<Enigma::s32> rectangle)
            : irr::gui::IGUIElement(irr::gui::EGUIET_ELEMENT,
                                    environment,
                                    parent?parent:environment->getRootGUIElement(),
                                    id,
                                    rectangle)
        {}

        ~IVideoPlayer(){}

        virtual void OpenVideo(std::string& uri)=0;
        virtual void OpenVideo(const Enigma::c8* uri)=0;

        virtual void Play()=0;
        virtual void Pause()=0;
        virtual void Stop()=0;

        virtual int GetFramesPerSecond()=0;
        virtual void SetFramesPerSecond(int fps)=0;
    };
};

#endif // IVIDEOPLAYER_HPP_INCLUDED
