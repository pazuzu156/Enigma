#ifndef OPENSTEER_HPP_INCLUDED
#define OPENSTEER_HPP_INCLUDED

/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#ifdef ENIGMA_PLATFORM_WINDOWS
    #pragma warning( push )
    #pragma warning( disable : 4061 )
    #pragma warning( disable : 4100 )
    #pragma warning( disable : 4127 )
    #pragma warning( disable : 4217 )
    #pragma warning( disable : 4242 )
    #pragma warning( disable : 4244 )
    #pragma warning( disable : 4280 )
    #pragma warning( disable : 4514 )
    #pragma warning( disable : 4619 )
    #pragma warning( disable : 4668 )
    #pragma warning( disable : 4710 )
    #pragma warning( disable : 4820 )
    #include <OpenSteer\SteerLibrary.h>
    #pragma warning( pop )
    #define OPENSTEER_LOADED 1
#endif

#ifdef ENIGMA_PLATFORM_LINUX
    #include <OpenSteer\SteerLibrary.h>
    #define OPENSTEER_LOADED 2
#endif

#ifdef ENIGMA_PLATFORM_MAC
    #include <OpenSteer\SteerLibrary.h>
    #define OPENSTEER_LOADED 3
#endif

#ifdef ENIGMA_PLATFORM_BSD
    #include <OpenSteer\SteerLibrary.h>
    #define OPENSTEER_LOADED 4
#endif

#ifdef ENIGMA_PLATFORM_OPENSOLARIS
    #include <OpenSteer\SteerLibrary.h>
    #define OPENSTEER_LOADED 5
#endif

#ifndef OPENSTEER_LOADED
    #include <OpenSteer\SteerLibrary.h>
    #define OPENSTEER_LOADED 6
#endif

#endif // OPENSTEER_HPP_INCLUDED
