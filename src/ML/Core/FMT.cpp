#include <ML/Core/FMT.hpp>

#ifdef ML_SYSTEM_WINDOWS
#include <Windows.h>
#endif

namespace ml
{
	bool FMT::setTextAttrib(const uint6_t value)
	{
#ifdef ML_SYSTEM_WINDOWS
		return SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), value);
#else 
		return false;
#endif
	}
}