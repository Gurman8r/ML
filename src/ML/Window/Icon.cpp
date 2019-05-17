#include <ML/Window/Icon.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Icon::Icon()
		: width	(0)
		, height(0)
		, pixels(0)
	{
	}

	Icon::Icon(const int32_t width, const int32_t height, uint8_t * pixels)
		: width(width)
		, height(height)
		, pixels(pixels)
	{
	}

	Icon::Icon(const int32_t width, const int32_t height, const uint8_t * pixels)
		: width	(width)
		, height(height)
		, pixels(std::remove_cv_t<uint8_t *>(pixels))
	{
	}

	Icon::Icon(const Icon & copy)
		: width	(copy.width)
		, height(copy.height)
		, pixels(copy.pixels)
	{
	}

	Icon::~Icon() { }

	/* * * * * * * * * * * * * * * * * * * * */

	void Icon::serialize(std::ostream & out) const
	{
		out << GetTypeName() << " | " << width << " " << height << " ";
	}

	void Icon::deserialize(std::istream & in)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Icon::equals(const Icon & other) const
	{
		return 
			(width == other.width) && 
			(height == other.height) &&
			(std::memcmp(pixels, other.pixels, (width * height)) == 0);
	}

	bool Icon::lessThan(const Icon & other) const
	{
		return
			(width < other.width) || 
			(height < other.height) ||
			(std::memcmp(pixels, other.pixels, (width * height)) < 0);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}