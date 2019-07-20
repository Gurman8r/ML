#ifndef _ML_ARRAY_HPP_
#define _ML_ARRAY_HPP_

#include <ML/Core/Alg.hpp>
#include <ML/Core/Hash.hpp>

#define ML_NO_NEW_ARRAYS // prevent new-ing arrays because they're destructorless

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class T, size_t N
	> struct Array
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static_assert(0 < N, "Array : size negative or zero");

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { Size = N };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename T;
		using data_type			= typename value_type[Size];
		using self_type			= typename Array<value_type, Size>;
		using pointer			= typename value_type *;
		using reference			= typename value_type &;
		using const_pointer		= typename const value_type *;
		using const_reference	= typename const value_type &;
		using iterator			= typename pointer;
		using const_iterator	= typename const_pointer;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		data_type m_data; // aggregate initializer

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr auto begin()			-> iterator			{ return data(); }
		constexpr auto begin()	const	-> const_iterator	{ return data(); }
		constexpr auto cbegin() const	-> const_iterator	{ return begin(); }
		constexpr auto cend()	const	-> const_iterator	{ return end(); }
		constexpr auto data()			-> pointer			{ return m_data; }
		constexpr auto data()	const	-> const_pointer	{ return m_data; }
		constexpr auto end()			-> iterator			{ return data() + size(); }
		constexpr auto end()	const	-> const_iterator	{ return data() + size(); }
		constexpr auto hash()	const	-> hash_t			{ return Hash()(size(), data()); }
		constexpr auto size()	const	-> size_t			{ return Size; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr const_reference operator[](size_t i) const { return m_data[i]; }

		constexpr reference operator[](size_t i) { return m_data[i]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr auto fill(const_reference value)
		{
			self_type temp { uninit };
			alg::fill(temp, value);
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef ML_NO_NEW_ARRAYS
		private:
			inline void * operator	new		 (size_t size) { return nullptr; }
			inline void * operator	new[]	 (size_t size) { return nullptr; }
			inline void	  operator	delete	 (void * ptr)  { return;  }
			inline void	  operator	delete[] (void * ptr)  { return;  }
#endif // ML_NO_NEW_ARRAYS

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class T, size_t N
	> inline ML_SERIALIZE(Ostream & out, const Array<T, N> & value)
	{
		for (const auto & elem : value)
		{
			out << elem << ' ';
		}
		return out;
	}

	template <
		class T, size_t N
	> inline ML_DESERIALIZE(Istream & in, Array<T, N> & value)
	{
		for (auto & elem : value)
		{
			in >> elem;
		}
		return in;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class T, size_t N
	> constexpr bool operator==(const Array<T, N> & lhs, const Array<T, N> & rhs)
	{
		return alg::equals(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <
		class T, size_t N
	> constexpr bool operator!=(const Array<T, N> & lhs, const Array<T, N> & rhs)
	{
		return !(lhs == rhs);
	}

	template <
		class T, size_t N
	> constexpr bool operator<(const Array<T, N> & lhs, const Array<T, N> & rhs)
	{
		return alg::less(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <
		class T, size_t N
	> constexpr bool operator<=(const Array<T, N> & lhs, const Array<T, N> & rhs)
	{
		return (lhs < rhs) || (lhs == rhs);
	}

	template <
		class T, size_t N
	> constexpr bool operator>(const Array<T, N> & lhs, const Array<T, N> & rhs)
	{
		return !(lhs < rhs) && (lhs != rhs);
	}

	template <
		class T, size_t N
	> constexpr bool operator>=(const Array<T, N> & lhs, const Array<T, N> & rhs)
	{
		return (lhs > rhs) || (lhs == rhs);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ARRAY_HPP_