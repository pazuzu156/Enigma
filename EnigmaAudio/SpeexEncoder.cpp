/*
Copyright © 2011  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "SpeexEncoder.hpp"

#include <iostream>

namespace Enigma
{
	SpeexEncoder::SpeexEncoder()
	{
		this->mIsDisabled=true;
		Load();
	}

	SpeexEncoder::~SpeexEncoder()
	{
		Unload();
	}

	void SpeexEncoder::Load()
	{
		const SpeexMode* mode;
		mode = speex_lib_get_mode (SPEEX_MODEID_WB);

		/*Create a new encoder state in narrowband mode*/
		mState = speex_encoder_init(mode);

		/*Set the quality to 8 (15 kbps)*/
		int quality;
		quality=10;
		speex_encoder_ctl(mState, SPEEX_SET_QUALITY, &quality);

		int sampleRate;
		sampleRate=VOICE_CHAT_FREQUENCY;
		speex_encoder_ctl(mState,SPEEX_SET_SAMPLING_RATE,&sampleRate);

		//int frameSize;
		//speex_encoder_ctl(mState,SPEEX_GET_FRAME_SIZE,&frameSize);
		//std::cout << "FrameSize: " << frameSize << std::endl;

		/*Initialization of the structure that holds the bits*/
		speex_bits_init(&mBits);
	}

	void SpeexEncoder::Unload()
	{
		/*Destroy the bit-packing struct*/
		speex_bits_destroy(&mBits);

		/*Destroy the encoder state*/
		speex_encoder_destroy(mState);
	}

	Enigma::u8* SpeexEncoder::Encode(Enigma::u8* inputBuffer,size_t inputSize, size_t& outputSize)
	{
		if(!this->mIsDisabled)
		{
			speex_bits_reset(&mBits);
			speex_encode_int(mState, (Enigma::s16*)inputBuffer, &mBits);
			speex_bits_insert_terminator(&mBits);
			outputSize = speex_bits_nbytes(&mBits);

			/*Copy the bits to an array of char that can be written*/
			Enigma::u8* cbits = new Enigma::u8[outputSize]();
			speex_bits_write(&mBits, (char*)cbits, outputSize);

			return cbits;
		}
		else
		{
			Enigma::u8* cbits = new Enigma::u8[inputSize]();
			outputSize = inputSize;
			memcpy(cbits,inputBuffer,outputSize);

			return cbits;
		}
	}
};