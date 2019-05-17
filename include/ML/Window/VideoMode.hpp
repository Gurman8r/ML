#ifndef _ML_VIDEO_MODE_HPP_
#define _ML_VIDEO_MODE_HPP_

#include <ML/Window/Export.hpp>
#include <ML/Core/Vector2.hpp>
#include <ML/Core/List.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API VideoMode final
		: public ITrackable
		, public IComparable<VideoMode>
	{
		/* * * * * * * * * * * * * * * * * * * * */

		vec2u	 size;
		uint32_t bitsPerPixel;

		/* * * * * * * * * * * * * * * * * * * * */

		VideoMode();
		VideoMode(uint32_t width, uint32_t height, uint32_t bitsPerPixel = 32);
		VideoMode(const vec2u & size, uint32_t bitsPerPixel = 32);
		VideoMode(const VideoMode & copy);
		~VideoMode();

		/* * * * * * * * * * * * * * * * * * * * */

		static const VideoMode & desktop();
		static const List<VideoMode> & resolutions();

		/* * * * * * * * * * * * * * * * * * * * */

		inline const uint32_t & width()  const { return size[0]; }
		inline const uint32_t & height() const { return size[1]; }

		/* * * * * * * * * * * * * * * * * * * * */

		void serialize(std::ostream & out) const override;
		void deserialize(std::istream & in) override;

		/* * * * * * * * * * * * * * * * * * * * */

		bool equals(const VideoMode & value) const override;
		bool lessThan(const VideoMode & value) const override;

		/* * * * * * * * * * * * * * * * * * * * */

		inline bool good() const
		{
			return std::find(
				resolutions().begin(),
				resolutions().end(), 
				(*this)
			) != resolutions().end();
		}

		inline operator bool() const { return good(); }

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_VIDEO_MODE_HPP_