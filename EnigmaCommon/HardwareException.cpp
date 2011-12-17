#include "HardwareException.hpp"

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

    #include <windows.h>
    #include <eh.h>
    #pragma warning( pop )
#endif

namespace Enigma
{
	#ifdef ENIGMA_PLATFORM_WINDOWS

	std::string HardwareException::GenerateMessage(unsigned int u, void* exception)
	{
		std::string message;

		EXCEPTION_POINTERS* pExp = (EXCEPTION_POINTERS*)exception;

		switch(u)
		{
			case EXCEPTION_ACCESS_VIOLATION:
				message.append("Access Violation at ");
				if(pExp!=NULL && pExp->ExceptionRecord!=NULL)
				{
					message.append(boost::lexical_cast<std::string>(pExp->ExceptionRecord->ExceptionAddress));
				}
				else
				{
					message.append("an unknown address");
				}
				message.append(".\r\n");

				switch(pExp->ExceptionRecord->ExceptionInformation[0]) //for access violations this is a readwrite flag.
				{
					case 0:
					message.append("The thread attempted to read inaccessible data at ");
					break;
					case 1:
					message.append("The thread attempted to write to an inaccessible address at ");
					break;
					case 8:
					message.append("The thread causes a user-mode data execution prevention (DEP) violation at ");
					break;
				}

				if(pExp!=NULL && pExp->ExceptionRecord!=NULL && pExp->ExceptionRecord->ExceptionInformation!=NULL)
				{
					message.append(boost::lexical_cast<std::string>(pExp->ExceptionRecord->ExceptionInformation[1])); //second element is the bad address.
				}
				else
				{
					message.append("an unknown address");
				}
				message.append(".\r\n");
			break;
			case EXCEPTION_DATATYPE_MISALIGNMENT:
				message.append("DATATYPE_MISALIGNMENT at ");

				if(pExp!=NULL && pExp->ExceptionRecord!=NULL)
				{
					message.append(boost::lexical_cast<std::string>(pExp->ExceptionRecord->ExceptionAddress));
				}
				else
				{
					message.append("an unknown address");
				}
				message.append(".\r\n");
			break;
			case EXCEPTION_BREAKPOINT:
				message.append("BREAKPOINT at ");

				if(pExp!=NULL && pExp->ExceptionRecord!=NULL)
				{
					message.append(boost::lexical_cast<std::string>(pExp->ExceptionRecord->ExceptionAddress)); //second element is the bad address.
				}
				else
				{
					message.append("an unknown address");
				}
				message.append(".\r\n");
			break;
			case EXCEPTION_SINGLE_STEP:
				message.append("SINGLE_STEP at ");

				if(pExp!=NULL && pExp->ExceptionRecord!=NULL)
				{
					message.append(boost::lexical_cast<std::string>(pExp->ExceptionRecord->ExceptionAddress)); //second element is the bad address.
				}
				else
				{
					message.append("an unknown address");
				}
				message.append(".\r\n");
			break;
			case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
				message.append("ARRAY_BOUNDS_EXCEEDED at ");

				if(pExp!=NULL && pExp->ExceptionRecord!=NULL)
				{
					message.append(boost::lexical_cast<std::string>(pExp->ExceptionRecord->ExceptionAddress)); //second element is the bad address.
				}
				else
				{
					message.append("an unknown address");
				}
				message.append(".\r\n");
			break;
			case EXCEPTION_FLT_DENORMAL_OPERAND:
				message.append("FLT_DENORMAL_OPERAND at ");

				if(pExp!=NULL && pExp->ExceptionRecord!=NULL)
				{
					message.append(boost::lexical_cast<std::string>(pExp->ExceptionRecord->ExceptionAddress)); //second element is the bad address.
				}
				else
				{
					message.append("an unknown address");
				}
				message.append(".\r\n");
			break;
			case EXCEPTION_FLT_DIVIDE_BY_ZERO:
				message.append("FLT_DIVIDE_BY_ZERO at ");

				if(pExp!=NULL && pExp->ExceptionRecord!=NULL)
				{
					message.append(boost::lexical_cast<std::string>(pExp->ExceptionRecord->ExceptionAddress)); //second element is the bad address.
				}
				else
				{
					message.append("an unknown address");
				}
				message.append(".\r\n");
			break;
			case EXCEPTION_FLT_INEXACT_RESULT:
				message.append("FLT_INEXACT_RESULT at ");

				if(pExp!=NULL && pExp->ExceptionRecord!=NULL)
				{
					message.append(boost::lexical_cast<std::string>(pExp->ExceptionRecord->ExceptionAddress)); //second element is the bad address.
				}
				else
				{
					message.append("an unknown address");
				}
				message.append(".\r\n");
			break;
			case EXCEPTION_FLT_INVALID_OPERATION:
				message.append("FLT_INVALID_OPERATION at ");

				if(pExp!=NULL && pExp->ExceptionRecord!=NULL)
				{
					message.append(boost::lexical_cast<std::string>(pExp->ExceptionRecord->ExceptionAddress)); //second element is the bad address.
				}
				else
				{
					message.append("an unknown address");
				}
				message.append(".\r\n");
			break;
			case EXCEPTION_FLT_OVERFLOW:
				message.append("FLT_OVERFLOW at ");

				if(pExp!=NULL && pExp->ExceptionRecord!=NULL)
				{
					message.append(boost::lexical_cast<std::string>(pExp->ExceptionRecord->ExceptionAddress)); //second element is the bad address.
				}
				else
				{
					message.append("an unknown address");
				}
				message.append(".\r\n");
			break;
			case EXCEPTION_FLT_STACK_CHECK:
				message.append("FLT_STACK_CHECK at ");

				if(pExp!=NULL && pExp->ExceptionRecord!=NULL)
				{
					message.append(boost::lexical_cast<std::string>(pExp->ExceptionRecord->ExceptionAddress)); //second element is the bad address.
				}
				else
				{
					message.append("an unknown address");
				}
				message.append(".\r\n");
			break;
			case EXCEPTION_FLT_UNDERFLOW:
				message.append("FLT_UNDERFLOW at ");

				if(pExp!=NULL && pExp->ExceptionRecord!=NULL)
				{
					message.append(boost::lexical_cast<std::string>(pExp->ExceptionRecord->ExceptionAddress)); //second element is the bad address.
				}
				else
				{
					message.append("an unknown address");
				}
				message.append(".\r\n");
			break;
			case EXCEPTION_INT_DIVIDE_BY_ZERO:
				message.append("INT_DIVIDE_BY_ZERO at ");

				if(pExp!=NULL && pExp->ExceptionRecord!=NULL)
				{
					message.append(boost::lexical_cast<std::string>(pExp->ExceptionRecord->ExceptionAddress)); //second element is the bad address.
				}
				else
				{
					message.append("an unknown address");
				}
				message.append(".\r\n");
			break;
			case EXCEPTION_INT_OVERFLOW:
				message.append("INT_OVERFLOW at ");

				if(pExp!=NULL && pExp->ExceptionRecord!=NULL)
				{
					message.append(boost::lexical_cast<std::string>(pExp->ExceptionRecord->ExceptionAddress)); //second element is the bad address.
				}
				else
				{
					message.append("an unknown address");
				}
				message.append(".\r\n");
			break;
			case EXCEPTION_PRIV_INSTRUCTION:
				message.append("PRIV_INSTRUCTION at ");

				if(pExp!=NULL && pExp->ExceptionRecord!=NULL)
				{
					message.append(boost::lexical_cast<std::string>(pExp->ExceptionRecord->ExceptionAddress)); //second element is the bad address.
				}
				else
				{
					message.append("an unknown address");
				}
				message.append(".\r\n");
			break;
			case EXCEPTION_IN_PAGE_ERROR:
				message.append("IN_PAGE_ERROR at ");

				if(pExp!=NULL && pExp->ExceptionRecord!=NULL)
				{
					message.append(boost::lexical_cast<std::string>(pExp->ExceptionRecord->ExceptionAddress)); //second element is the bad address.
				}
				else
				{
					message.append("an unknown address");
				}
				message.append(".\r\n");
			break;
			case EXCEPTION_ILLEGAL_INSTRUCTION:
				message.append("ILLEGAL_INSTRUCTION at ");

				if(pExp!=NULL && pExp->ExceptionRecord!=NULL)
				{
					message.append(boost::lexical_cast<std::string>(pExp->ExceptionRecord->ExceptionAddress)); //second element is the bad address.
				}
				else
				{
					message.append("an unknown address");
				}
				message.append(".\r\n");
			break;
			case EXCEPTION_NONCONTINUABLE_EXCEPTION:
				message.append("NONCONTINUABLE_EXCEPTION at ");

				if(pExp!=NULL && pExp->ExceptionRecord!=NULL)
				{
					message.append(boost::lexical_cast<std::string>(pExp->ExceptionRecord->ExceptionAddress)); //second element is the bad address.
				}
				else
				{
					message.append("an unknown address");
				}
				message.append(".\r\n");
			break;
			case EXCEPTION_STACK_OVERFLOW:
				message.append("STACK_OVERFLOW at ");

				if(pExp!=NULL && pExp->ExceptionRecord!=NULL)
				{
					message.append(boost::lexical_cast<std::string>(pExp->ExceptionRecord->ExceptionAddress)); //second element is the bad address.
				}
				else
				{
					message.append("an unknown address");
				}
				message.append(".\r\n");
			break;
			case EXCEPTION_INVALID_DISPOSITION:
				message.append("INVALID_DISPOSITION at ");

				if(pExp!=NULL && pExp->ExceptionRecord!=NULL)
				{
					message.append(boost::lexical_cast<std::string>(pExp->ExceptionRecord->ExceptionAddress)); //second element is the bad address.
				}
				else
				{
					message.append("an unknown address");
				}
				message.append(".\r\n");
			break;
			case EXCEPTION_GUARD_PAGE:
				message.append("GUARD_PAGE at ");

				if(pExp!=NULL && pExp->ExceptionRecord!=NULL)
				{
					message.append(boost::lexical_cast<std::string>(pExp->ExceptionRecord->ExceptionAddress)); //second element is the bad address.
				}
				else
				{
					message.append("an unknown address");
				}
				message.append(".\r\n");
			break;
			case EXCEPTION_INVALID_HANDLE:
				message.append("INVALID_HANDLE at ");

				if(pExp!=NULL && pExp->ExceptionRecord!=NULL)
				{
					message.append(boost::lexical_cast<std::string>(pExp->ExceptionRecord->ExceptionAddress)); //second element is the bad address.
				}
				else
				{
					message.append("an unknown address");
				}
				message.append(".\r\n");
			break;
			default:
				message.append("Unknown structured exception at ");

				if(pExp!=NULL && pExp->ExceptionRecord!=NULL)
				{
					message.append(boost::lexical_cast<std::string>(pExp->ExceptionRecord->ExceptionAddress)); //second element is the bad address.
				}
				else
				{
					message.append("an unknown address");
				}
				message.append(".\r\n");
			break;
		}

		return message;
	}
		
	#endif
};