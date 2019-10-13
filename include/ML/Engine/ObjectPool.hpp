#ifndef _ML_OBJECT_POOL_HPP_
#define _ML_OBJECT_POOL_HPP_

#include <ML/Core/Disposable.hpp>
#include <ML/Core/Newable.hpp>
#include <ML/Core/Array.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// WIP
	template <class T, size_t N> struct ObjectPool final
		: public Disposable
		, public Newable
		, public NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename T;
		using self_type			= typename ObjectPool<T, N>;
		using pointer			= typename value_type *;
		using reference			= typename value_type &;
		using const_pointer		= typename const value_type *;
		using const_reference	= typename const value_type &;

		enum : size_t { Size = N };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ObjectPool() {}

		~ObjectPool() { dispose(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool dispose() override
		{
			return true;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		Array<T, N> m_data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_OBJECT_POOL_HPP_