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

		ShaderParser() = delete;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool parse(String const & src, SStream & v, SStream & g, SStream & f);
		
		static String parse(String const & src);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SHADER_PARSER_HPP_