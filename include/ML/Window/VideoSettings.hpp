#ifndef _ML_SCREEN_HPP_
#define _ML_SCREEN_HPP_

#include <ML/Window/Export.hpp>
#include <ML/Core/Matrix.hpp>
#include <ML/Core/List.hpp>
#include <ML/Core/I_Newable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API VideoSettings final
		: public I_Newable
	{
		/* * * * * * * * * * * * * * * * * * * * */

		vec2u	 resolution;
		uint32_t colorDepth;

		/* * * * * * * * * * * * * * * * * * * * */

		VideoSettings();
		VideoSettings(uint32_t width, uint32_t height, uint32_t colorDepth);
		VideoSettings(const vec2u & size, uint32_t colorDepth);
		VideoSettings(const VideoSettings & copy);
		~VideoSettings();

		/* * * * * * * * * * * * * * * * * * * * */

		static const VideoSettings & desktop();
		static const List<VideoSettings> & resolutions();

		/* * * * * * * * * * * * * * * * * * * * */

		inline const uint32_t & width()  const { return resolution[0]; }
		inline const uint32_t & height() const { return resolution[1]; }

		/* * * * * * * * * * * * * * * * * * * * */

		inline bool isValidDesktopResolution() const
		{
			return std::find(
				resolutions().begin(),
				resolutions().end(), 
				(*this)
			) != resolutions().end();
		}

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline ML_SERIALIZE(ostream & out, const VideoSettings & value)
	{
		return out << value.resolution << " " << value.colorDepth;
	}

	inline ML_DESERIALIZE(istream & in, VideoSettings & value)
	{
		return in >> value.resolution >> value.colorDepth;
	}

	inline bool operator==(const VideoSettings & lhs, const VideoSettings & rhs)
	{
		return
			lhs.resolution == rhs.resolution &&
			lhs.colorDepth == rhs.colorDepth;
	}

	inline bool operator!=(const VideoSettings & lhs, const VideoSettings & rhs)
	{
		return !(lhs == rhs);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SCREEN_HPP_