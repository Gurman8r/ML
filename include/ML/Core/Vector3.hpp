#ifndef _ML_VECTOR_3_HPP_
#define _ML_VECTOR_3_HPP_

#include <ML/Core/Vector.hpp>

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class _Elem
	> class Vector3
		: public Vector<_Elem, 3>
	{
	public: // Usings
		/* * * * * * * * * * * * * * * * * * * * */
		using value_type			= typename _Elem;
		using self_type				= typename Vector3<value_type>;
		using base_type				= typename Vector<value_type, 3>;

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
		Vector3()
			: base_type()
		{
		}

		Vector3(const_reference xyz)
			: base_type(xyz)
		{
		}

		Vector3(const_reference x, const_reference y)
			: base_type({ x, y, static_cast<value_type>(0) })
		{
		}

		Vector3(const_reference x, const_reference y, const_reference z)
			: base_type({ x, y, z })
		{
		}

		template <
			size_t S
		> Vector3(const Vector<value_type, S> & copy)
			: base_type(copy)
		{
		}

		template <
			class T
		> Vector3(const Vector3<T> & copy)
			: base_type(copy)
		{
		}
		
		template <
			class  T, 
			size_t S
		> Vector3(const Vector<T, S> & copy, const_reference def = static_cast<value_type>(0))
			: base_type(copy, def)
		{
		}

		virtual ~Vector3() {}


	public: // Functions
		/* * * * * * * * * * * * * * * * * * * * */
		inline static self_type cross(const self_type & a, const self_type & b)
		{
			return self_type(
				(a[1] * b[2]) - (a[2] * b[1]),
				(a[2] * b[0]) - (a[0] * b[2]),
				(a[0] * b[1]) - (a[1] * b[0]));
		}

		inline self_type cross(const self_type & other) const
		{
			return self_type::cross((*this), other);
		}


	public: // Constants
		/* * * * * * * * * * * * * * * * * * * * */
		static const self_type Zero;
		static const self_type One;
		static const self_type Up;
		static const self_type Down;
		static const self_type Left;
		static const self_type Right;
		static const self_type Forward;
		static const self_type Back;

# ifdef GLM_VERSION
	public: // GLM
		/* * * * * * * * * * * * * * * * * * * * */
		Vector3(const glm::tvec3<value_type, glm::defaultp> & value)
			: self_type(value.x, value.y, value.z)
		{
		}

		inline operator glm::tvec3<value_type, glm::defaultp>() const
		{
			return glm::tvec3<value_type, glm::defaultp>(
				(*this)[0], (*this)[1], (*this)[2]
			);
		}
# endif
	};

	// Constants
	/* * * * * * * * * * * * * * * * * * * * */
	template <class T> const Vector3<T> Vector3<T>::Zero	= Vector3<T>( 0,  0,  0);
	template <class T> const Vector3<T> Vector3<T>::One		= Vector3<T>( 1,  1,  1);
	template <class T> const Vector3<T> Vector3<T>::Up		= Vector3<T>( 0,  1,  0);
	template <class T> const Vector3<T> Vector3<T>::Down	= Vector3<T>( 0, -1,  0);
	template <class T> const Vector3<T> Vector3<T>::Left	= Vector3<T>(-1,  0,  0);
	template <class T> const Vector3<T> Vector3<T>::Right	= Vector3<T>( 1,  0,  0);
	template <class T> const Vector3<T> Vector3<T>::Forward = Vector3<T>( 0,  0,  1);
	template <class T> const Vector3<T> Vector3<T>::Back	= Vector3<T>( 0,  0, -1);

	// Types
	/* * * * * * * * * * * * * * * * * * * * */
	using vec3f = typename Vector3<float>;
	using vec3i = typename Vector3<int32_t>;
	using vec3d = typename Vector3<double>;
	using vec3b = typename Vector3<uint8_t>;
	using vec3u = typename Vector3<uint32_t>;
	using vec3	= typename vec3f;
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_VECTOR_3_HPP_