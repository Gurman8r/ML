/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/Config.hpp>

#if defined(ML_SYSTEM_WINDOWS)

#include <Windows.h>

extern INT main();

INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	return main();
}

#endif

/* * * * * * * * * * * * * * * * * * * * */