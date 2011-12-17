#ifndef MESSAGE_HPP_INCLUDED
#define MESSAGE_HPP_INCLUDED

/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "TypeDefs.hpp"

namespace Enigma
{
    const size_t MESSAGE_TYPE=0;
    const size_t MESSAGE_LENGTH=1;
    const size_t MESSAGE_STATUS=2;

    union DllExport Message
    {
        Enigma::s8* signed8BitIntegerPointer;
        Enigma::u8* unsigned8BitIntegerPointer;

        Enigma::s16* signed16BitIntegerPointer;
        Enigma::u16* unsigned16BitIntegerPointer;

        Enigma::s32* signed32BitIntegerPointer;
        Enigma::u32* unsigned32BitIntegerPointer;

        Enigma::s64* signed64BitIntegerPointer;
        Enigma::u64* unsigned64BitIntegerPointer;


        Enigma::f32* signed32BitFloatPointer;

        Enigma::f64* signed64BitFloatPointer;
    };
};

#endif // MESSAGE_HPP_INCLUDED
