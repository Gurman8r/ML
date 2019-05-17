/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/StandardLib.hpp>

/* * * * * * * * * * * * * * * * * * * * */

extern int32_t main(int32_t, char **);

/* * * * * * * * * * * * * * * * * * * * */

#ifdef ML_SYSTEM_WINDOWS
#include <Windows.h>
INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	return main(__argc, __argv);
}
#endif

/* * * * * * * * * * * * * * * * * * * * */