#ifndef IAUDIOEVENTLISTENER_HPP_INCLUDED
#define IAUDIOEVENTLISTENER_HPP_INCLUDED

/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "TypeDefs.hpp"
#include "Entities\Character.hpp"

namespace Enigma
{
    class DllExport IAudioEventListener
    {
        private:

        protected:

        public:

        virtual ~IAudioEventListener(){};

		/*
		 * This method is called when the voice received event occurs.
		 * When this event is received one of the listeners should be responsible for adding the included buffer to an audio stream buffer.
		 * The buffer is bits of audio data from other players.
		 */
		virtual void onVoiceReceived(Enigma::s32 characterId, const Enigma::Character& character, Enigma::u8* buffer,size_t length)=0;

		/*
		 * This method is called when the state of any audio recording should be changed.
		 * When this event is received one of hte listeneners should be responsibel for stopping or starting the current recording device.
		 * Generally this event will not cause exceptions.
		 */
		virtual void onRecorderStateChanged(bool isRecording)=0;

		/*
		 * This method is called when some part of the system requests that a sound be played.
		 * When this event is received one of the listeners should be responsible for loading the file and passing it to an Enigma audio driver.
		 */
		virtual void onPlayFile(const Enigma::Entity& entity, size_t audioId, const std::string& filename)=0;
	};
};

#endif // IAUDIOEVENTLISTENER_HPP_INCLUDED