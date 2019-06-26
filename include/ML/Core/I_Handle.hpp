#ifndef _ML_I_HANDLE_HPP_
#define _ML_I_HANDLE_HPP_

#include <ML/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class T
	> struct I_Handle
	{
		/* * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename T;
		using pointer			= typename value_type * ;
		using reference			= typename value_type & ;
		using const_pointer		= typename const value_type * ;
		using const_reference	= typename const value_type & ;

		/* * * * * * * * * * * * * * * * * * * * */

		I_Handle(const_reference value)
			: m_handle(value)
		{
		}

		virtual ~I_Handle() {}

		/* * * * * * * * * * * * * * * * * * * * */

		inline void * get_handle() const
		{ 
			return (void *)(intptr_t)(m_handle);
		}

		inline bool set_handle(const_reference value)
		{
			return ((!m_handle) && (m_handle = value));
		}

		/* * * * * * * * * * * * * * * * * * * * */

		inline auto good() const				-> bool { return  m_handle; }
		inline auto get_pointer()				-> pointer { return &m_handle; }
		inline auto get_reference()				-> reference { return  m_handle; }
		inline auto get_const_pointer() const	-> const_pointer { return &m_handle; }
		inline auto get_const_reference() const	-> const_reference { return  m_handle; }

		/* * * * * * * * * * * * * * * * * * * * */

		inline operator bool() const			{ return good(); }
		inline operator pointer()				{ return get_pointer(); }
		inline operator reference()				{ return get_reference(); }
		inline operator const_pointer() const	{ return get_const_pointer(); }
		inline operator const_reference() const	{ return get_const_reference(); }

		/* * * * * * * * * * * * * * * * * * * * */

	private: mutable value_type m_handle;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_I_HANDLE_HPP_