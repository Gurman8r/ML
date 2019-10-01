#ifndef _ML_ARRAY_HPP_
#define _ML_ARRAY_HPP_

#include <ML/Core/Alg.hpp>
#include <ML/Core/Hash.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <class T, size_t N> struct Array
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static_assert(0 < N, "Array : size negative or zero");

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { Size = N };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename T;
		using base_type			= typename value_type[Size];
		using self_type			= typename Array<value_type, Size>;
		using pointer			= typename value_type *;
		using reference			= typename value_type &;
		using const_pointer		= typename const value_type *;
		using const_reference	= typename const value_type &;
		using iterator			= typename pointer;
		using const_iterator	= typename const_pointer;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		base_type m_data; // aggregate initializer

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr auto at(size_t i)			-> reference		{ return m_data[i]; }
		constexpr auto at(size_t i) const	-> const_reference	{ return m_data[i]; }
		constexpr auto back()				-> reference		{ return (*end()); }
		constexpr auto back()		const	-> const_reference	{ return (*cend()); }
		constexpr auto begin()				-> iterator			{ return data(); }
		constexpr auto begin()		const	-> const_iterator	{ return data(); }
		constexpr auto cbegin()		const	-> const_iterator	{ return begin(); }
		constexpr auto cend()		const	-> const_iterator	{ return end(); }
		constexpr auto data()				-> pointer			{ return m_data; }
		constexpr auto data()		const	-> const_pointer	{ return m_data; }
		constexpr auto end()				-> iterator			{ return data() + size(); }
		constexpr auto end()		const	-> const_iterator	{ return data() + size(); }
		constexpr auto front()				-> reference		{ return (*begin()); }
		constexpr auto front()		const	-> const_reference	{ return (*cbegin()); }
		constexpr auto hash()		const	-> hash_t			{ return Hash { data(), size() }; }
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr size_t size() { return self_type::Size; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr reference operator[](size_t i) { return at(i); }
		
		constexpr const_reference operator[](size_t i) const { return at(i); }

		template <class U> constexpr operator std::array<U, Size>() const
		{
			std::array<U, Size> temp { uninit };
			for (size_t i = 0; i < temp.size(); i++)
			{
				temp[i] = static_cast<U>(m_data[i]);
			}
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class T, size_t N
	> inline ML_SERIALIZE(std::ostream & out, const Array<T, N> & value)
	{
		for (const auto & elem : value)
		{
			out << elem << " ";
		}
		return out;
	}

	template <
		class T, size_t N
	> inline ML_DESERIALIZE(std::istream & in, Array<T, N> & value)
	{
		for (auto & elem : value)
		{
			if (in.good()) in >> elem;
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

	namespace alg
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T = typename uint32_t
		> static constexpr Array<uint8_t, sizeof(T)> to_bytes(const T value)
		{
			Array<uint8_t, sizeof(T)> temp {};
			for (T i = 0; i < sizeof(T); i++)
			{
				temp[i] = static_cast<uint8_t>(value >> (i * constant_t<T>::eight));
			}
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

template <class T, _STD size_t N> struct _STD hash<_ML Array<T, N>>
{
	inline _STD size_t operator()(const _ML Array<T, N> & value) const noexcept
	{
		return static_cast<_STD size_t>(value.hash());
	}
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_ARRAY_HPP_