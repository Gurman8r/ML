#ifndef _ML_VIDEO_MODE_HPP_
#define _ML_VIDEO_MODE_HPP_

#include <ML/Window/Export.hpp>
#include <ML/Core/Matrix.hpp>
#include <ML/Core/List.hpp>
#include <ML/Core/I_Newable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API VideoMode final
		: public I_Newable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		vec2u	 resolution;
		uint32_t colorDepth;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		VideoMode();
		VideoMode(uint32_t width, uint32_t height, uint32_t colorDepth);
		VideoMode(const vec2u & size, uint32_t colorDepth);
		VideoMode(const VideoMode & copy);
		~VideoMode();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static const VideoMode & get_desktop();
		static const List<VideoMode> & get_modes();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline const uint32_t & width()  const { return resolution[0]; }
		inline const uint32_t & height() const { return resolution[1]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool isValidDesktopResolution() const
		{
			return std::find(
				get_modes().begin(),
				get_modes().end(), 
				(*this)
			) != get_modes().end();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	inline ML_SERIALIZE(Ostream & out, const VideoMode & value)
	{
		return out << value.resolution << " " << value.colorDepth;
	}

	inline ML_DESERIALIZE(Istream & in, VideoMode & value)
	{
		return in >> value.resolution >> value.colorDepth;
	}

	inline bool operator==(const VideoMode & lhs, const VideoMode & rhs)
	{
		return
			lhs.resolution == rhs.resolution &&
			lhs.colorDepth == rhs.colorDepth;
	}

	inline bool operator!=(const VideoMode & lhs, const VideoMode & rhs)
	{
		return !(lhs == rhs);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_VIDEO_MODE_HPP_