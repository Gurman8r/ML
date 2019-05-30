#include <ML/Core/Meta.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace meta
	{
		// VALUE_T OPERATORS
		/* * * * * * * * * * * * * * * * * * * * */

		template <
			template <class> class Val, class T
		> constexpr bool operator==(const Val<T> & lhs, const Val<T> & rhs)
		{
			return (lhs() == rhs());
		}

		template <
			template <class> class Val, class T
		> constexpr bool operator!=(const Val<T> & lhs, const Val<T> & rhs)
		{
			return !(lhs == rhs);
		}

		template <
			template <class> class Val, class T
		> constexpr bool operator<(const Val<T> & lhs, const Val<T> & rhs)
		{
			return (lhs() < rhs());
		}

		template <
			template <class> class Val, class T
		> constexpr bool operator>(const Val<T> & lhs, const Val<T> & rhs)
		{
			return !(lhs < rhs);
		}

		template <
			template <class> class Val, class T
		> constexpr bool operator<=(const Val<T> & lhs, const Val<T> & rhs)
		{
			return (lhs < rhs) || (lhs == rhs);
		}

		template <
			template <class> class Val, class T
		> constexpr bool operator>=(const Val<T> & lhs, const Val<T> & rhs)
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
			return (lhs * value_t<T>::minus_one);
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
}