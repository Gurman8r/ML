#ifndef _ML_RECT_HPP_
#define _ML_RECT_HPP_

#include <ML/Core/Matrix.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class T
	> struct Rect final : public tvec4<T>
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
		
		constexpr Rect(const_reference width, const_reference height)
			: base_type({ 0, 0, width, height })
		{
		}
		
		constexpr Rect(const_reference left, const_reference top, const_reference width, const_reference height)
			: base_type({ left, top, width, height })
		{
		}

		constexpr Rect(const self_type & copy)
			: base_type(copy)
		{
		}

		template <
			class U
		> constexpr Rect(const tvec4<U> & copy)
			: base_type(copy)
		{
		}
		
		constexpr Rect(const coord_type & pos, const coord_type & size)
			: base_type({ pos[0], pos[1], size[0], size[1] })
		{
		}
		
		constexpr Rect(const coord_type & size)
			: base_type({ 0, 0, size[0], size[1] })
		{
		}
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		constexpr value_type left()		const { return (*this)[0]; }
		constexpr value_type top()		const { return (*this)[1]; }
		constexpr value_type width()	const { return (*this)[2]; }
		constexpr value_type height()	const { return (*this)[3]; }
		constexpr value_type bottom()	const { return (top() + height()); }
		constexpr value_type right()	const { return (left() + width()); }
		constexpr coord_type position()	const { return { left(), top() }; }
		constexpr coord_type size()		const { return { width(), height() }; }
		constexpr coord_type center()	const { return (position() + (size() / static_cast<value_type>(2))); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr self_type & left		(const_reference value)		{ return set(0, value); }
		constexpr self_type & top		(const_reference value)		{ return set(1, value); }
		constexpr self_type & width		(const_reference value)		{ return set(2, value); }
		constexpr self_type & height	(const_reference value)		{ return set(3, value); }
		constexpr self_type & bottom	(const_reference value)		{ return height(value - top()); }
		constexpr self_type & right		(const_reference value)		{ return width(value - left()); }
		constexpr self_type & position	(const coord_type & value)	{ return left(value[0]).top(value[1]); }
		constexpr self_type & size		(const coord_type & value)	{ return width(value[0]).height(value[1]); }
		constexpr self_type & center	(const coord_type & value)	{ return position(value - (size() / static_cast<value_type>(2))); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		constexpr operator base_type() const
		{
			return base_type { (*this)[0], (*this)[1], (*this)[2], (*this)[3] };
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	using FloatRect = Rect<float_t>;
	using IntRect	= Rect<int32_t>;
	using UintRect	= Rect<uint32_t>;

	/* * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_RECT_HPP_