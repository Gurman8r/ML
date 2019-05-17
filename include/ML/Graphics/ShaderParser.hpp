#ifndef _ML_SHADER_PARSER_HPP_
#define _ML_SHADER_PARSER_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Core/File.hpp>

#define ML_INCLUDE_SHADER_PATH "../../../assets/shaders/" // FIXME: shouldn't be hardcoded

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API ShaderParser final
	{
	public:
		static bool parseShader(const String & src, SStream & v, SStream & g, SStream & f);

		static String parseIncludes(const String & src);

		static bool parseWrapped(const String & src, const char lhs, const char rhs, String & out);
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SHADER_PARSER_HPP_