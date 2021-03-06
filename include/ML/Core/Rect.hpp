#ifndef _ML_RECT_HPP_
#define _ML_RECT_HPP_

#include <ML/Core/Matrix.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <class T> struct Rect final : public tvec4<T>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		using value_type		= typename T;
		using self_type			= typename Rect<value_type>;
		using base_type			= typename tvec4<value_type>;
		using coord_type		= typename tvec2<value_type>;
		using pointer			= typename base_type::pointer;
		using reference			= typename base_type::reference;
		using const_pointer		= typename base_type::const_pointer;
		using const_reference	= typename base_type::const_reference;
		using iterator			= typename base_type::iterator;
		using const_iterator	= typename base_type::const_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr Rect()
			: base_type()
		{
		}
		
		constexpr Rect(value_type width, value_type height)
			: base_type({ 0, 0, width, height })
		{
		}
		
		constexpr Rect(value_type left, value_type top, value_type width, value_type height)
			: base_type({ left, top, width, height })
		{
		}

		constexpr Rect(self_type const & copy)
			: base_type(copy)
		{
		}

		template <class U> constexpr Rect(const tvec4<U> & copy)
			: base_type(copy)
		{
		}
		
		constexpr Rect(coord_type const & pos, coord_type const & size)
			: base_type({ pos[0], pos[1], size[0], size[1] })
		{
		}
		
		constexpr Rect(coord_type const & size)
			: base_type({ 0, 0, size[0], size[1] })
		{
		}
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		constexpr auto left()		const -> value_type { return (*this)[0]; }
		constexpr auto top()		const -> value_type { return (*this)[1]; }
		constexpr auto width()		const -> value_type { return (*this)[2]; }
		constexpr auto height()		const -> value_type { return (*this)[3]; }
		constexpr auto bot()		const -> value_type { return (top() + height()); }
		constexpr auto right()		const -> value_type { return (left() + width()); }
		constexpr auto position()	const -> coord_type { return { left(), top() }; }
		constexpr auto size()		const -> coord_type { return { width(), height() }; }
		constexpr auto center()		const -> coord_type { return (position() + (size() / (T)2)); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr auto left		(value_type value)			-> self_type & { return set(0, value); }
		constexpr auto top		(value_type value)			-> self_type & { return set(1, value); }
		constexpr auto width	(value_type value)			-> self_type & { return set(2, value); }
		constexpr auto height	(value_type value)			-> self_type & { return set(3, value); }
		constexpr auto bot		(value_type value)			-> self_type & { return height(value - top()); }
		constexpr auto right	(value_type value)			-> self_type & { return width(value - left()); }
		constexpr auto position	(coord_type const & value)	-> self_type & { return left(value[0]).top(value[1]); }
		constexpr auto size		(coord_type const & value)	-> self_type & { return width(value[0]).height(value[1]); }
		constexpr auto center	(coord_type const & value)	-> self_type & { return position(value - (size() / (T)2)); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	ML_USING FloatRect = Rect<float_t>;
	ML_USING IntRect	= Rect<int32_t>;
	ML_USING UintRect	= Rect<uint32_t>;

	/* * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_RECT_HPP_