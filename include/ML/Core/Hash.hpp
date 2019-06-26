#ifndef _ML_HASH_HPP_
#define _ML_HASH_HPP_

#include <ML/Core/Type.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct Hash final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using type = type_t<hash_t>;

		static constexpr hash_t fnv1a_basis { type(14695981039346656037ULL)() };
		static constexpr hash_t fnv1a_prime { type(1099511628211ULL)() };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> constexpr hash_t operator()(hash_t size, const T * arr, hash_t seed)
		{
			return ((size > 0)
				? (*this)(
					(size - 1), (arr + 1), (seed ^ type{ *arr }()) * fnv1a_prime)
				: seed
			);
		}

		template <
			class T
		> constexpr hash_t operator()(hash_t size, const T * arr)
		{
			return (*this)(size, arr, fnv1a_basis);
		}

		template <
			class T, hash_t N
		> constexpr hash_t operator()(const T(&value)[N])
		{
			return (*this)(N - 1, value);
		}

		template <
			class T
		> inline hash_t operator()(const T & value)
		{
			return std::hash<T>()(value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_HASH_HPP_
