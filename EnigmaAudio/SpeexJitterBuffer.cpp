/*
Copyright © 2011  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

//inspired by the jitter bufffer created by Jean-Marc Valin.

#include "SpeexJitterBuffer.hpp"

namespace Enigma
{
	SpeexJitterBuffer::SpeexJitterBuffer()
	{
	   this->mAreBitsValid=false;
	   this->mPackets=NULL;
	   this->mDecoder=NULL;
	   this->mFrameSize=0;
	}

	SpeexJitterBuffer::~SpeexJitterBuffer()
	{

	}

	void SpeexJitterBuffer::Init(void* decoder, int sampling_rate)
	{
	   this->mDecoder = decoder;
	   speex_decoder_ctl(decoder, SPEEX_GET_FRAME_SIZE, &this->mFrameSize);

	   this->mPackets = jitter_buffer_init(this->mFrameSize);

	   speex_bits_init(&this->mCurrentPacket);
	   this->mAreBitsValid = 0;
	}

	void SpeexJitterBuffer::Destroy()
	{
	   jitter_buffer_destroy(this->mPackets);
	   speex_bits_destroy(&this->mCurrentPacket);
	}

	void SpeexJitterBuffer::Put(char* packet, int len, int timestamp)
	{
	   JitterBufferPacket p;

	   p.data = packet;
	   p.len = len;
	   p.timestamp = timestamp;
	   p.span = this->mFrameSize;
	   p.sequence = 0; //sequence not available but struct won't be initialized to zero.
	   p.user_data = 0; //user_data not used but struct won't be initialized to zero.

	   jitter_buffer_put(this->mPackets, &p);
	}

	void SpeexJitterBuffer::Get(Enigma::s16* out, int* current_timestamp)
	{
	   int i;
	   int ret;
	   spx_int32_t activity;
	   char data[VOICE_CHAT_BUFFER_SIZE];
	   JitterBufferPacket packet;

	   packet.data = data;
	   packet.len = VOICE_CHAT_BUFFER_SIZE; //This must be higher than the packet stored.
	   packet.timestamp = 0; //timestamp not used (I hope) but struct won't be initialized to zero.
	   packet.span = this->mFrameSize;
	   packet.sequence = 0; //sequence not available but struct won't be initialized to zero.
	   packet.user_data = 0; //user_data not used but struct won't be initialized to zero.

	   if (this->mAreBitsValid)
	   {
		  /* Try decoding last received packet */
		  ret = speex_decode_int(this->mDecoder, &this->mCurrentPacket, out);
		  if (ret == 0)
		  {
			 jitter_buffer_tick(this->mPackets);
			 return;
		  } 
		  else 
		  {
			 this->mAreBitsValid = 0;
		  }
	   }

	   ret = jitter_buffer_get(this->mPackets, &packet, this->mFrameSize, NULL);
	   
	   if (ret != JITTER_BUFFER_OK)
	   {
		  /* No packet found */

		  /*Packet is late or lost*/
		  speex_decode_int(this->mDecoder, NULL, out);
	   } 
	   else 
	   {
		  speex_bits_read_from(&this->mCurrentPacket, packet.data, packet.len);
		  /* Decode packet */
		  ret = speex_decode_int(this->mDecoder, &this->mCurrentPacket, out);
		  if (ret == 0)
		  {
			 this->mAreBitsValid = 1;
		  } 
		  else 
		  {
			 /* Error while decoding */
			for (i=0;i<this->mFrameSize;i++)
			{
				out[i]=0;
			}
		  }
	   }
	   speex_decoder_ctl(this->mDecoder, SPEEX_GET_ACTIVITY, &activity);
	   if (activity < 30)
	   {
		  jitter_buffer_update_delay(this->mPackets, &packet, NULL);
	   }
	   jitter_buffer_tick(this->mPackets);
	}

	int SpeexJitterBuffer::GetPointerTimestamp()
	{
		return jitter_buffer_get_pointer_timestamp(this->mPackets);
	}
};