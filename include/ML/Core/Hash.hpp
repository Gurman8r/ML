#ifndef _ML_HASH_HPP_
#define _ML_HASH_HPP_

#include <ML/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct Hash final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr auto basis { static_cast<hash_t>(14695981039346656037ULL) };
		static constexpr auto prime { static_cast<hash_t>(1099511628211ULL) };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class ... Args
		> constexpr Hash(Args && ... args)
			: m_value { (*this)(std::forward<Args>(args)...) }
		{
		}

		constexpr Hash() : m_value { 0 } {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> constexpr hash_t operator()(const T * arr, hash_t size, hash_t seed)
		{
			return ((size > 0)
				? (*this)((arr + 1), (size - 1), (seed ^ static_cast<hash_t>(*arr)) * prime)
				: seed
			);
		}

		template <
			class T
		> constexpr hash_t operator()(const T * arr, hash_t size)
		{
			return (*this)(arr, size, basis);
		}

		template <
			class T, hash_t N
		> constexpr hash_t operator()(const T(&value)[N])
		{
			return (*this)(value, (N - 1));
		}

		template <
			class T
		> inline hash_t operator()(const T & value)
		{
			return static_cast<hash_t>(std::hash<T>()(value));
		}

		constexpr const hash_t & operator()() const
		{
			return m_value;
		}

		constexpr operator const hash_t &() const
		{
			return (*this)();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: hash_t m_value;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_HASH_HPP_
