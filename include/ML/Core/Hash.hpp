#ifndef _ML_HASH_HPP_
#define _ML_HASH_HPP_

#include <ML/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// fnv1a hash implementation
	struct Hash final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr auto basis { static_cast<hash_t>(14695981039346656037ULL) };
		static constexpr auto prime { static_cast<hash_t>(1099511628211ULL) };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Args> constexpr Hash(Args && ... args) noexcept
			: m_value { (*this)(std::forward<Args>(args)...) }
		{
		}

		constexpr Hash() noexcept : m_value { 0 } {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> constexpr hash_t operator()(const T * arr, hash_t size, hash_t seed) noexcept
		{
			return ((size > 0)
				? (*this)((arr + 1), (size - 1), (seed ^ static_cast<hash_t>(*arr)) * prime)
				: seed
			);
		}

		template <
			class T
		> constexpr hash_t operator()(const T * arr, hash_t size) noexcept
		{
			return (*this)(arr, size, basis);
		}

		template <
			class T, hash_t N
		> constexpr hash_t operator()(const T(&value)[N]) noexcept
		{
			return (*this)(value, (N - 1));
		}

		constexpr const hash_t & operator()() const noexcept
		{
			return m_value;
		}

		constexpr operator const hash_t &() const noexcept
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
