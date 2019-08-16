#include <ML/Engine/Python.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/Input.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Engine/ContentLoader.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace py = pybind11;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	PYBIND11_EMBEDDED_MODULE(memelib_cplusplus, m)
	{
		// Config
		/* * * * * * * * * * * * * * * * * * * * */
		m.attr("architecture")		= ML_ARCHITECTURE;
		m.attr("compiler_name")		= ML_CC_NAME;
		m.attr("configuration")		= ML_CONFIGURATION;
		m.attr("platform_target")	= ML_PLATFORM_TARGET;


		// IO
		/* * * * * * * * * * * * * * * * * * * * */
		m.def("pause",	[]() { Debug::pause(0); });
		m.def("print",	[](const std::string & value) { cout << value; });
		m.def("printl", [](const std::string & value) { cout << value << endl; });


		// Content
		/* * * * * * * * * * * * * * * * * * * * */
		m.def("load", [](const Tree<std::string, std::string> & data)
		{
			Metadata md;
			for (const auto & pair : data) md.setData(
				pair.first, pair.second
			);
			return (bool)ContentLoader::parseMetadata(md);
		});
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}