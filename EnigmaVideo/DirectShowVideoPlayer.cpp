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

#include "DirectShowVideoPlayer.hpp"

namespace Enigma
{

    DirectShowVideoPlayer::DirectShowVideoPlayer(
        irr::gui::IGUIEnvironment *environment,
        irr::gui::IGUIElement *parent,
        Enigma::s32 id,
        irr::core::rect<Enigma::s32> rectangle)
            : IVideoPlayer(
                environment,
                parent?parent:environment->getRootGUIElement(),
                id,
                rectangle)
    {
        CoInitialize(0);

        this->mAMStream = NULL;
        this->mPrimaryVideoStream = NULL;
        this->mDDStream = NULL;
        this->mSample = NULL;
        this->mSurface = NULL;
        this->mTime = NULL;

        this->mDriver=environment->getVideoDriver();
        this->mSkin=environment->getSkin();
        this->mColor = irr::video::SColor(255,255,255,255);
        this->mBuffer=NULL;
        this->mImage=NULL;

        this->SetFramesPerSecond(72); //72 should be enough.
    }

    DirectShowVideoPlayer::~DirectShowVideoPlayer()
    {
        //directshow stuff
        if(this->mPrimaryVideoStream!=NULL)
        {
            this->mPrimaryVideoStream->Release();
        }

        if(this->mDDStream!=NULL)
        {
            this->mDDStream->Release();
        }

        if(this->mSample!=NULL)
        {
            this->mSample->Release();
        }

        if(this->mSurface!=NULL)
        {
            this->mSurface->Release();
        }

        if(this->mAMStream!=NULL)
        {
            this->mAMStream->Release();
        }

        CoUninitialize();

        //irrlicht stuff
        if(this->mBuffer)
        {
            this->mBuffer->drop();
        }
        if(this->mImage)
        {
            this->mImage->drop();
        }
    }

    void DirectShowVideoPlayer::LockMovie()
    {
        DDSURFACEDESC  directDrawSurfaceDescription;
        directDrawSurfaceDescription.dwSize=sizeof(DDSURFACEDESC);

        if(this->mSurface!=NULL)
        {
            this->mSurface->Lock(
                NULL,
                &directDrawSurfaceDescription,
                DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,
                NULL);

            this->mMoviePitch  = directDrawSurfaceDescription.lPitch;
            this->mMovieBuffer = directDrawSurfaceDescription.lpSurface;
        }
    }

    void DirectShowVideoPlayer::UnlockMovie()
    {
        if(this->mSurface!=NULL)
        {
            this->mSurface->Unlock(NULL);
        }
    }

    void DirectShowVideoPlayer::UpdateMovieFrame()
    {
        if(GetTickCount()-this->mOldTick < this->mTime)
        {
            this->mIsDirty = false;
        }
        else
        {
            this->mIsDirty = true;
            this->mOldTick = GetTickCount();
            if(this->mSample!=NULL && this->isVisible() && this->isEnabled())
            {
                this->mSample->Update(0,NULL,NULL,0);
            }
        }
    }

    void DirectShowVideoPlayer::DrawMovie()
    {
        if(this->mBuffer!=NULL && this->mSurface!=NULL && this->mIsDirty)
        {
            int x=0;
            int y=0;
            void* pBits = this->mBuffer->lock();
            LONG  Pitch = this->mBuffer->getPitch();
            DDSURFACEDESC  directDrawSurfaceDescription;
            directDrawSurfaceDescription.dwSize=sizeof(DDSURFACEDESC);
            this->mSurface->Lock( NULL,&directDrawSurfaceDescription, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT , NULL);
            int wmin=(Pitch<directDrawSurfaceDescription.lPitch)?Pitch:directDrawSurfaceDescription.lPitch;
            try
            {
                for(size_t h=0; h<directDrawSurfaceDescription.dwHeight; h++)
                {
                    memcpy((BYTE*)pBits+((y+h)*Pitch)+x*4,(BYTE*)directDrawSurfaceDescription.lpSurface+h*directDrawSurfaceDescription.lPitch,wmin);
                }
            }
            catch(...)
            {
                //std::cout << "Unknown Exception in DirectShowVideoPlayer::DrawMovie()" << std::endl;
            }

            this->mSurface->Unlock(NULL);
            this->mBuffer->unlock();
        }
    }

    void DirectShowVideoPlayer::OpenVideo(std::string& uri)
    {
        this->OpenVideo(uri.c_str());
    }

