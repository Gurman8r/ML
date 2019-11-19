#ifndef _ML_SHADER_PARSER_HPP_
#define _ML_SHADER_PARSER_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Core/File.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API ShaderParser final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using IncludeCallback = typename String(*)(const String&);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool		parseShader(String const & src, SStream & v, SStream & g, SStream & f);
		static String	parseShader(String const & src);
		static String	parseIncludes(String const & src);
		static String	parseIncludes(String line, char lhs, char rhs, IncludeCallback callback);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SHADER_PARSER_HPP_