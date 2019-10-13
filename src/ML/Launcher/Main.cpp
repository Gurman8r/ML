/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <ML/Launcher/Launcher.hpp>

namespace ml
{
	template <class T> class I_Managed
	{
		using type = typename detail::decay_t<T>;
	};

	struct Test : public I_Managed<Test>
	{
		friend struct I_Managed<Test>;
	};
}


ml::int32_t main()
{
	using namespace ml;
	return ML_Launcher(ML_ARGC, ML_ARGV);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */