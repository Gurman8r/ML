#include <ML/Engine/Python.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/Input.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Engine/ContentLoader.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace py = pybind11;
	
	using Str = typename std::string;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	PYBIND11_EMBEDDED_MODULE(memelib_cplusplus, m)
	{
		m.def("pause",	[]() { Debug::pause(0); });
		m.def("print",	[](const Str & value) { cout << value; });
		m.def("printl", [](const Str & value) { cout << value << endl; });

		m.def("load", [](const Tree<Str, Str> & data)
		{
			Metadata md;
			for (const auto & pair : data)
			{
				md.setData(pair.first, pair.second);
			}
			return ContentLoader::parseMetadata(md);
		});
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}