#ifndef SPEEXJITTERBUFFER_HPP_INCLUDED
#define SPEEXJITTERBUFFER_HPP_INCLUDED

/*
Copyright © 2011  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

//inspired by the jitter bufffer created by Jean-Marc Valin.

#include <speex/speex_jitter.h>
#include <speex/speex.h>

#include "TypeDefs.hpp"

namespace Enigma
{
    class DllExport SpeexJitterBuffer
    {
        private:

	   SpeexBits mCurrentPacket;
	   bool mAreBitsValid;
	   JitterBuffer* mPackets;
	   void* mDecoder;
	   Enigma::s32 mFrameSize;

        protected:

        public:

		SpeexJitterBuffer();
        ~SpeexJitterBuffer();

		void Init(void* decoder, int sampling_rate);
		void Destroy();
		void Put(char* packet, int len, int timestamp);
		void Get(Enigma::s16* out, int* current_timestamp); 
		int GetPointerTimestamp();

    };
};

#endif // SPEEXJITTERBUFFER_HPP_INCLUDED