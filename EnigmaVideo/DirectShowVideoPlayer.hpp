#ifndef DIRECTSHOWVIDEOPLAYER_HPP_INCLUDED
#define DIRECTSHOWVIDEOPLAYER_HPP_INCLUDED

/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

//Based on Emil_halim's TMovie class.

#ifdef ENIGMA_PLATFORM_WINDOWS

#pragma warning( push )
#pragma warning( disable : 4061 )
#pragma warning( disable : 4100 )
#pragma warning( disable : 4127 )
#pragma warning( disable : 4217 )
#pragma warning( disable : 4242 )
#pragma warning( disable : 4244 )
#pragma warning( disable : 4280 )
#pragma warning( disable : 4514 )
#pragma warning( disable : 4619 )
#pragma warning( disable : 4668 )
#pragma warning( disable : 4710 )
#pragma warning( disable : 4820 )
    #include "windows.h"
    #include <dshow.h>
    #include <mmstream.h>
    #include <amstream.h>
    #include <ddstream.h>
#pragma warning( pop )

#include "IVideoPlayer.hpp"

namespace Enigma
{
    static GUID MY_CLSID_AMMultiMediaStream={0x49C47CE5,0x9BA4,0x11D0,0x82,0x12,0x00,0xC0,0x4F,0xC3,0x2C,0x45};
    static GUID MY_IID_IAMMultiMediaStream={0xBEBE595C,0x9A6F,0x11D0,0x8F,0xDE,0x00,0xC0,0x4F,0xD9,0x18,0x9D};
    static GUID MY_MSPID_PrimaryVideo={0xA35FF56A,0x9FDA,0x11D0,0x8F,0xDF,0x00,0xC0,0x4F,0xD9,0x18,0x9D};
    static GUID MY_IID_IDirectDrawMediaStream={0xF4104FCE,0x9A70,0x11D0,0x8F,0xDE,0x00,0xC0,0x4F,0xD9,0x18,0x9D};
    static GUID MY_MSPID_PrimaryAudio={0xA35FF56B,0x9FDA,0x11D0,0x8F,0xDF,0x00,0xC0,0x4F,0xD9,0x18,0x9D};

    class DllExport DirectShowVideoPlayer
        : public IVideoPlayer
    {

        private:

        //directshow stuff
        IAMMultiMediaStream* mAMStream;
        IMediaStream* mPrimaryVideoStream;
        IDirectDrawMediaStream* mDDStream;
        IDirectDrawStreamSample* mSample;
        IDirectDrawSurface* mSurface;
        RECT mMovieRect;
        LONG mMoviePitch;
        void* mMovieBuffer;
        DWORD mTime;
        DWORD mOldTick;

        //irrlicht stuff
        irr::video::IVideoDriver* mDriver;
        irr::gui::IGUISkin* mSkin;
        irr::video::SColor mColor;
        irr::video::IImage* mImage;
        irr::video::ITexture* mBuffer;

        //other stuff
        bool mIsDirty;

        protected:

        void LockMovie();
        void UnlockMovie();
        void UpdateMovieFrame();
        void DrawMovie();

        public:

        DirectShowVideoPlayer(
            irr::gui::IGUIEnvironment *environment,
            irr::gui::IGUIElement *parent,
            Enigma::s32 id,
            irr::core::rect<Enigma::s32> rectangle);

        ~DirectShowVideoPlayer();

        virtual void OpenVideo(std::string& uri);
        virtual void OpenVideo(const Enigma::c8* uri);

        virtual void Play();
        virtual void Pause();
        virtual void Stop();

        virtual int GetFramesPerSecond();
        virtual void SetFramesPerSecond(int fps);

        //irrlicht interface
        virtual void OnPostRender(Enigma::u32 timeMs);
        virtual void draw();
    };
};

#endif

#endif // DIRECTSHOWVIDEOPLAYER_HPP_INCLUDED
