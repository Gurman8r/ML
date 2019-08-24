/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <ML/Launcher/Launcher.hpp>

ml::int32_t main()
{
	constexpr ml::C_String test { ml::typeof<ml::C_String>().name() };

	return ML_Launcher(__argc, __argv);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */