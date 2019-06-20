#ifndef _ML_ICON_HPP_
#define _ML_ICON_HPP_

#include <ML/Window/Export.hpp>
#include <ML/Core/INonNewable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct Icon final : public INonNewable
	{
		int32_t		width;
		int32_t		height;
		uint8_t *	pixels;

		constexpr Icon()
			: width(0), height(0), pixels(nullptr)
		{
		}

		constexpr Icon(const int32_t width, const int32_t height, uint8_t * pixels)
			: width(width), height(height), pixels(pixels)
		{
		}

		constexpr Icon(const int32_t width, const int32_t height, const uint8_t * pixels)
			: Icon(width, height, (uint8_t *)pixels)
		{
		}

		constexpr Icon(const Icon & copy)
			: Icon(copy.width, copy.height, copy.pixels)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ICON_HPP_