#ifndef _ML_VIDEO_MODE_HPP_
#define _ML_VIDEO_MODE_HPP_

#include <ML/Window/Export.hpp>
#include <ML/Core/Matrix.hpp>
#include <ML/Core/List.hpp>
#include <ML/Core/I_Newable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API VideoMode final : public I_Newable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		vec2u	 resolution;
		uint32_t bitsPerPixel;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		VideoMode();
		VideoMode(uint32_t width, uint32_t height, uint32_t bitsPerPixel);
		VideoMode(const vec2u & size, uint32_t bitsPerPixel);
		VideoMode(const VideoMode & copy);
		~VideoMode();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static const VideoMode &		get_desktop_mode();
		static const List<VideoMode> &	get_fullscreen_modes();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool is_valid() const
		{
			const List<VideoMode> & modes { get_fullscreen_modes() };
			return std::find(modes.begin(), modes.end(), (*this)) != modes.end();
		}
		
		inline auto width()  const -> const uint32_t & { return resolution[0]; }
		inline auto height() const -> const uint32_t & { return resolution[1]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	inline ML_SERIALIZE(Ostream & out, const VideoMode & value)
	{
		return out
			<< value.resolution << " "
			<< value.bitsPerPixel << " ";
	}

	inline ML_DESERIALIZE(Istream & in, VideoMode & value)
	{
		return in 
			>> value.resolution 
			>> value.bitsPerPixel;
	}

	inline bool operator==(const VideoMode & lhs, const VideoMode & rhs)
	{
		return (lhs.resolution == rhs.resolution) && (lhs.bitsPerPixel == rhs.bitsPerPixel);
	}

	inline bool operator!=(const VideoMode & lhs, const VideoMode & rhs)
	{
		return !(lhs == rhs);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_VIDEO_MODE_HPP_