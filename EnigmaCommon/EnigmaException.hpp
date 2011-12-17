#ifndef ENIGMAEXCEPTION_HPP_INCLUDED
#define ENIGMAEXCEPTION_HPP_INCLUDED

/*
Copyright � 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "HardwareException.hpp"

#pragma warning( push )
#pragma warning( disable : 4275 )

namespace Enigma
{
    class DllExport EnigmaException : public std::runtime_error
    {
		private:

		protected:

        public:

        EnigmaException(std::string const s) 
			: runtime_error(s) 
		{

		}
		
		EnigmaException(Enigma::c8 const s[]) 
			: runtime_error(std::string(s)) 
		{

		}

        virtual ~EnigmaException() throw() 
		{

		}
    };
};

#pragma warning( pop )

#endif // ENIGMAEXCEPTION_HPP_INCLUDED
