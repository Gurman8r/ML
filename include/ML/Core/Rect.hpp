#ifndef _ML_RECT_HPP_
#define _ML_RECT_HPP_

#include <ML/Core/Vector4.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class _Elem
	> class Rect final
		: public Vector4<_Elem>
	{
	public: // Usings
		/* * * * * * * * * * * * * * * * * * * * */
		using value_type			= typename _Elem;
		using self_type				= typename Rect<value_type>;
		using base_type				= typename Vector4<value_type>;
		using coord_type			= typename Vector<value_type, 2>;

		using init_type				= typename base_type::init_type;
		using array_type			= typename base_type::array_type;
		using const_value			= typename base_type::const_value;
		using pointer				= typename base_type::pointer;
		using reference				= typename base_type::reference;
		using const_pointer			= typename base_type::const_pointer;
		using const_reference		= typename base_type::const_reference;

		using iterator				= typename base_type::iterator;
		using const_iterator		= typename base_type::const_iterator;
		using reverse_iterator		= typename base_type::reverse_iterator;
		using const_reverse_iterator= typename base_type::const_reverse_iterator;


	public: // Constructors
		/* * * * * * * * * * * * * * * * * * * * */
		Rect()
			: base_type()
		{
		}
		
		Rect(const_reference width, const_reference height)
			: base_type(0, 0, width, height)
		{
		}
		
		Rect(const_reference left, const_reference top, const_reference width, const_reference height)
			: base_type(left, top, width, height)
		{
		}
		
		Rect(const self_type & copy)
			: base_type(copy)
		{
		}
		
		Rect(const coord_type & pos, const coord_type & size)
			: base_type(pos[0], pos[1], size[0], size[1])
		{
		}
		
		Rect(const coord_type & size)
			: base_type(0, 0, size[0], size[1])
		{
		}
		
		~Rect() {}


	public: // Member Functions
		/* * * * * * * * * * * * * * * * * * * * */
		inline value_type left()	const { return this->get(0); }
		inline value_type top()		const { return this->get(1); }
		inline value_type width()	const { return this->get(2); }
		inline value_type height()	const { return this->get(3); }
		inline value_type bottom()	const { return (top() + height()); }
		inline value_type right()	const { return (left() + width()); }
		inline coord_type position()const { return { left(), top() }; }
		inline coord_type size()	const { return { width(), height() }; }
		inline coord_type center()	const { return (position() + (size() / static_cast<value_type>(2))); }

		/* * * * * * * * * * * * * * * * * * * * */

		inline self_type & left		(const_value value)	{ return set(0, value); }
		inline self_type & top		(const_value value)	{ return set(1, value); }
		inline self_type & width	(const_value value)	{ return set(2, value); }
		inline self_type & height	(const_value value)	{ return set(3, value); }
		inline self_type & bottom	(const_value value)	{ return height(value - top()); }
		inline self_type & right	(const_value value)	{ return width(value - left()); }
		inline self_type & position	(const coord_type & value)	{ return left(value[0]).top(value[1]); }
		inline self_type & size		(const coord_type & value)	{ return width(value[0]).height(value[1]); }
		inline self_type & center	(const coord_type & value)	{ return position(value - (size() / static_cast<value_type>(2))); }


	public: // Operators
		/* * * * * * * * * * * * * * * * * * * * */
		inline operator base_type() const
		{
			return base_type { (*this)[0], (*this)[1], (*this)[2], (*this)[3] };
		}
	};

	// Types
	/* * * * * * * * * * * * * * * * * * * * */
	using FloatRect = Rect<float>;
	using IntRect	= Rect<int32_t>;
	using UintRect	= Rect<uint32_t>;
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_RECT_HPP_