#include <ML/Core/Console.hpp>
#include <ML/Core/Debug.hpp>

# ifdef ML_SYSTEM_WINDOWS
#	include <Windows.h>
#	include <conio.h>
# endif // ML_SYSTEM_WINDOWS

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	bool FMT::setTextAttribute(const uint16_t value)
	{
#ifdef ML_SYSTEM_WINDOWS
		if (HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE))
		{
			return SetConsoleTextAttribute(handle, value);
		}
#endif
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}