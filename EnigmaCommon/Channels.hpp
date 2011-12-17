#ifndef CHANNELS_HPP_INCLUDED
#define CHANNELS_HPP_INCLUDED

/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

namespace Enigma
{
	const static size_t CHANNEL_NONE=5; //this should never be used.
    const static size_t CHANNEL_AUTHENTICATION=0;
    const static size_t CHANNEL_MOVEMENT=1;
    const static size_t CHANNEL_COMBAT=2;
    const static size_t CHANNEL_CHAT=3;
    const static size_t CHANNEL_VOICE_CHAT=4;
};

#endif // CHANNELS_HPP_INCLUDED
