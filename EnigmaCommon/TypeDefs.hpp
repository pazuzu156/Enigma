#ifndef TYPEDEFS_HPP_INCLUDED
#define TYPEDEFS_HPP_INCLUDED

/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include <stddef.h>

#ifdef _MSC_VER
	#pragma warning(disable: 4251)
#endif

namespace Enigma
{
	#ifndef BOOST_LIB_DIAGNOSTIC
		#define BOOST_LIB_DIAGNOSTIC
	#endif

	#ifndef VOICE_CHAT_BUFFER_SIZE
		//#define VOICE_CHAT_BUFFER_SIZE (4096 * 8)
		//#define VOICE_CHAT_BUFFER_SIZE (6400 * 4)
		#define VOICE_CHAT_BUFFER_SIZE 16384 //forum recommendation
	#endif

	#ifndef VOICE_CHAT_FREQUENCY
		#define VOICE_CHAT_FREQUENCY 16000
		//#define VOICE_CHAT_FREQUENCY 32000
	#endif

	#ifndef VOICE_CHAT_RESOLUTION
		#define VOICE_CHAT_RESOLUTION 16
	#endif

	#ifndef NUMBER_OF_AUDIO_BUFFERS
		//#define NUMBER_OF_AUDIO_BUFFERS 3
		#define NUMBER_OF_AUDIO_BUFFERS 4 //forum recommendation
	#endif

	#ifndef NUMBER_OF_AUDIO_SOURCES
		#define NUMBER_OF_AUDIO_SOURCES 100
	#endif

	#ifdef _MSC_VER
		#define DllExport   __declspec( dllexport )

		typedef unsigned __int8 u8;
		typedef __int8 s8;
		typedef unsigned __int16 u16;
		typedef __int16 s16;
		typedef unsigned __int32	u32;
		typedef __int32			s32;
		typedef __int64 s64; //fix undefine issue.
		typedef unsigned __int64 u64; //fix undefine issue.

		typedef char c8;
		typedef float f32;
		typedef double f64;
	#else
		#define DllExport   /**/

		typedef unsigned char u8;
		typedef signed char s8;
		typedef unsigned short u16;
		typedef signed short s16;
		typedef unsigned int		u32;
		typedef signed int		s32;
		typedef int64_t s64; //fix undefine issue.
		typedef uint64_t u64; //fix undefine issue.

		typedef char c8;
		typedef float f32;
		typedef double f64;
	#endif

	template <class T>
	struct vector3d
	{
		vector3d()
			:X(0),Y(0),Z(0){}
		T X;
		T Y;
		T Z;
	};
};

#endif // TYPEDEFS_HPP_INCLUDED
