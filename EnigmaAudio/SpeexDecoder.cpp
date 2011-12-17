/*
Copyright © 2011  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "SpeexDecoder.hpp"

#include <iostream>

namespace Enigma
{
	SpeexDecoder::SpeexDecoder()
	{
		this->mIsDisabled=true;
		Load();
	}

	SpeexDecoder::~SpeexDecoder()
	{
		Unload();
	}

	void SpeexDecoder::Load()
	{
		const SpeexMode* mode;
		mode = speex_lib_get_mode (SPEEX_MODEID_WB);

		/*Create a new decoder state in narrowband mode*/
		mState = speex_decoder_init(mode);

		/*Set the quality to 8 (15 kbps)*/
		int quality;
		quality=10;
		speex_decoder_ctl(mState, SPEEX_SET_QUALITY, &quality);

		/*Set the perceptual enhancement on*/
		//int tmp;
		//tmp=1;
		//speex_decoder_ctl(mState, SPEEX_SET_ENH, &tmp);

		this->mSampleRate=VOICE_CHAT_FREQUENCY;
		speex_decoder_ctl(mState,SPEEX_SET_SAMPLING_RATE,&mSampleRate);

		//int frameSize;
		//speex_decoder_ctl(mState,SPEEX_GET_FRAME_SIZE,&frameSize);
		//std::cout << "FrameSize: " << frameSize << std::endl;

		/*Initialization of the structure that holds the bits*/
		speex_bits_init(&mBits);

		/*Setup jitter buffer*/
		this->mJitter.Init(mState, mSampleRate);
	}

	void SpeexDecoder::Unload()
	{
		/*Destroy the bit-stream truct*/
		speex_bits_destroy(&mBits);

		/*Destroy the decoder state*/
		speex_decoder_destroy(mState);

		this->mJitter.Destroy();
	}

	Enigma::u8* SpeexDecoder::Decode(Enigma::u8* inputBuffer,size_t inputSize, size_t& outputSize,Enigma::s32 timestamp)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mMutex);

		outputSize = VOICE_CHAT_BUFFER_SIZE;
		Enigma::u8* cbits = new Enigma::u8[VOICE_CHAT_BUFFER_SIZE]();

		if(!this->mIsDisabled)
		{
			speex_bits_reset(&mBits);
			speex_bits_read_from(&mBits, (char*)inputBuffer, inputSize);
			outputSize = speex_bits_nbytes(&mBits);

			speex_decode_int(mState,&mBits,(Enigma::s16*)cbits);
		}
		else
		{
			outputSize = inputSize;
			memcpy(cbits,inputBuffer,outputSize);
		}


		//debug
		//std::cout << "OutputSize: " << outputSize << " InputSize: " << inputSize << " BufferSize: " << VOICE_CHAT_BUFFER_SIZE << std::endl;

		return cbits;
	}

	void SpeexDecoder::PushJitterBuffer(Enigma::u8* inputBuffer,size_t inputSize,Enigma::s32 timestamp)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mMutex);

		this->mJitter.Put((Enigma::s8*)inputBuffer, inputSize, timestamp);
	}
	
	Enigma::u8* SpeexDecoder::PopJitterBuffer(size_t& outputSize)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mMutex);

		outputSize = VOICE_CHAT_BUFFER_SIZE;
		Enigma::u8* cbits = new Enigma::u8[VOICE_CHAT_BUFFER_SIZE]();

		this->mJitter.Get((Enigma::s16*)cbits, NULL);

		return cbits;
	}
};