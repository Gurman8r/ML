#ifndef _ML_SCREEN_HPP_
#define _ML_SCREEN_HPP_

#include <ML/Window/Export.hpp>
#include <ML/Core/Vector2.hpp>
#include <ML/Core/List.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API VideoSettings final
		: public IObject
		, public IComparable<VideoSettings>
	{
		/* * * * * * * * * * * * * * * * * * * * */

		vec2u	 resolution;
		uint32_t colorDepth;

		/* * * * * * * * * * * * * * * * * * * * */

		VideoSettings();
		VideoSettings(const uint32_t width, const uint32_t height, const uint32_t colorDepth);
		VideoSettings(const vec2u & size, const uint32_t colorDepth);
		VideoSettings(const VideoSettings & copy);
		~VideoSettings();

		/* * * * * * * * * * * * * * * * * * * * */

		static const VideoSettings & desktop();
		static const List<VideoSettings> & resolutions();

		/* * * * * * * * * * * * * * * * * * * * */

		inline const uint32_t & width()  const { return resolution[0]; }
		inline const uint32_t & height() const { return resolution[1]; }

		/* * * * * * * * * * * * * * * * * * * * */

		void serialize(std::ostream & out) const override;
		bool equals(const VideoSettings & value) const override;
		bool lessThan(const VideoSettings & value) const override;

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

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SCREEN_HPP_