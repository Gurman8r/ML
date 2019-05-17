/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/StandardLib.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#if defined(ML_SYSTEM_WINDOWS)

#include <Windows.h>

extern int32_t main(int32_t, char **);

INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	return main(__argc, __argv);
}

#endif

/* * * * * * * * * * * * * * * * * * * * */