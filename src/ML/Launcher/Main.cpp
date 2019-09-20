/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <ML/Launcher/Launcher.hpp>

ml::int32_t main()
{
	using namespace ml;
	constexpr auto t { typeof<Debug>().m_name };
	return ML_Launcher(__argc, __argv);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */