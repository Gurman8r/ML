#ifndef _ML_SHADER_PARSER_HPP_
#define _ML_SHADER_PARSER_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Core/File.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API ShaderParser final
	{
	public:
		using IncludeClbk = typename String(*)(const String&);

	public:
		static bool		parseShader(const String & src, SStream & v, SStream & g, SStream & f);
		static String	parseShader(const String & src);
		static String	parseIncludes(const String & src);
		static String	parseIncludes(const String & src, char lhs, char rhs, IncludeClbk callback);
		static bool		parseWrapped(const String & src, const char lhs, const char rhs, String & out);
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SHADER_PARSER_HPP_