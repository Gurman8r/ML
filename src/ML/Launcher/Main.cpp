/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <ML/Launcher/Launcher.hpp>

ml::int32_t main()
{
	using namespace ml;
	static_assert(typeof<int32_t>::name		== "int");
	static_assert(typeof<uint32_t>::name	== "unsigned int");
	static_assert(typeof<float_t>::name		== "float");
	static_assert(typeof<C_String>::name	== "const char*");
	static_assert(typeof<Debug>::name		== "struct ml::Debug");
	static_assert(typeof<std::string>::name == "class std::string");
	static_assert(typeof<String>::name		== "struct ml::BasicString<char>");
	static_assert(typeof<List<String>>::name== "struct ml::List<struct ml::BasicString<char> >");
	return ML_Launcher(ML_ARGC, ML_ARGV);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */