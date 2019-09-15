#ifndef _ML_VIDEO_MODE_HPP_
#define _ML_VIDEO_MODE_HPP_

#include <ML/Window/Export.hpp>
#include <ML/Core/Matrix.hpp>
#include <ML/Core/List.hpp>
#include <ML/Core/I_NonNewable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct VideoMode final : public I_NonNewable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		vec2u	 size;
		uint32_t depth;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr VideoMode(const vec2u & size, uint32_t depth)
			: size { size }
			, depth { depth }
		{
		}

		constexpr VideoMode(uint32_t width, uint32_t height, uint32_t depth)
			: VideoMode { { width, height }, depth }
		{
		}
		
		constexpr VideoMode(const VideoMode & copy)
			: VideoMode { copy.size, copy.depth }
		{
		}

		constexpr VideoMode()
			: VideoMode { { NULL }, 0 }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		constexpr auto width()  const -> const uint32_t & { return size[0]; }
		constexpr auto height() const -> const uint32_t & { return size[1]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline operator bool() const
		{
			return (this->size[0] > 0) && (this->size[1] > 0) && (this->depth > 0);
		}

		constexpr bool operator==(const VideoMode & other)
		{
			return (this->size == other.size) && (this->depth == other.depth);
		}

		constexpr bool operator!=(const VideoMode & other)
		{
			return !((*this) == other);
		}

		constexpr bool operator<(const VideoMode & other)
		{
			return (this->size < other.size) || (this->depth < other.depth);
		}

		constexpr bool operator>(const VideoMode & other)
		{
			return !((*this) < other);
		}

		constexpr bool operator<=(const VideoMode & other)
		{
			return ((*this) == other) || ((*this) < other);
		}

		constexpr bool operator>=(const VideoMode & other)
		{
			return ((*this) == other) || ((*this) > other);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	inline ML_SERIALIZE(Ostream & out, const VideoMode & value)
	{
		return out << value.size << " " << value.depth << " ";
	}

	inline ML_DESERIALIZE(Istream & in, VideoMode & value)
	{
		return in >> value.size >> value.depth;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_VIDEO_MODE_HPP_