/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "SoundSource.hpp"

namespace Enigma
{
    SoundSource::SoundSource(ALuint source)
    {
		this->mSource=source;
		this->Init();
    }

    SoundSource::SoundSource()
    {
		this->mSource=AL_NONE;
		this->Init();
    }

	/*SoundSource::SoundSource(const SoundSource& value) 
	{
		this->Init(value.GetSource());
	}*/

    SoundSource::~SoundSource()
    {
		//leave source because they last for application lifetime but clean up buffers.
		//this->DestroyBuffers();
    }

	void SoundSource::Init()
	{
		this->mBuffer=AL_NONE;
		this->mPriority=0;
		this->mIsStreaming=false;
		this->mHaveStreamBufferBeenCreated=false;

		this->mLastOpenALBuffer=AL_NONE;
		this->mLastBuffer=NULL;

		//Variable because this may need adjusted for best results in different environments.
		this->mReceiveBufferSize = NUMBER_OF_AUDIO_BUFFERS;

		this->mTimestamp = 0;

		this->mAudioDecoder = NULL;

		if(alIsSource(this->mSource))
		{
			alSourcef(this->mSource, AL_PITCH, 1.0f);
			this->CheckForOpenALError(true,"alSourcef");

			alSourcef(this->mSource, AL_GAIN, 1.0f);
			this->CheckForOpenALError(true,"alSourcef");

			ALfloat source0Pos[]={0.0,0.0,0.0};
			alSourcefv(this->mSource, AL_POSITION, source0Pos);
			this->CheckForOpenALError(true,"alSourcefv");

			ALfloat source0Vel[]={0.0,0.0,0.0};
			alSourcefv(this->mSource, AL_VELOCITY, source0Vel);
			this->CheckForOpenALError(true,"alSourcefv");

			alSourcei(this->mSource, AL_LOOPING, AL_FALSE);
			this->CheckForOpenALError(true,"alSourcei");
		}
	}

	void SoundSource::CreateBuffers()
	{
		if(!mHaveStreamBufferBeenCreated)
		{
			this->mHaveStreamBufferBeenCreated=true;
			if(alIsSource(this->mSource))
			{
				while((this->mAvailableBuffers.size()+this->GetQueuedBufferCount())<mReceiveBufferSize)
				{
					ALuint buffer=0;

					alGenBuffers(1,&buffer);
					this->CheckForOpenALError(true,"alGenBuffers");
					if(alIsBuffer(buffer))
					{
						this->mAvailableBuffers.push(buffer);
					}
				}
			}
		}
	}

	void SoundSource::DestroyBuffers()
	{
		this->ReclaimProcessedBuffers();
		while(!this->mAvailableBuffers.empty())
		{
			alDeleteBuffers(1,&this->mAvailableBuffers.front());
			this->mAvailableBuffers.pop();
		}

		//alSourcei(this->mSource, AL_BUFFER,NULL);
		//this->CheckForOpenALError(true,"alSourcei");
	}

	void SoundSource::Poll()
	{
		if(!this->mIsStreaming)
		{
			return; //these methods assume streaming so skip them otherwise.
		}

		//try to make buffers available.
		this->ReclaimProcessedBuffers();

		this->ReallyQueueBuffer();

		//try to make buffers available.
		this->ReclaimProcessedBuffers();


		//if queue is full start playing.
		this->PlayStreamIfReady();
	}

	IAudioDecoder* SoundSource::GetAudioDecoder() const
	{
		return this->mAudioDecoder;
	}

	void SoundSource::SetAudioDecoder(IAudioDecoder* audioDecoder)
	{
		this->mAudioDecoder=audioDecoder;
	}

	Enigma::s32 SoundSource::GetTimestamp() const
	{
		return this->mTimestamp;
	}

	void SoundSource::SetTimestamp(Enigma::s32 timestamp)
	{
		this->mTimestamp=timestamp;
	}

	void SoundSource::IncrementTimestamp()
	{
		this->mTimestamp+=VOICE_CHAT_BUFFER_SIZE;
	}