    void DirectShowVideoPlayer::OpenVideo(const Enigma::c8* uri)
    {
		try
		{
			size_t bufferSize = strlen(uri);
			wchar_t* buffer = new wchar_t[bufferSize];
			MultiByteToWideChar(CP_ACP,0,uri,-1,buffer,bufferSize);
			CoCreateInstance(MY_CLSID_AMMultiMediaStream,0,1,MY_IID_IAMMultiMediaStream,(void**)&this->mAMStream);
			this->mAMStream->Initialize((STREAM_TYPE)0,0,NULL);
			this->mAMStream->AddMediaStream(0,&MY_MSPID_PrimaryVideo,0,NULL);
			this->mAMStream->AddMediaStream(0,&MY_MSPID_PrimaryAudio,AMMSF_ADDDEFAULTRENDERER,NULL);
			this->mAMStream->OpenFile(buffer,4);
			this->mAMStream->GetMediaStream(MY_MSPID_PrimaryVideo, &this->mPrimaryVideoStream);
			this->mPrimaryVideoStream->QueryInterface(MY_IID_IDirectDrawMediaStream,(void**)&this->mDDStream);
			this->mDDStream->CreateSample(0,0,0,&this->mSample);
			this->mSample->GetSurface(&this->mSurface,&this->mMovieRect);
			this->mAMStream->SetState(STREAMSTATE_RUN);
			delete[] buffer;

			if(this->mImage==NULL)
			{
				DDSURFACEDESC  directDrawSurfaceDescription;
				directDrawSurfaceDescription.dwSize=sizeof(DDSURFACEDESC);
				this->mSurface->GetSurfaceDesc(&directDrawSurfaceDescription);

				#ifdef IRRLICHT_VERSION_1_5
				this->mImage = this->mDriver->createImage(
					irr::video::ECF_R8G8B8,
					//irr::core::dimension2d<Enigma::s32>(this->getAbsoluteClippingRect().getWidth(),this->getAbsoluteClippingRect().getHeight()));
					irr::core::dimension2d<Enigma::s32>(directDrawSurfaceDescription.dwWidth,directDrawSurfaceDescription.dwHeight));
				#else
				this->mImage = this->mDriver->createImage(
					irr::video::ECF_R8G8B8,
					//irr::core::dimension2d<Enigma::u32>(this->getAbsoluteClippingRect().getWidth(),this->getAbsoluteClippingRect().getHeight()));
					irr::core::dimension2d<Enigma::u32>(directDrawSurfaceDescription.dwWidth,directDrawSurfaceDescription.dwHeight));
				#endif
			}
			if(this->mBuffer==NULL)
			{
				this->mBuffer=this->mDriver->addTexture("EnigmaVideoPlayer",this->mImage);
			}
		}
		catch(Enigma::EnigmaException& e)
		{
			std::cout << "Enigma Exception: " << e.what() << std::endl;
		}
		catch (Enigma::HardwareException&)
		{
			throw;
		}
		catch (std::overflow_error&)
		{
			throw; //may need to pop some calls off the stack.
		}
		catch (std::exception& e)
		{
			std::cout << "Exception: " << e.what() << std::endl;
		}
        catch(...)
        {
            std::cout << "Unknown Exception in DirectShowVideoPlayer::OpenVideo(const Enigma::c8* uri)" << std::endl;
        }
    }

    void DirectShowVideoPlayer::Play()
    {
        this->mAMStream->SetState(STREAMSTATE_RUN);
    }

    void DirectShowVideoPlayer::Pause()
    {
        this->mAMStream->SetState(STREAMSTATE_STOP);
    }

    void DirectShowVideoPlayer::Stop()
    {
        this->mAMStream->SetState(STREAMSTATE_STOP);
    }

    int DirectShowVideoPlayer::GetFramesPerSecond()
    {
        //1000 = number of milliseconds in a second.
        return this->mTime*1000;
    }

    void DirectShowVideoPlayer::SetFramesPerSecond(int fps)
    {
        //1000 = number of milliseconds in a second.
        this->mTime = 1000/fps;
    }

    void DirectShowVideoPlayer::OnPostRender(Enigma::u32 timeMs)
    {
        //sample isn't updated unless visable and enabled so check isn't needed here.
        this->UpdateMovieFrame();
    }

    void DirectShowVideoPlayer::draw()
    {
        if(this->isVisible() && this->isEnabled())
        {
            this->DrawMovie();
            if(this->mBuffer!=NULL)
            {
                this->mDriver->draw2DImage(
                    this->mBuffer,
                    AbsoluteClippingRect,
                    irr::core::rect<Enigma::s32>(0,0,this->mBuffer->getOriginalSize().Width,this->mBuffer->getOriginalSize().Height),
                    &AbsoluteClippingRect);
            }
            else
            {
                this->mSkin->draw2DRectangle(
                this,
                this->mSkin->getColor(irr::gui::EGDC_3D_DARK_SHADOW),
                AbsoluteRect,
                &AbsoluteClippingRect);
            }
            this->mIsDirty=false;
        }

        IGUIElement::draw();
    }

};
#endif
