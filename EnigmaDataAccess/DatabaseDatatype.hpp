#ifndef DATABASEDATATYPE_HPP_INCLUDED
#define DATABASEDATATYPE_HPP_INCLUDED

/*
Copyright � 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "TypeDefs.hpp"

namespace Enigma
{
    const size_t DATABASE_DATATYPE_UNTYPED=0;
    const size_t DATABASE_DATATYPE_INTEGER=1;

    const size_t DATABASE_DATATYPE_FLOAT=2;
    const size_t DATABASE_DATATYPE_DOUBLE=2; //same as float at least for now.
	const size_t DATABASE_DATATYPE_REAL=2; //same as float at least for now.

    const size_t DATABASE_DATATYPE_TEXT=3;
    const size_t DATABASE_DATATYPE_BLOB=4;
    const size_t DATABASE_DATATYPE_NULL=5;

	const size_t DATABASE_DATATYPE_UUID=6;
	const size_t DATABASE_DATATYPE_GUID=6;
};

#endif // DATABASEDATATYPE_HPP_INCLUDED
