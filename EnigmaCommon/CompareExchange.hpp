#ifndef COMPAREEXCHANGE_HPP_INCLUDED
#define COMPAREEXCHANGE_HPP_INCLUDED

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
    #include <windows.h>
    #include <_intrin.h>
    #pragma warning( pop )
    #define COMPARE_EXCHANGE_LOADED 1
#endif

#ifdef ENIGMA_PLATFORM_LINUX
    #include <glib.h> //Linux has compare & exchange right in glib interesting.
    #define COMPARE_EXCHANGE_LOADED 2
#endif

#ifdef ENIGMA_PLATFORM_MAC
    #include <libkern/OSBase.h>
    #define COMPARE_EXCHANGE_LOADED 3
#endif

#ifdef ENIGMA_PLATFORM_BSD
    #include <glib.h>
    #define COMPARE_EXCHANGE_LOADED 4
#endif

#ifdef ENIGMA_PLATFORM_OPENSOLARIS
    #include <sys/atomic.h>
    #define COMPARE_EXCHANGE_LOADED 5
#endif

#ifdef COMPARE_EXCHANGE_LOADED //no support for unknown platform here to risky.
namespace Enigma
{
    inline bool CompareExchangePointer(void* destination, void* newValue, void* oldValue)
    {
    #ifdef ENIGMA_PLATFORM_WINDOWS
        //returns pointer
        InterlockedCompareExchangePointer(destination,newValue,oldValue);
        return true;
    #endif

    #ifdef ENIGMA_PLATFORM_LINUX
        //this actually returns bool.
        return g_atomic_pointer_compare_and_exchange(destination,oldValue,newValue);
    #endif

    #ifdef ENIGMA_PLATFORM_MAC
        //this actually returns bool.
        return OSCompareAndSwapPtr(oldValue,newValue,destination);
    #endif

    #ifdef ENIGMA_PLATFORM_BSD
        //Hmm, I think BSD uses glib. It's been a while.
        return g_atomic_pointer_compare_and_exchange(destination,oldValue,newValue);
    #endif

    #ifdef ENIGMA_PLATFORM_OPENSOLARIS
        //returns pointer
        atomic_cas_ptr(destination,oldValue,newValue);
        return true;
    #endif
    }
};
#endif

#endif // COMPAREEXCHANGE_HPP_INCLUDED
