#ifndef HARDWAREEXCEPTION_HPP_INCLUDED
#define HARDWAREEXCEPTION_HPP_INCLUDED

/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "string.hpp"
#include "iostream.hpp"

#ifdef ENIGMA_PLATFORM_WINDOWS
    #pragma warning( push )
    #pragma warning( disable : 4061 )
    #pragma warning( disable : 4100 )
    #pragma warning( disable : 4127 )
    #pragma warning( disable : 4217 )
    #pragma warning( disable : 4242 )
    #pragma warning( disable : 4244 )
    #pragma warning( disable : 4280 )
    #pragma warning( disable : 4513 )
    #pragma warning( disable : 4514 )
    #pragma warning( disable : 4619 )
    #pragma warning( disable : 4668 )
    #pragma warning( disable : 4710 )
    #pragma warning( disable : 4820 )
    #include <ostream>
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdarg.h>
    #include <stdexcept>
    #include <exception>
    #pragma warning( pop )
#else
    #include <ostream>
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdarg.h>
    #include <stdexcept>
    #include <exception>
#endif

#include "boost_lexical_cast.hpp"

#include "TypeDefs.hpp"

#pragma warning( push )
#pragma warning( disable : 4275 )

namespace Enigma
{
    class DllExport HardwareException : public std::runtime_error
    {
		private:

		protected:

        public:

        HardwareException(std::string const s) 
			: runtime_error(s)
		{
		}
		
		HardwareException(Enigma::c8 const s[]) 
			: runtime_error(std::string(s))
		{

		}

		#ifdef ENIGMA_PLATFORM_WINDOWS

		std::string GenerateMessage(unsigned int u, void* pExp);

		HardwareException(unsigned int u, void* pExp) 
			: runtime_error(GenerateMessage(u,pExp))
		{

		}
		#endif

		virtual ~HardwareException() throw(){};
    };
};

#pragma warning( pop )

#endif // HARDWAREEXCEPTION_HPP_INCLUDED