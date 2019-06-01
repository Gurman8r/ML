#include <ML/Core/Meta.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace meta
	{
		// TYPE_T OPERATORS
		/* * * * * * * * * * * * * * * * * * * * */

		template <
			template <class> class Tty, class T
		> constexpr bool operator==(const Tty<T> & lhs, const Tty<T> & rhs)
		{
			return (lhs() == rhs());
		}

		template <
			template <class> class Tty, class T
		> constexpr bool operator!=(const Tty<T> & lhs, const Tty<T> & rhs)
		{
			return !(lhs == rhs);
		}

		template <
			template <class> class Tty, class T
		> constexpr bool operator<(const Tty<T> & lhs, const Tty<T> & rhs)
		{
			return (lhs() < rhs());
		}

		template <
			template <class> class Tty, class T
		> constexpr bool operator>(const Tty<T> & lhs, const Tty<T> & rhs)
		{
			return !(lhs < rhs);
		}

		template <
			template <class> class Tty, class T
		> constexpr bool operator<=(const Tty<T> & lhs, const Tty<T> & rhs)
		{
			return (lhs < rhs) || (lhs == rhs);
		}

		template <
			template <class> class Tty, class T
		> constexpr bool operator>=(const Tty<T> & lhs, const Tty<T> & rhs)
		{
			return (lhs > rhs) || (lhs == rhs);
		}

		/* * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace meta
	{
		// C_STRING OPERATORS
		/* * * * * * * * * * * * * * * * * * * * */

		constexpr bool operator==(const c_string & lhs, const c_string & rhs)
		{
			return alg::range::equals(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
		}

		constexpr bool operator!=(const c_string & lhs, const c_string & rhs)
		{
			return !(lhs == rhs);
		}

		constexpr bool operator<(const c_string & lhs, const c_string & rhs)
		{
			return alg::range::less(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
		}

		constexpr bool operator>(const c_string & lhs, const c_string & rhs)
		{
			return !(lhs > rhs);
		}

		constexpr bool operator<=(const c_string & lhs, const c_string & rhs)
		{
			return (lhs < rhs) || (lhs == rhs);
		}

		constexpr bool operator>=(const c_string & lhs, const c_string & rhs)
		{
			return (lhs > rhs) || (lhs == rhs);
		}

		/* * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace meta
	{
		// ARRAY / MATRIX OPERATORS
		/* * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr bool operator==(const M<T, N...> & lhs, const M<T, N...> & rhs)
		{
			return alg::range::equals(lhs, rhs);
		}

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr bool operator!=(const M<T, N...> & lhs, const M<T, N...> & rhs)
		{
			return !(lhs == rhs);
		}

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr bool operator<(const M<T, N...> & lhs, const M<T, N...> & rhs)
		{
			return alg::range::less(lhs, rhs);
		}

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr bool operator<=(const M<T, N...> & lhs, const M<T, N...> & rhs)
		{
			return (lhs < rhs) || (lhs == rhs);
		}

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr bool operator>(const M<T, N...> & lhs, const M<T, N...> & rhs)
		{
			return !(lhs < rhs) && (lhs != rhs);
		}

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr bool operator>=(const M<T, N...> & lhs, const M<T, N...> & rhs)
		{
			return (lhs > rhs) || (lhs == rhs);
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr auto operator+=(M<T, N...> & lhs, const M<T, N...> & rhs)
			-> M<T, N...> &
		{
			for (size_t i = 0; i < lhs.size(); i++)
			{
				lhs[i] = (lhs[i] + rhs[i]);
			}
			return lhs;
		}

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr auto operator-=(M<T, N...> & lhs, const M<T, N...> & rhs)
			-> M<T, N...> &
		{
			for (size_t i = 0; i < lhs.size(); i++)
			{
				lhs[i] = (lhs[i] - rhs[i]);
			}
			return lhs;
		}

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr auto operator*=(M<T, N...> & lhs, const M<T, N...> & rhs)
			-> M<T, N...> &
		{
			for (size_t i = 0; i < lhs.size(); i++)
			{
				lhs[i] = (lhs[i] * rhs[i]);
			}
			return lhs;
		}

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr auto operator/=(M<T, N...> & lhs, const M<T, N...> & rhs)
			-> M<T, N...> &
		{
			for (size_t i = 0; i < lhs.size(); i++)
			{
				lhs[i] = (lhs[i] / rhs[i]);
			}
			return lhs;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr auto operator+(const M<T, N...> & lhs, const M<T, N...> & rhs)
			-> M<T, N...>
		{
			M<T, N...> temp { lhs };
			temp += rhs;
			return temp;
		}

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr auto operator-(const M<T, N...> & lhs, const M<T, N...> & rhs)
			-> M<T, N...>
		{
			M<T, N...> temp { lhs };
			temp -= rhs;
			return temp;
		}

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr auto operator*(const M<T, N...> & lhs, const M<T, N...> & rhs)
			-> M<T, N...>
		{
			M<T, N...> temp { lhs };
			temp *= rhs;
			return temp;
		}

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr auto operator/(const M<T, N...> & lhs, const M<T, N...> & rhs)
			-> M<T, N...>
		{
			M<T, N...> temp { lhs };
			temp /= rhs;
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr auto operator+=(M<T, N...> & lhs, const T & rhs)
			-> M<T, N...> &
		{
			for (auto & elem : lhs)
			{
				elem += rhs;
			}
			return lhs;
		}

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr auto operator-=(M<T, N...> & lhs, const T & rhs)
			-> M<T, N...> &
		{
			for (auto & elem : lhs)
			{
				elem -= rhs;
			}
			return lhs;
		}

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr auto operator*=(M<T, N...> & lhs, const T & rhs)
			-> M<T, N...> &
		{
			for (auto & elem : lhs)
			{
				elem *= rhs;
			}
			return lhs;
		}

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr auto operator/=(M<T, N...> & lhs, const T & rhs)
			-> M<T, N...> &
		{
			for (auto & elem : lhs)
			{
				elem /= rhs;
			}
			return lhs;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr auto operator+(const M<T, N...> & lhs, const T & rhs)
			-> M<T, N...>
		{
			M<T, N...> temp { lhs };
			temp += rhs;
			return temp;
		}

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr auto operator-(const M<T, N...> & lhs, const T & rhs)
			-> M<T, N...>
		{
			M<T, N...> temp { lhs };
			temp -= rhs;
			return temp;
		}

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr auto operator*(const M<T, N...> & lhs, const T & rhs)
			-> M<T, N...>
		{
			M<T, N...> temp { lhs };
			temp *= rhs;
			return temp;
		}

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr auto operator/(const M<T, N...> & lhs, const T & rhs)
			-> M<T, N...>
		{
			M<T, N...> temp { lhs };
			temp /= rhs;
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr auto operator-(const M<T, N...> & lhs)
			-> M<T, N...>
		{
			return (lhs * type_t<T>::minus_one);
		}

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr auto operator+(const M<T, N...> & lhs)
			-> M<T, N...>
		{
			return -(-(lhs));
		}

		/* * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace meta
	{
		/* * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t, size_t> class M, class T
		> constexpr auto operator*=(M<T, 3, 3> & lhs, const M<T, 3, 3> & rhs)
			-> M<T, 3, 3> &
		{
			for (size_t y = 0; y < 3; y++)
			{
				for (size_t x = 0; x < 3; x++)
				{
					for (size_t r = 0; r < 3; r++)
					{
						lhs[y * 3 + x] += lhs[y * 3 + x] * rhs[r * 3 + x];
					}
				}
			}
			return lhs;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t ...> class M, class T
		> constexpr auto operator*(const M<T, 3, 3> & lhs, const M<T, 3, 3> & rhs)
			-> M<T, 3, 3>
		{
			M<T, 3, 3> temp { lhs };
			temp *= rhs;
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}