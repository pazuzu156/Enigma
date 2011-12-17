#ifndef SOUNDSOURCE_HPP_INCLUDED
#define SOUNDSOURCE_HPP_INCLUDED

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

#include "OpenAL.hpp"
#include "sndfile.hpp"

#include "EnigmaException.hpp"
#include "Entities\Entity.hpp"
#include "IAudioDecoder.hpp"

#include <queue>

namespace Enigma
{

    class DllExport SoundSource
    {
        private:

		//The amount of samples/packets/buffers to hold before starting playback.
		Enigma::s32 mReceiveBufferSize;

		Enigma::s32 mTimestamp;

        ALuint mSource;
        ALuint mBuffer;
        size_t mPriority;

        bool mIsStreaming;
		bool mHaveStreamBufferBeenCreated;

		IAudioDecoder* mAudioDecoder;

		std::queue<ALuint> mAvailableBuffers;
		std::queue<Enigma::u8*> mBuffers;

		ALuint mLastOpenALBuffer;
		Enigma::u8* mLastBuffer;

        protected:

		void CreateBuffers();
		void DestroyBuffers();

		void ReallyQueueBuffer();
		bool ReallyQueueBuffer(Enigma::u8* buffer);
		bool ReallyQueueOpenALBuffer(const ALuint buffer);

		void ReclaimProcessedBuffers();

		void PlayStreamIfReady();

        public:

        SoundSource(ALuint source);
        SoundSource();
		//SoundSource(const SoundSource& value);
        ~SoundSource();

		size_t GetAvailableBufferCount() const {return mAvailableBuffers.size();}

		void Init();

		void Poll();

		void QueueBuffer(Enigma::u8* encodedBuffer,size_t encodedSize);

		IAudioDecoder* GetAudioDecoder() const;
		void SetAudioDecoder(IAudioDecoder* audioDecoder);		

		Enigma::s32 GetTimestamp() const;
		void SetTimestamp(Enigma::s32 timestamp);
		void IncrementTimestamp();

        ALuint GetSource() const;
        size_t GetPriority() const;
        void SetPriority(size_t value);

		ALint GetQueuedBufferCount();
		ALint GetProcessedBufferCount();
		bool GetIsOutOfBuffers();

        void Play();
        void Play(ALuint buffer);
        void Play(const std::string& filename);
        void Play(Enigma::c8* filename);
        void Play(const Enigma::c8* filename);

        void Stop();

        void LoadAudio(const std::string& filename);
        void LoadAudio(const Enigma::c8* filename);

        ALint GetState() const;
        bool GetIsBusy() const;

        void UpdateSourcePosition(const Enigma::Entity& entity);
        void UpdateSourcePosition(f32 x,f32 y,f32 z);

        void SetBuffer(ALuint value);

        void CheckForOpenALError(bool nothrow,const Enigma::c8* functionName) const;

        void Dispose();

        bool LessThan(const SoundSource& y) const;
        bool GreaterThan(const SoundSource& y) const;

        bool operator<(const SoundSource& y) const;
        bool operator>(const SoundSource& y) const;
        bool operator<(SoundSource& y);
        bool operator>(SoundSource& y);

    };

};


#endif // SOUNDSOURCE_HPP_INCLUDED
