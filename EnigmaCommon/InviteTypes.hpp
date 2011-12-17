#ifndef INVITETYPES_HPP_INCLUDED
#define INVITETYPES_HPP_INCLUDED

/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "TypeDefs.hpp"
#include "ChatType.hpp"

namespace Enigma
{
	static const size_t INVITE_TYPE_UNKNOWN=CHAT_TYPE_NONE;
	static const size_t INVITE_TYPE_NONE=CHAT_TYPE_NONE;
    static const size_t INVITE_TYPE_MAP=CHAT_TYPE_MAP;
    static const size_t INVITE_TYPE_SYSTEM=CHAT_TYPE_SYSTEM;
    static const size_t INVITE_TYPE_BROADCAST=CHAT_TYPE_BROADCAST;
    static const size_t INVITE_TYPE_WHISPER=CHAT_TYPE_WHISPER;
    static const size_t INVITE_TYPE_PARTY=CHAT_TYPE_PARTY;
    static const size_t INVITE_TYPE_GUILD=CHAT_TYPE_GUILD;
};

#endif // INVITETYPES_HPP_INCLUDED
