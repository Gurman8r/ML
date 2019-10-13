#ifndef _ML_HANDLE_HPP_
#define _ML_HANDLE_HPP_

#include <ML/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Handle is a representation of a hardware/device object.
	template <class T> struct Handle
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename T;
		using self_type			= typename Handle<value_type>;
		using pointer			= typename value_type * ;
		using reference			= typename value_type & ;
		using const_pointer		= typename const value_type * ;
		using const_reference	= typename const value_type & ;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Handle(const_reference value)
			: m_handle(value)
		{
		}

		virtual ~Handle() {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto get_address() const -> void * { return ML_ADDRESSOF(m_handle); }
		
		inline auto get_handle() -> reference { return m_handle; }
		
		inline auto get_handle() const -> const_reference { return m_handle; }

		inline auto set_handle(value_type value) -> self_type & { m_handle = value; return (*this); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline operator bool() const			{ return m_handle;	}
		inline operator pointer()				{ return &m_handle; }
		inline operator reference()				{ return m_handle;	}
		inline operator const_pointer() const	{ return &m_handle; }
		inline operator const_reference() const	{ return m_handle;	}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected: mutable value_type m_handle;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_HANDLE_HPP_