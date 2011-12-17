#ifndef SPEEXENCODER_HPP_INCLUDED
#define SPEEXENCODER_HPP_INCLUDED

/*
Copyright © 2011  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "IAudioEncoder.hpp"

#include <speex/speex.h>

namespace Enigma
{
	class DllExport SpeexEncoder : public IAudioEncoder
    {
        private:

		/*Holds the state of the encoder*/
		void* mState;

		/*Holds bits so they can be read and written to by the Speex routines*/
		SpeexBits mBits;

		bool mIsDisabled;

        protected:

        public:

		SpeexEncoder();

        ~SpeexEncoder();

		void Load();

		void Unload();

		virtual Enigma::u8* Encode(Enigma::u8* inputBuffer,size_t inputSize, size_t& outputSize);
    };
};

#endif // SPEEXENCODER_HPP_INCLUDED