#ifndef _ML_VECTOR_2_HPP_
#define _ML_VECTOR_2_HPP_

#include <ML/Core/Vector.hpp>

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class _Elem
	> class Vector2
		: public Vector<_Elem, 2>
	{
	public: // Usings
		/* * * * * * * * * * * * * * * * * * * * */
		using value_type			= typename _Elem;
		using self_type				= typename Vector2<value_type>;
		using base_type				= typename Vector<value_type, 2>;

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
		Vector2()
			: base_type()
		{
		}

		Vector2(const_reference xy)
			: base_type(xy)
		{
		}

		Vector2(const_reference x, const_reference y)
			: base_type({ x, y })
		{
		}
		
		template <
			size_t S
		> Vector2(const Vector<value_type, S> & copy)
			: base_type(copy)
		{
		}

		template <
			class T
		> Vector2(const Vector2<T> & copy)
			: base_type(copy)
		{
		}

		template <
			class T, 
			size_t S
		> Vector2(const Vector<T, S> & copy, const_reference def = static_cast<value_type>(0))
			: base_type(copy, def)
		{
		}

		virtual ~Vector2() {}

		
	public: // Static Functions
		/* * * * * * * * * * * * * * * * * * * * */
		inline static float angle(const self_type & from, const self_type & to)
		{
			return angle(to - from);
		};

		inline static float angle(const self_type & value)
		{
			return std::atan2f(value[0], value[1]);
		};

		inline static float det(const self_type & a, const self_type & b)
		{
			return 
				(static_cast<float>(a[0]) * static_cast<float>(b[1])) - 
				(static_cast<float>(a[1]) * static_cast<float>(b[0]));
		};

		inline static self_type angle(float value)
		{
			return self_type(cos(value), sin(value));
		};


	public: // Constants
		/* * * * * * * * * * * * * * * * * * * * */
		static const self_type Zero;
		static const self_type One;
		static const self_type Up;
		static const self_type Down;
		static const self_type Left;
		static const self_type Right;

# ifdef GLM_VERSION
	public: // GLM
		/* * * * * * * * * * * * * * * * * * * * */
		Vector2(const glm::tvec2<value_type, glm::defaultp> & value)
			: self_type(value.x, value.y)
		{
		}

		inline operator glm::tvec2<value_type, glm::defaultp>() const
		{
			return glm::tvec2<value_type, glm::defaultp>((*this)[0], (*this)[1]);
		}
# endif
	};

	// Constants
	/* * * * * * * * * * * * * * * * * * * * */
	template <class T> const Vector2<T> Vector2<T>::Zero	= Vector2<T>( 0,  0);
	template <class T> const Vector2<T> Vector2<T>::One		= Vector2<T>( 1,  1);
	template <class T> const Vector2<T> Vector2<T>::Up		= Vector2<T>( 0,  1);
	template <class T> const Vector2<T> Vector2<T>::Down	= Vector2<T>( 0, -1);
	template <class T> const Vector2<T> Vector2<T>::Left	= Vector2<T>(-1,  0);
	template <class T> const Vector2<T> Vector2<T>::Right	= Vector2<T>( 1,  0);

	// Types
	/* * * * * * * * * * * * * * * * * * * * */
	using vec2f = typename Vector2<float>;
	using vec2i = typename Vector2<int32_t>;
	using vec2d = typename Vector2<double>;
	using vec2b = typename Vector2<uint8_t>;
	using vec2u = typename Vector2<uint32_t>;
	using vec2	= typename vec2f;
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_VECTOR_2_HPP_