#ifndef SPEEXDECODER_HPP_INCLUDED
#define SPEEXDECODER_HPP_INCLUDED

/*
Copyright © 2011  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include <speex/speex.h>
#include <speex/speex_jitter.h>
#include "SpeexDecoder.hpp"
#include <boost/thread.hpp> 

#include "IAudioDecoder.hpp"
#include "SpeexJitterBuffer.hpp"

namespace Enigma
{
	class DllExport SpeexDecoder : public IAudioDecoder
    {
        private:

		mutable boost::shared_mutex mMutex;

		/*Holds the state of the decoder*/
		void* mState;

		/*Holds bits so they can be read and written to by the Speex routines*/
		SpeexBits mBits;

		SpeexJitterBuffer mJitter;

		int mSampleRate;

		bool mIsDisabled;

        protected:

        public:

		SpeexDecoder();

        ~SpeexDecoder();

		void Load();

		void Unload();
		virtual Enigma::u8* Decode(Enigma::u8* inputBuffer,size_t inputSize, size_t& outputSize,Enigma::s32 timestamp);

		virtual void PushJitterBuffer(Enigma::u8* inputBuffer,size_t inputSize,Enigma::s32 timestamp);
		virtual Enigma::u8* PopJitterBuffer(size_t& outputSize);
    };
};

#endif // SPEEXDECODER_HPP_INCLUDED