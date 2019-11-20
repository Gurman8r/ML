#ifndef _ML_SCOPED_BINDER_HPP_
#define _ML_SCOPED_BINDER_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Core/StandardLib.hpp>
#include <ML/Core/NonCopyable.hpp>

// Named scope binder
#define ML_BIND_SCOPE_EX(T, var, value, ...) \
	ScopedBinder<T> var { value, ##__VA_ARGS__ }

// Anonymous scope binder
#define ML_BIND_SCOPE(T, value, ...) \
	ML_BIND_SCOPE_EX(T, ML_ANON(ML_CONCAT(ScopedBinder_, T)), value, ##__VA_ARGS__)

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <class ... T> struct ScopedBinder;

	template <> struct ScopedBinder<> final { ScopedBinder() = delete; };

	/* * * * * * * * * * * * * * * * * * * * */

	template <class T> struct ML_GRAPHICS_API ScopedBinder<T> : public NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class ... Args
		> explicit ScopedBinder(T const * value, Args && ... args)
			: m_value{ value }
		{
			if (m_value) { m_value->bind(std::forward<Args>(args)...); }
		}

		template <
			class ... Args
		> explicit ScopedBinder(T const & value, Args && ... args)
			: ScopedBinder { (value ? &value : nullptr), std::forward<Args>(args)... }
		{
		}

		~ScopedBinder()
		{
			if (m_value) { m_value->unbind(); }
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline operator bool() const { return m_value; }

		inline operator T const *() const { return m_value; }

		inline T * operator->() { return (T *)m_value; }

		inline T const * operator->() const { return m_value; }

		inline T & operator*() { return (T &)*m_value; }

		inline T const & operator*() const { return *m_value; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: T const * m_value;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SCOPED_BINDER_HPP_