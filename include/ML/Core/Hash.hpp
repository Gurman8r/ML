#ifndef _ML_HASH_HPP_
#define _ML_HASH_HPP_

// Sources:
// https://github.com/Manu343726/ctti/blob/master/include/ctti/detail/hash.hpp
// http://www.isthe.com/chongo/tech/comp/fnv/#FNV-param

#include <ML/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using hash_t = typename uint64_t;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace hash
	{
		template <typename T>
		struct fnv1a final
		{
			enum : hash_t
			{
				Basis = 14695981039346656037ULL,
				Prime = 1099511628211ULL
			};

			constexpr fnv1a(size_t size, const T * arr, hash_t hash)
				: m_value { ((size > 0)
				? (fnv1a(size - 1, arr + 1, (hash ^ *arr) * fnv1a::Prime))
				: (hash)
			) }
			{
			}

			constexpr fnv1a(size_t size, const T * arr)
				: fnv1a(size, arr, fnv1a::Basis)
			{
			}

			template <size_t N>
			constexpr fnv1a(const T(&value)[N])
				: fnv1a((N - 1), &value[0])
			{
			}

			constexpr operator hash_t() const { return m_value; }

		private:
			hash_t m_value;
		};
	}

	namespace detail
	{
		template <typename T>
		struct hash_impl;

		struct fnv1a
		{
			enum : hash_t
			{
				Basis = 14695981039346656037ULL,
				Prime = 1099511628211ULL
			};
		};

		template <typename T>
		struct hash
		{
			constexpr hash_t operator()(const T & value) const
			{
				return hash_impl<T>()(value);
			}
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_HASH_HPP_