#ifndef _ML_SCREEN_HPP_
#define _ML_SCREEN_HPP_

#include <ML/Window/Export.hpp>
#include <ML/Core/Vector2.hpp>
#include <ML/Core/List.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API Screen final
		: public ITrackable
		, public IComparable<Screen>
	{
		/* * * * * * * * * * * * * * * * * * * * */

		vec2u	 resolution;
		uint32_t colorDepth;

		/* * * * * * * * * * * * * * * * * * * * */

		Screen();
		Screen(const uint32_t width, const uint32_t height);
		Screen(const vec2u & size);
		Screen(const uint32_t width, const uint32_t height, const uint32_t colorDepth);
		Screen(const vec2u & size, const uint32_t colorDepth);
		Screen(const Screen & copy);
		~Screen();

		/* * * * * * * * * * * * * * * * * * * * */

		static const Screen &		desktop();
		static const List<Screen> & resolutions();

		/* * * * * * * * * * * * * * * * * * * * */

		inline const uint32_t & width()  const { return resolution[0]; }
		inline const uint32_t & height() const { return resolution[1]; }

		/* * * * * * * * * * * * * * * * * * * * */

		void serialize(std::ostream & out) const override;
		void deserialize(std::istream & in) override;

		/* * * * * * * * * * * * * * * * * * * * */

		bool equals(const Screen & value) const override;
		bool lessThan(const Screen & value) const override;

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