#ifndef _ML_ARRAY_HPP_
#define _ML_ARRAY_HPP_

#include <ML/Core/Alg.hpp>
#include <ML/Core/Hash.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// ARRAY_T
	namespace meta
	{
		/* * * * * * * * * * * * * * * * * * * * */

		template <
			class T, size_t N
		> struct array_t
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			static_assert(0 < N, "array_t : size negative or zero");

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			enum : size_t { Size = N };

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			using value_type		= typename T;
			using type				= typename type_t<value_type>;
			using data_type			= typename value_type[Size];
			using self_type			= typename array_t<value_type, Size>;
			using pointer			= typename value_type *;
			using reference			= typename value_type &;
			using const_pointer		= typename const value_type *;
			using const_reference	= typename const value_type &;

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			data_type m_data; // aggregate initializer

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			constexpr auto begin()			-> pointer			{ return data(); }
			constexpr auto begin()	const	-> const_pointer	{ return data(); }
			constexpr auto cbegin() const	-> const_pointer	{ return begin(); }
			constexpr auto cend()	const	-> const_pointer	{ return end(); }
			constexpr auto data()			-> pointer			{ return m_data; }
			constexpr auto data()	const	-> const_pointer	{ return m_data; }
			constexpr auto end()			-> pointer			{ return data() + size(); }
			constexpr auto end()	const	-> const_pointer	{ return data() + size(); }
			constexpr auto hash()	const	-> size_t			{ return ml::hash()(size(), data()); }
			constexpr auto size()	const	-> size_t			{ return Size; }

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			constexpr const_reference operator[](size_t i) const 
			{ 
				return m_data[i]; 
			}

			constexpr reference operator[](size_t i)
			{ 
				return m_data[i]; 
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			static constexpr auto fill(const_reference value)
			{
				self_type temp { uninit };
				alg::fill(temp, value);
				return temp;
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		};
		
		/* * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t ...> class Arr, class T, size_t ... N
		> inline ML_SERIALIZE(ostream & out, const Arr<T, N...> & value)
		{
			for (const auto & elem : value)
			{
				out << elem << ' ';
			}
			return out;
		}

		template <
			template <class, size_t ...> class Arr, class T, size_t ... N
		> inline ML_DESERIALIZE(istream & in, Arr<T, N...> & value)
		{
			for (auto & elem : value)
			{
				in >> elem;
			}
			return in;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t ...> class Arr, class T, size_t ... N
		> constexpr bool operator==(const Arr<T, N...> & lhs, const Arr<T, N...> & rhs)
		{
			return alg::equals(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
		}

		template <
			template <class, size_t ...> class Arr, class T, size_t ... N
		> constexpr bool operator!=(const Arr<T, N...> & lhs, const Arr<T, N...> & rhs)
		{
			return !(lhs == rhs);
		}

		template <
			template <class, size_t ...> class Arr, class T, size_t ... N
		> constexpr bool operator<(const Arr<T, N...> & lhs, const Arr<T, N...> & rhs)
		{
			return alg::less(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
		}

		template <
			template <class, size_t ...> class Arr, class T, size_t ... N
		> constexpr bool operator<=(const Arr<T, N...> & lhs, const Arr<T, N...> & rhs)
		{
			return (lhs < rhs) || (lhs == rhs);
		}

		template <
			template <class, size_t ...> class Arr, class T, size_t ... N
		> constexpr bool operator>(const Arr<T, N...> & lhs, const Arr<T, N...> & rhs)
		{
			return !(lhs < rhs) && (lhs != rhs);
		}

		template <
			template <class, size_t ...> class Arr, class T, size_t ... N
		> constexpr bool operator>=(const Arr<T, N...> & lhs, const Arr<T, N...> & rhs)
		{
			return (lhs > rhs) || (lhs == rhs);
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t ...> class Arr, class T, size_t ... N
		> constexpr auto operator+=(Arr<T, N...> & lhs, const Arr<T, N...> & rhs)
			-> Arr<T, N...> &
		{
			for (size_t i = 0; i < lhs.size(); i++)
			{
				lhs[i] = (lhs[i] + rhs[i]);
			}
			return lhs;
		}

		template <
			template <class, size_t ...> class Arr, class T, size_t ... N
		> constexpr auto operator-=(Arr<T, N...> & lhs, const Arr<T, N...> & rhs)
			-> Arr<T, N...> &
		{
			for (size_t i = 0; i < lhs.size(); i++)
			{
				lhs[i] = (lhs[i] - rhs[i]);
			}
			return lhs;
		}

		template <
			template <class, size_t ...> class Arr, class T, size_t ... N
		> constexpr auto operator*=(Arr<T, N...> & lhs, const Arr<T, N...> & rhs)
			-> Arr<T, N...> &
		{
			for (size_t i = 0; i < lhs.size(); i++)
			{
				lhs[i] = (lhs[i] * rhs[i]);
			}
			return lhs;
		}

		template <
			template <class, size_t ...> class Arr, class T, size_t ... N
		> constexpr auto operator/=(Arr<T, N...> & lhs, const Arr<T, N...> & rhs)
			-> Arr<T, N...> &
		{
			for (size_t i = 0; i < lhs.size(); i++)
			{
				lhs[i] = (lhs[i] / rhs[i]);
			}
			return lhs;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t ...> class Arr, class T, size_t ... N
		> constexpr auto operator+(const Arr<T, N...> & lhs, const Arr<T, N...> & rhs)
			-> Arr<T, N...>
		{
			Arr<T, N...> temp { lhs };
			temp += rhs;
			return temp;
		}

		template <
			template <class, size_t ...> class Arr, class T, size_t ... N
		> constexpr auto operator-(const Arr<T, N...> & lhs, const Arr<T, N...> & rhs)
			-> Arr<T, N...>
		{
			Arr<T, N...> temp { lhs };
			temp -= rhs;
			return temp;
		}

		template <
			template <class, size_t ...> class Arr, class T, size_t ... N
		> constexpr auto operator*(const Arr<T, N...> & lhs, const Arr<T, N...> & rhs)
			-> Arr<T, N...>
		{
			Arr<T, N...> temp { lhs };
			temp *= rhs;
			return temp;
		}

		template <
			template <class, size_t ...> class Arr, class T, size_t ... N
		> constexpr auto operator/(const Arr<T, N...> & lhs, const Arr<T, N...> & rhs)
			-> Arr<T, N...>
		{
			Arr<T, N...> temp { lhs };
			temp /= rhs;
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t ...> class Arr, class T, size_t ... N
		> constexpr auto operator+=(Arr<T, N...> & lhs, const T & rhs)
			-> Arr<T, N...> &
		{
			for (auto & elem : lhs)
			{
				elem += rhs;
			}
			return lhs;
		}

		template <
			template <class, size_t ...> class Arr, class T, size_t ... N
		> constexpr auto operator-=(Arr<T, N...> & lhs, const T & rhs)
			-> Arr<T, N...> &
		{
			for (auto & elem : lhs)
			{
				elem -= rhs;
			}
			return lhs;
		}

		template <
			template <class, size_t ...> class Arr, class T, size_t ... N
		> constexpr auto operator*=(Arr<T, N...> & lhs, const T & rhs)
			-> Arr<T, N...> &
		{
			for (auto & elem : lhs)
			{
				elem *= rhs;
			}
			return lhs;
		}

		template <
			template <class, size_t ...> class Arr, class T, size_t ... N
		> constexpr auto operator/=(Arr<T, N...> & lhs, const T & rhs)
			-> Arr<T, N...> &
		{
			for (auto & elem : lhs)
			{
				elem /= rhs;
			}
			return lhs;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t ...> class Arr, class T, size_t ... N
		> constexpr auto operator+(const Arr<T, N...> & lhs, const T & rhs)
			-> Arr<T, N...>
		{
			Arr<T, N...> temp { lhs };
			temp += rhs;
			return temp;
		}

		template <
			template <class, size_t ...> class Arr, class T, size_t ... N
		> constexpr auto operator-(const Arr<T, N...> & lhs, const T & rhs)
			-> Arr<T, N...>
		{
			Arr<T, N...> temp { lhs };
			temp -= rhs;
			return temp;
		}

		template <
			template <class, size_t ...> class Arr, class T, size_t ... N
		> constexpr auto operator*(const Arr<T, N...> & lhs, const T & rhs)
			-> Arr<T, N...>
		{
			Arr<T, N...> temp { lhs };
			temp *= rhs;
			return temp;
		}

		template <
			template <class, size_t ...> class Arr, class T, size_t ... N
		> constexpr auto operator/(const Arr<T, N...> & lhs, const T & rhs)
			-> Arr<T, N...>
		{
			Arr<T, N...> temp { lhs };
			temp /= rhs;
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t ...> class Arr, class T, size_t ... N
		> constexpr auto operator-(const Arr<T, N...> & lhs)
			-> Arr<T, N...>
		{
			return (lhs * type_t<T>::minus_one);
		}

		template <
			template <class, size_t ...> class Arr, class T, size_t ... N
		> constexpr auto operator+(const Arr<T, N...> & lhs)
			-> Arr<T, N...>
		{
			return -(-(lhs));
		}

		/* * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ARRAY_HPP_