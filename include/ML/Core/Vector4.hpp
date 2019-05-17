#ifndef _ML_VECTOR_4_HPP_
#define _ML_VECTOR_4_HPP_

#include <ML/Core/Vector.hpp>

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class _Elem
	> class Vector4
		: public Vector<_Elem, 4>
	{
	public: // Usings
		/* * * * * * * * * * * * * * * * * * * * */
		using value_type			= typename _Elem;
		using self_type				= typename Vector4<value_type>;
		using base_type				= typename Vector<value_type, 4>;

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
		Vector4()
			: base_type()
		{
		}

		Vector4(const_reference xyzw)
			: base_type(xyzw)
		{
		}

		Vector4(const_reference x, const_reference y, const_reference z, const_reference w)
			: base_type({ x, y, z, w })
		{
		}

		template <
			size_t S
		> Vector4(const Vector<value_type, S> & copy)
			: base_type(copy)
		{
		}

		template <
			class T
		> Vector4(const Vector4<T> & copy)
			: base_type(copy)
		{
		}

		template <
			class T, 
			size_t S
		> Vector4(const Vector<T, S> & copy, const_reference def = static_cast<value_type>(0))
			: base_type(copy, def)
		{
		}

		virtual ~Vector4() {}


	public: // Constants
		/* * * * * * * * * * * * * * * * * * * * */
		static const self_type Zero;
		static const self_type One;

# ifdef GLM_VERSION
	public: // GLM
		/* * * * * * * * * * * * * * * * * * * * */
		Vector4(const glm::tvec4<value_type, glm::defaultp> & value)
			: self_type(value.x, value.y, value.z, value.w)
		{
		}

		inline operator glm::tvec4<value_type, glm::defaultp>() const
		{
			return glm::tvec4<value_type, glm::defaultp>(
				(*this)[0], (*this)[1], (*this)[2], (*this)[3]
			);
		}
# endif
	};

	// Constants
	/* * * * * * * * * * * * * * * * * * * * */
	template <class T> const Vector4<T> Vector4<T>::Zero = Vector4<T>(0);
	template <class T> const Vector4<T> Vector4<T>::One	 = Vector4<T>(1);

	// Types
	/* * * * * * * * * * * * * * * * * * * * */
	using vec4f = typename Vector4<float>;
	using vec4i = typename Vector4<int32_t>;
	using vec4d = typename Vector4<double>;
	using vec4b = typename Vector4<uint8_t>;
	using vec4u = typename Vector4<uint32_t>;
	using vec4	= typename vec4f;
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_VECTOR_4_HPP_