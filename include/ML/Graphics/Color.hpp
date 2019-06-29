#ifndef _ML_COLOR_HPP_
#define _ML_COLOR_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Core/Matrix.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */
	
	struct Color final
	{
		static constexpr vec4 clear			{ 0.0f, 0.0f, 0.0f, 0.0f };
		static constexpr vec4 white			{ 1.0f, 1.0f, 1.0f, 1.0f };
		static constexpr vec4 black			{ 0.0f, 0.0f, 0.0f, 1.0f };
		static constexpr vec4 gray			{ 0.2f, 0.2f, 0.2f, 1.0f };

		static constexpr vec4 red			{ 1.0f, 0.0f, 0.0f, 1.0f };
		static constexpr vec4 green			{ 0.0f, 1.0f, 0.0f, 1.0f };
		static constexpr vec4 blue			{ 0.0f, 0.0f, 1.0f, 1.0f };

		static constexpr vec4 cyan			{ 0.0f, 1.0f, 1.0f, 1.0f };
		static constexpr vec4 yellow		{ 1.0f, 1.0f, 0.0f, 1.0f };
		static constexpr vec4 magenta		{ 1.0f, 0.0f, 1.0f, 1.0f };

		static constexpr vec4 violet		{ 0.5f, 0.0f, 1.0f, 1.0f };
		static constexpr vec4 lime			{ 0.5f, 1.0f, 0.0f, 1.0f };
		static constexpr vec4 orange		{ 1.0f, 0.5f, 0.0f, 1.0f };

		static constexpr vec4 fuchsia		{ 1.0f, 0.0f, 0.5f, 1.0f };
		static constexpr vec4 aqua			{ 0.0f, 1.0f, 0.5f, 1.0f };
		static constexpr vec4 azure			{ 0.0f, 0.5f, 1.0f, 1.0f };
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_COLOR_HPP_