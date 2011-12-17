#ifndef GAMEMODE_HPP_INCLUDED
#define GAMEMODE_HPP_INCLUDED

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
	static const size_t GAME_MODE_NONE=0;
	static const size_t GAME_MODE_LOGIN=1;
	static const size_t GAME_MODE_SERVER_SELECT=2;
	static const size_t GAME_MODE_CHARACTER_SELECT=3;
	static const size_t GAME_MODE_CHARACTER_CREATE=4;
	static const size_t GAME_MODE_IN_GAME=5;
	static const size_t GAME_MODE_MINI_GAME=6;
};

#endif // GAMEMODE_HPP_INCLUDED
