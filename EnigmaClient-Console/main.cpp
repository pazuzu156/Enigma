/*
Copyright © 2011  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "Application.hpp"

Enigma::Application application;
#ifdef ENIGMA_PLATFORM_WINDOWS
void __cdecl CleanUp();
#else
void CleanUp();
#endif

#ifdef ENIGMA_PLATFORM_WINDOWS
void TranslateException( unsigned int, EXCEPTION_POINTERS* );
#endif

#undef main

#ifdef ENIGMA_PLATFORM_WINDOWS
int __cdecl main(int argc, Enigma::c8** argv)
#else
int main(int argc, Enigma::c8** argv)
#endif
{
	bool success=false;

    try
    {
        #ifdef ENIGMA_PLATFORM_WINDOWS
        _set_se_translator( TranslateException );
        #endif
        atexit(CleanUp);
        application.Start(argc,argv);
		success=true;
    }
    catch(Enigma::EnigmaException& e)
    {
        #ifdef ENIGMA_PLATFORM_WINDOWS
		MessageBoxA(0,e.what(),"Enigma Exception",MB_OK | MB_ICONERROR);
        #else
        std::cout << "Enigma Exception: " << e.what() << std::endl;
        #endif
    }
    catch (std::overflow_error&)
    {
        throw; //may need to pop some calls off the stack.
    }
	catch (Enigma::HardwareException& e)
	{
		#ifdef ENIGMA_PLATFORM_WINDOWS
		MessageBoxA(0,e.what(),"Hardware Exception",MB_OK | MB_ICONERROR);
        #else
        std::cout << "Hardware Exception: " << e.what() << std::endl;
        #endif
		throw;
	}
    catch (std::exception& e)
    {
        #ifdef ENIGMA_PLATFORM_WINDOWS
		MessageBoxA(0,e.what(),"Exception",MB_OK | MB_ICONERROR);
        #else
        std::cout << "Exception: " << e.what() << std::endl;
        #endif
    }

    if(success)
    {
        return EXIT_SUCCESS;
    }
    else
    {
        return EXIT_FAILURE;
    }
}

#ifdef ENIGMA_PLATFORM_WINDOWS
void __cdecl CleanUp()
#else
void CleanUp()
#endif
{
	//not needed at this point.
}

#ifdef ENIGMA_PLATFORM_WINDOWS
void TranslateException( unsigned int u, EXCEPTION_POINTERS* pExp )
{
	throw Enigma::HardwareException(u,pExp);
}
#endif
