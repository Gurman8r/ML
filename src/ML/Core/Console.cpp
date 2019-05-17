#include <ML/Core/Console.hpp>
#include <ML/Core/Debug.hpp>

# ifdef ML_SYSTEM_WINDOWS
#	include <Windows.h>
#	include <conio.h>
# endif // ML_SYSTEM_WINDOWS

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Console::Console()
	{
	}

	Console::~Console()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Console::enableMenuItem(const uint32_t item, const uint32_t enable)
	{
#ifdef ML_SYSTEM_WINDOWS
		if (HWND window = GetConsoleWindow())
		{
			if (HMENU menu = GetSystemMenu(window, false))
			{
				return EnableMenuItem(menu, item, enable);
			}
		}
#endif
		return false;
	}

	bool Console::setTextAttribute(const uint16_t value)
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