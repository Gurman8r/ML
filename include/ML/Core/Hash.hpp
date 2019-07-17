#ifndef _ML_HASH_HPP_
#define _ML_HASH_HPP_

#include <ML/Core/Type.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct Hash final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr hash_t basis { type_t<hash_t>(14695981039346656037ULL) };
		static constexpr hash_t prime { type_t<hash_t>(1099511628211ULL) };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> constexpr hash_t operator()(hash_t size, const T * arr, hash_t seed)
		{
			return ((size > 0)
				? (*this)(
					(size - 1), (arr + 1), (seed ^ type_t<hash_t>(*arr)) * prime)
				: seed
			);
		}

		template <
			class T
		> constexpr hash_t operator()(hash_t size, const T * arr)
		{
			return (*this)(size, arr, basis);
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
