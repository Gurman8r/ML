#ifndef _ML_COLOR_HPP_
#define _ML_COLOR_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Core/Matrix.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */
	
	struct Color final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr Color() = delete;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr vec4 clear		{ 0.0f, 0.0f, 0.0f, 0.0f };
		static constexpr vec4 white		{ 1.0f, 1.0f, 1.0f, 1.0f };
		static constexpr vec4 black		{ 0.0f, 0.0f, 0.0f, 1.0f };
		static constexpr vec4 gray		{ 0.2f, 0.2f, 0.2f, 1.0f };

		static constexpr vec4 red		{ 1.0f, 0.0f, 0.0f, 1.0f };
		static constexpr vec4 green		{ 0.0f, 1.0f, 0.0f, 1.0f };
		static constexpr vec4 blue		{ 0.0f, 0.0f, 1.0f, 1.0f };

		static constexpr vec4 cyan		{ 0.0f, 1.0f, 1.0f, 1.0f };
		static constexpr vec4 yellow	{ 1.0f, 1.0f, 0.0f, 1.0f };
		static constexpr vec4 magenta	{ 1.0f, 0.0f, 1.0f, 1.0f };

		static constexpr vec4 violet	{ 0.5f, 0.0f, 1.0f, 1.0f };
		static constexpr vec4 lime		{ 0.5f, 1.0f, 0.0f, 1.0f };
		static constexpr vec4 orange	{ 1.0f, 0.5f, 0.0f, 1.0f };

		static constexpr vec4 fuchsia	{ 1.0f, 0.0f, 0.5f, 1.0f };
		static constexpr vec4 aqua		{ 0.0f, 1.0f, 0.5f, 1.0f };
		static constexpr vec4 azure		{ 0.0f, 0.5f, 1.0f, 1.0f };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct Color32 final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr Color32() = delete;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr vec4b clear	{ 0x00, 0x00, 0x00, 0x00 };
		static constexpr vec4b white	{ 0xFF, 0xFF, 0xFF, 0xFF };
		static constexpr vec4b black	{ 0x00, 0x00, 0x00, 0xFF };
		static constexpr vec4b gray		{ 0x40, 0x40, 0x40, 0xFF };

		static constexpr vec4b red		{ 0xFF, 0x00, 0x00, 0xFF };
		static constexpr vec4b green	{ 0x00, 0xFF, 0x00, 0xFF };
		static constexpr vec4b blue		{ 0x00, 0x00, 0xFF, 0xFF };

		static constexpr vec4b cyan		{ 0x00, 0xFF, 0xFF, 0xFF };
		static constexpr vec4b yellow	{ 0xFF, 0xFF, 0x00, 0xFF };
		static constexpr vec4b magenta	{ 0xFF, 0x00, 0xFF, 0xFF };

		static constexpr vec4b violet	{ 0x80, 0x00, 0xFF, 0xFF };
		static constexpr vec4b lime		{ 0x80, 0xFF, 0x00, 0xFF };
		static constexpr vec4b orange	{ 0xFF, 0x80, 0x00, 0xFF };

		static constexpr vec4b fuchsia	{ 0xFF, 0x00, 0x80, 0xFF };
		static constexpr vec4b aqua		{ 0x00, 0xFF, 0x80, 0xFF };
		static constexpr vec4b azure	{ 0x00, 0x80, 0xFF, 0xFF };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_COLOR_HPP_