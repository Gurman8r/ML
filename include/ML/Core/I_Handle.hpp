#ifndef _ML_I_HANDLE_HPP_
#define _ML_I_HANDLE_HPP_

#include <ML/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Handle is a representation of a hardware/device object.
	template <class T> struct I_Handle
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename T;
		using self_type			= typename I_Handle<value_type>;
		using pointer			= typename value_type * ;
		using reference			= typename value_type & ;
		using const_pointer		= typename const value_type * ;
		using const_reference	= typename const value_type & ;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		I_Handle(const_reference value)
			: m_handle(value)
		{
		}

		virtual ~I_Handle() {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline void * get_handle() const
		{ 
			return ML_ADDRESSOF(m_handle);
		}

		inline self_type & set_handle(const_reference value)
		{
			m_handle = value;
			return (*this);
		}

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

#endif // !_ML_I_HANDLE_HPP_