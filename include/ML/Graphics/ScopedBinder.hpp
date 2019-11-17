#ifndef _ML_SCOPED_BINDER_HPP_
#define _ML_SCOPED_BINDER_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <class ... T> struct ScopedBinder;

	template <> struct ScopedBinder<> final { ScopedBinder() = delete; };

	template <class T> struct ML_GRAPHICS_API ScopedBinder<T> final
	{
		template <class ... Args> ScopedBinder(ptr_t<T> value, Args && ... args) 
			: m_value{ value }
		{
			if (m_value) { m_value->bind(std::forward<Args>(args)...); }
		}

		~ScopedBinder()
		{
			if (m_value) { m_value->unbind(); }
		}

		inline operator ptr_t<T>() { return m_value; }

		inline operator const_ptr_t<T>() const { return m_value; }

		inline ptr_t<T> operator->() { return m_value; }

		inline const_ptr_t<T> operator->() const { return m_value; }

	private: ptr_t<T> m_value;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SCOPED_BINDER_HPP_