    ALuint SoundSource::GetSource() const
    {
        return this->mSource;
    }

    size_t SoundSource::GetPriority() const
    {
        return this->mPriority;
    }

    void SoundSource::SetPriority(size_t value)
    {
        this->mPriority=value;
    }

	ALint SoundSource::GetQueuedBufferCount()
	{
		ALint value=0;

		if(alIsSource(this->mSource))
		{
			alGetSourcei(this->mSource, AL_BUFFERS_QUEUED, &value);
			this->CheckForOpenALError(true,"alGetSourcei");
		}

		return value;
	}

	ALint SoundSource::GetProcessedBufferCount()
	{
		ALint value=0;

		if(alIsSource(this->mSource))
		{
			alGetSourcei(this->mSource, AL_BUFFERS_PROCESSED, &value);
			this->CheckForOpenALError(true,"alGetSourcei");
		}

		return value;
	}

	bool SoundSource::GetIsOutOfBuffers()
	{
		if(this->GetProcessedBufferCount() >= this->GetQueuedBufferCount())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void SoundSource::QueueBuffer(Enigma::u8* encodedBuffer,size_t encodedSize)
	{
		this->mIsStreaming=true;

		Enigma::u8* buffer=NULL;
		size_t decodedSize=0;

		//This means it is the end of the stream.
		if(encodedSize==sizeof(Enigma::s32))
		{
			this->mTimestamp=0;

			//if this is the end don't wait for max buffer size just play what we have.
			if(this->GetState()!=AL_PLAYING && this->GetQueuedBufferCount()>this->GetProcessedBufferCount())
			{
				alSourcei(this->mSource, AL_LOOPING, AL_FALSE);
				this->CheckForOpenALError(true,"alSourcei");

				alSourcePlay(this->mSource);
				this->CheckForOpenALError(true,"alSourcePlay");
			}

			//diagnostic
			std::cout << "-----------------------------------" << std::endl;
			std::cout << "AvailableBuffers: " << this->mAvailableBuffers.size() << std::endl;
			std::cout << "WaitingBuffers: " << this->mBuffers.size() << std::endl;
			std::cout << "ProcessedBufferCount: " << GetProcessedBufferCount() << std::endl;
			std::cout << "QueuedBufferCount: " << GetQueuedBufferCount() << std::endl;

			return;
		}

		//decode voice data.
		buffer = this->mAudioDecoder->Decode(encodedBuffer,encodedSize,decodedSize,this->GetTimestamp());

		if(buffer!=NULL)
		{
			//try to make buffers available.
			this->ReclaimProcessedBuffers();

			//if there are no buffers ready then this function returns false so the buffer will get pushed unto the queue.
			if(!this->ReallyQueueBuffer(buffer))
			{
				this->mBuffers.push(buffer);
			}
		}
		else
		{
			std::cout << "Decoding voice data resulted in no PCM data!" << std::endl;
		}
	}

	void SoundSource::ReallyQueueBuffer()
	{
		if(!this->mIsStreaming)
		{
			return;
		}

		if(this->mBuffers.empty())
		{
			return;
		}

		Enigma::u8* buffer=this->mBuffers.front();
		if(this->ReallyQueueBuffer(buffer))
		{
			this->mBuffers.pop();
		}
	}

    bool SoundSource::ReallyQueueBuffer(Enigma::u8* buffer)
	{
        ALuint streamBuffer=AL_NONE;
        ALenum format = AL_FORMAT_MONO16;
		//buffer
        ALuint length = VOICE_CHAT_BUFFER_SIZE;
		ALuint frequency = VOICE_CHAT_FREQUENCY;

		if(this->mLastBuffer==buffer)
		{
			std::cout << "Duplicate buffer!" << std::endl;
			return true; //buffer is duplicate go ahead and throw it away.
		}

		if(this->mAvailableBuffers.empty())
		{
			return false;
		}

		streamBuffer=this->mAvailableBuffers.front();
        if(alIsBuffer(streamBuffer))
        {	
			this->CheckForOpenALError(false,"alIsBuffer");
			if(buffer!=NULL)
			{	
				alBufferData(streamBuffer,format,buffer,length,frequency);

				//check to make sure alBufferData didn't cause an error before moving on.
				if(alGetError() == AL_NO_ERROR)
				{
					if(this->ReallyQueueOpenALBuffer(streamBuffer))
					{
						this->mAvailableBuffers.pop();
						this->mLastBuffer=buffer;
						return true;
					}
				}
				else
				{
					std::cout << "An error occured while populating the buffer with audio data!" << std::endl;
				}
			}
			else
			{
				std::cout << "NULL buffer removed from queue!" << std::endl;
			}		
        }
		else
		{
			std::cout << "Stream buffer is not valid!" << std::endl;
		}

		return false;
    }

	bool SoundSource::ReallyQueueOpenALBuffer(const ALuint buffer)
	{
		if(this->mLastOpenALBuffer==buffer)
		{
			std::cout << "Duplicate OpenAL buffer!" << std::endl;

			//something strange happened best to delete the buffer and let the correct number of buffers be recreated.
			if(alIsBuffer(buffer))
			{
				alDeleteBuffers(1,&buffer);
				this->mAvailableBuffers.pop(); //normally pop isn't called when false is returned but this buffer is bad.
			}

			return false;
		}

		if(alIsBuffer(buffer) && alIsSource(this->mSource))
        {
			//add buffer to OpenAL queue.
			alSourceQueueBuffers(this->mSource,1,&buffer);
			this->CheckForOpenALError(true,"alSourceQueueBuffers");

			this->mLastOpenALBuffer=buffer;

			return true;
		}

		return false;
	}

	void SoundSource::PlayStreamIfReady()
	{
		if(!this->mIsStreaming)
		{
			return;
		}

		if(!alIsSource(this->mSource))
		{
			return;
		}

/*
		if(this->GetState()!=AL_PLAYING && this->GetQueuedBufferCount()>this->GetProcessedBufferCount())
		{
			if(this->mAvailableBuffers.empty())
			{
				alSourcei(this->mSource, AL_LOOPING, AL_FALSE);
				this->CheckForOpenALError(true,"alSourcei");

				alSourcePlay(this->mSource);
				this->CheckForOpenALError(true,"alSourcePlay");
			}
		}
		else
		{	
			//If all the queued buffers have been played then it's time to stop.
			if(this->GetQueuedBufferCount()==this->GetProcessedBufferCount())
			{
				//stop playback because there is nothing to play.
				alSourceStop(this->mSource);
				this->CheckForOpenALError(true,"alSourceStop");
			}
		}
*/
	}

	void SoundSource::ReclaimProcessedBuffers()
	{
		if(!this->mIsStreaming)
		{
			return;
		}

		if(!alIsSource(this->mSource))
        {
			return;
		}

		ALint value;
		ALuint extraBuffer;
		ALuint otherBuffer;

		//loop through all the processed buffers and move them to queue.
		alGetSourcei(this->mSource, AL_BUFFERS_PROCESSED, &value);
		this->CheckForOpenALError(true,"alGetSourcei");
		while(value>0)
		{
			alSourceUnqueueBuffers(this->mSource, 1,&extraBuffer);
			this->CheckForOpenALError(true,"alSourceUnqueueBuffers");

			if(alGetError() != AL_NO_ERROR)
			{
				return; //failed to unqueue buffer continuing could cause corruption.
			}

			if(alIsBuffer(extraBuffer))
			{
				if(this->mAvailableBuffers.empty() || (this->mAvailableBuffers.back()!=extraBuffer && this->mAvailableBuffers.front()!=extraBuffer))
				{
					this->mAvailableBuffers.push(extraBuffer);
				}
				else
				{
					std::cout << "Buffer was queued multiple times!" << std::endl;
				}
			}
			else
			{
				std::cout << "Unqueued buffer is invalid and thus not reusable!" << std::endl;
			}

			alGetSourcei(this->mSource, AL_BUFFERS_PROCESSED, &value);
			this->CheckForOpenALError(true,"alGetSourcei");
		}

		//if all the buffers are out of source then unbind them from it.
		if(this->GetQueuedBufferCount()==0)
		{
			alSourcei(this->mSource, AL_BUFFER,NULL);
			this->CheckForOpenALError(true,"alSourcei");
		}

		if(this->mAvailableBuffers.empty() && this->GetQueuedBufferCount() < this->mReceiveBufferSize)
		{
			//recreate buffers because they have vanished.
			this->mHaveStreamBufferBeenCreated=false;
			this->CreateBuffers();
		}
	}

	void SoundSource::Play()
	{
		this->mIsStreaming=false;
		if(alIsBuffer(this->mBuffer))
		{
			//alSourcei(this->mSource, AL_BUFFER,this->mBuffer);
			//this->CheckForOpenALError(true,"alSourcei");

			alSourceQueueBuffers(this->mSource,1,&mBuffer);
			this->CheckForOpenALError(true,"alSourceQueueBuffers");

			if(alIsSource(this->mSource))
			{
				alSourcePlay(this->mSource);
				this->CheckForOpenALError(true,"alSourcePlay");
			}
		}
	}

    void SoundSource::Play(ALuint buffer)
    {
        this->Stop();
        this->mBuffer=buffer;
        this->Play();
    }

    void SoundSource::Play(const std::string& filename)
    {
        this->Stop();
        this->LoadAudio(filename);
        this->Play();
    }

    void SoundSource::Play(Enigma::c8* filename)
    {
        this->Stop();
        this->LoadAudio(filename);
        this->Play();
    }

    void SoundSource::Play(const Enigma::c8* filename)
    {
        this->Stop();
        this->LoadAudio(filename);
        this->Play();
    }

    void SoundSource::Stop()
    {
        if(alIsSource(this->mSource))
        {
            alSourceStop(this->mSource);
            this->CheckForOpenALError(true,"alSourceStop");
        }
        if(alIsBuffer(this->mBuffer))
        {
            alDeleteBuffers(1,&mBuffer);
            this->CheckForOpenALError(true,"alDeleteBuffers");
            this->mBuffer=AL_NONE;
        }
    }

    void SoundSource::LoadAudio(const std::string& filename)
    {
        this->LoadAudio(filename.c_str());
    }

    void SoundSource::LoadAudio(const Enigma::c8* filename)
    {
        SNDFILE* soundFile=NULL;
        SF_INFO soundFileInfo;
        ALuint channels = 1;
        ALuint bitsPerSample = 16;
        ALenum format = AL_FORMAT_MONO16;;
        ALuint frequency;
        ALuint blockAlign = channels * bitsPerSample / 8;
        ALuint frameSize;

        if(filename==NULL)
        {
            std::cout << "filename must not be NULL." << std::endl;
            return;
        }
        else
        {
            std::cout << "Loading " << filename << "." << std::endl;
        }

        soundFile = sf_open(filename,SFM_READ,&soundFileInfo);
        if(soundFile!=NULL)
        {
            format = AL_FORMAT_MONO16; //default
            switch(soundFileInfo.channels)
            {
                case 1:
                    format = AL_FORMAT_MONO16;
                break;
                case 2:
                    format = AL_FORMAT_STEREO16;
                break;
                case 3:
                    //unknown
                break;
                case 4:
                    //format = AL_FORMAT_QUAD16;
                break;
                case 5:
                    //unknown
                break;
                case 6:
                    //format = AL_FORMAT_51CHN16;
                break;
                case 7:
                    //format = AL_FORMAT_61CHN16;
                break;
                case 8:
                    //format = AL_FORMAT_71CHN16;
                break;
                default:

                break;
            }

            frequency = soundFileInfo.samplerate;
            blockAlign = soundFileInfo.channels*2;
            frameSize = 2*soundFileInfo.channels;

            alGenBuffers(1,&mBuffer);
            this->CheckForOpenALError(true,"alGenBuffers");
            if(!alIsBuffer(this->mBuffer))
            {
                mBuffer=AL_NONE;
                std::cout << "Failed to create buffer for " << filename << "." << std::endl;
                return;
            }
            //read data.
            //short buffer[soundFileInfo.frames];
            short* buffer = new short[soundFileInfo.frames];
            size_t count=0;
            count=sf_readf_short(soundFile,buffer,soundFileInfo.frames);
            if(count==0)
            {
                std::cout << "Zero bytes read from " << filename << "." << std::endl;
                return;
            }
            else
            {
                std::cout << count << " bytes read from " << filename << "." << std::endl;
            }

            alBufferData(mBuffer, format, buffer,frameSize*soundFileInfo.frames, frequency);
            this->CheckForOpenALError(true,"alBufferData");
            if(false) //thinking of good way to check.
            {
                mBuffer=AL_NONE;
                std::cout << "Failed to read audio in " << filename << "." << std::endl;
            }
            delete []buffer;
            sf_close(soundFile);
        }
        else
        {
            std::cout << "Failed to read " << filename << "." << std::endl;
        }
    }

    ALint SoundSource::GetState() const
    {
        ALint state;

        if(alIsSource(this->mSource))
        {
            this->CheckForOpenALError(true,"alIsSource");

            alGetSourcei(this->mSource,AL_SOURCE_STATE,&state);
            this->CheckForOpenALError(true,"alGetSourcei");
        }
        else
        {
            this->CheckForOpenALError(true,"alIsSource");

            state = AL_STOPPED;
        }

        return state;
    }

    bool SoundSource::GetIsBusy() const
    {
        ALint state;

        state = this->GetState();
        if(state==AL_PLAYING)return true;
        if(state==AL_STREAMING)return true;

        return false;
    }

    void SoundSource::UpdateSourcePosition(const Enigma::Entity& entity)
    {
        this->UpdateSourcePosition(entity.GetX(),entity.GetY(),entity.GetZ());
    }

    void SoundSource::UpdateSourcePosition(f32 x,f32 y,f32 z)
    {
        ALfloat sourcePosition[3];
        sourcePosition[0] = x;
        sourcePosition[1] = y;
        sourcePosition[2] = z==0?0:-z;

        ALfloat sourceVelocity[3];
        sourceVelocity[0] = 0;
        sourceVelocity[1] = 0;
        sourceVelocity[2] = 0;

        if(alIsSource(this->mSource))
        {
            this->CheckForOpenALError(true,"alIsSource");

            alSourcefv(this->mSource, AL_POSITION,sourcePosition);
            this->CheckForOpenALError(true,"alSourcefv");
        }
        else
        {
            this->CheckForOpenALError(true,"alIsSource");
        }

        if(alIsSource(this->mSource))
        {
            this->CheckForOpenALError(true,"alIsSource");

            alSourcefv(this->mSource, AL_VELOCITY,sourceVelocity);
            this->CheckForOpenALError(true,"alSourcefv");
        }
        else
        {
            this->CheckForOpenALError(true,"alIsSource");
        }
    }

    void SoundSource::SetBuffer(ALuint value)
    {
        this->mBuffer=value;
        if(alIsSource(this->mSource))
        {
            this->CheckForOpenALError(true,"alIsSource");

            alSourcei(this->mSource, AL_BUFFER,this->mBuffer);
            this->CheckForOpenALError(true,"alSourcei");
        }
        else
        {
            this->CheckForOpenALError(true,"alIsSource");
        }

    }

    void SoundSource::Dispose()
    {
        if(alIsSource(this->mSource))
        {
            this->CheckForOpenALError(true,"alIsSource");

            alDeleteSources(1,&mSource);
            this->CheckForOpenALError(true,"alDeleteSources");
        }
        else
        {
            this->CheckForOpenALError(true,"alIsSource");
        }

        if(alIsBuffer(this->mBuffer))
        {
            this->CheckForOpenALError(true,"alIsBuffer");

            alDeleteBuffers(1,&mBuffer);
            this->CheckForOpenALError(true,"alDeleteBuffers");
        }
        else
        {
            this->CheckForOpenALError(true,"alIsBuffer");
        }
        std::cout << "Sound source disposed." << std::endl;
    }

    bool SoundSource::LessThan(const SoundSource& y) const
    {
        if(this->GetState()==AL_STREAMING && y.GetState()!=AL_STREAMING)
        {
            return false;
        }
        else
        if(this->GetState()!=AL_STREAMING && y.GetState()==AL_STREAMING)
        {
            return true;
        }
        else
        if(this->GetState()==AL_PLAYING && y.GetState()!=AL_PLAYING)
        {
            return false;
        }
        else
        if(this->GetState()!=AL_PLAYING && y.GetState()==AL_PLAYING)
        {
            return true;
        }
        else
        if(this->GetPriority() < y.GetPriority())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool SoundSource::GreaterThan(const SoundSource& y) const
    {
        if(this->GetState()!=AL_STREAMING && y.GetState()==AL_STREAMING)
        {
            return false;
        }
        else
        if(this->GetState()==AL_STREAMING && y.GetState()!=AL_STREAMING)
        {
            return true;
        }
        else
        if(this->GetState()!=AL_PLAYING && y.GetState()==AL_PLAYING)
        {
            return false;
        }
        else
        if(this->GetState()==AL_PLAYING && y.GetState()!=AL_PLAYING)
        {
            return true;
        }
        else
        if(this->GetPriority() > y.GetPriority())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool SoundSource::operator<(const SoundSource& y) const
    {
        return LessThan(y);
    }

    bool SoundSource::operator>(const SoundSource& y) const
    {
        return GreaterThan(y);
    }

    bool SoundSource::operator<(SoundSource& y)
    {
        return LessThan(y);
    }

    bool SoundSource::operator>(SoundSource& y)
    {
        return GreaterThan(y);
    }

    void SoundSource::CheckForOpenALError(bool nothrow,const Enigma::c8* functionName) const
    {
        ALenum error=alGetError();
        switch(error)
        {
            case AL_INVALID_NAME:
                if(!nothrow)
                {
                    throw EnigmaException("AL_INVALID_NAME");
                }
                else
                {
                    std::cout << "SoundSource::" << functionName << " caused a " << "AL_INVALID_NAME" << " error." << std::endl;
                }
            break;
            case AL_INVALID_ENUM:
                if(!nothrow)
                {
                    throw EnigmaException("AL_INVALID_ENUM");
                }
                else
                {
                    std::cout << "SoundSource::" << functionName << " caused a " << "AL_INVALID_ENUM" << std::endl;
                }
            break;
            case AL_INVALID_VALUE:
                if(!nothrow)
                {
                    throw EnigmaException("AL_INVALID_VALUE");
                }
                else
                {
                    std::cout << "SoundSource::" << functionName << " caused a " << "AL_INVALID_VALUE" << std::endl;
                }
            break;
            case AL_INVALID_OPERATION:
                if(!nothrow)
                {
                    throw EnigmaException("AL_INVALID_OPERATION");
                }
                else
                {
                    std::cout << "SoundSource::" << functionName << " caused a " << "AL_INVALID_OPERATION" << std::endl;
                }
            break;
            case AL_OUT_OF_MEMORY:
                if(!nothrow)
                {
                    throw EnigmaException("AL_OUT_OF_MEMORY");
                }
                else
                {
                    std::cout << "AL_OUT_OF_MEMORY" << std::endl;
                }
            break;
            case AL_NO_ERROR:
                //do nothing its fine.
            break;
            default:
                //this means the api has changed.
                throw EnigmaException("unknown openal exception.");
            break;
        };
    }

};
