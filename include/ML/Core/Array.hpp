#ifndef _ML_ARRAY_HPP_
#define _ML_ARRAY_HPP_

#include <ML/Core/Alg.hpp>
#include <ML/Core/Hash.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// fixed size array
	template <class T, size_t N> struct Array
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { Size = N };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename T;
		using base_type			= typename value_type[Size];
		using self_type			= typename Array<value_type, Size>;
		using pointer			= typename value_type *;
		using reference			= typename value_type &;
		using const_pointer		= typename value_type const *;
		using const_reference	= typename value_type const &;
		using iterator			= typename pointer;
		using const_iterator	= typename const_pointer;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		base_type m_data; // aggregate initializer

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr auto at(size_t i)			-> reference		{ return *(m_data + i); }
		constexpr auto at(size_t i) const	-> const_reference	{ return *(m_data + i); }
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
		constexpr auto hash()		const	-> hash_t			{ return Hash(data(), size()); }
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr size_t size() { return self_type::Size; }

		constexpr reference operator[](size_t i) { return at(i); }
		
		constexpr const_reference operator[](size_t i) const { return at(i); }

		template <class U> constexpr operator std::array<U, Size>() const
		{
			std::array<U, Size> temp { 0 };
			for (size_t i = 0; i < temp.size(); i++)
			{
				temp[i] = static_cast<U>(m_data[i]);
			}
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	// zero size array
	template <class T> struct Array<T, 0>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		enum : size_t { Size = 0 };
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		using value_type		= typename T;
		using base_type			= typename value_type[1];
		using self_type			= typename Array<value_type, 0>;
		using pointer			= typename value_type *;
		using reference			= typename value_type &;
		using const_pointer		= typename value_type const *;
		using const_reference	= typename value_type const &;
		using iterator			= typename pointer;
		using const_iterator	= typename const_pointer;
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		base_type m_data;
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		constexpr auto at(size_t)			-> reference		{ return m_data[0]; }
		constexpr auto at(size_t)	const	-> const_reference	{ return m_data[0]; }
		constexpr auto back()				-> reference		{ return m_data[0]; }
		constexpr auto back()		const	-> const_reference	{ return m_data[0]; }
		constexpr auto begin()				-> iterator			{ return &m_data[0]; }
		constexpr auto begin()		const	-> const_iterator	{ return &m_data[0]; }
		constexpr auto cbegin()		const	-> const_iterator	{ return &m_data[0]; }
		constexpr auto cend()		const	-> const_iterator	{ return &m_data[0]; }
		constexpr auto data()				-> pointer			{ return &m_data[0]; }
		constexpr auto data()		const	-> const_pointer	{ return &m_data[0]; }
		constexpr auto end()				-> iterator			{ return &m_data[0]; }
		constexpr auto end()		const	-> const_iterator	{ return &m_data[0]; }
		constexpr auto front()				-> reference		{ return m_data[0]; }
		constexpr auto front()		const	-> const_reference	{ return m_data[0]; }
		constexpr auto hash()		const	-> hash_t			{ return 0; }
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		static constexpr size_t size() { return 0; }
		constexpr reference operator[](size_t) { return m_data[0]; }
		constexpr const_reference operator[](size_t) const { m_data[0]; }
		template <class U> constexpr operator std::array<U, 0>() const { return {}; }
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class T, size_t N
	> inline ML_SERIALIZE(std::ostream & out, const Array<T, N> & value)
	{
		for (auto const & elem : value)
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
		class Tx, class Ty, size_t N
	> constexpr bool operator==(const Array<Tx, N> & lhs, const Array<Ty, N> & rhs)
	{
		return alg::equals(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <
		class Tx, class Ty, size_t N
	> constexpr bool operator!=(const Array<Tx, N> & lhs, const Array<Ty, N> & rhs)
	{
		return !(lhs == rhs);
	}

	template <
		class Tx, class Ty, size_t N
	> constexpr bool operator<(const Array<Tx, N> & lhs, const Array<Ty, N> & rhs)
	{
		return alg::less(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <
		class Tx, class Ty, size_t N
	> constexpr bool operator<=(const Array<Tx, N> & lhs, const Array<Ty, N> & rhs)
	{
		return (lhs < rhs) || (lhs == rhs);
	}

	template <
		class Tx, class Ty, size_t N
	> constexpr bool operator>(const Array<Tx, N> & lhs, const Array<Ty, N> & rhs)
	{
		return !(lhs < rhs) && (lhs != rhs);
	}

	template <
		class Tx, class Ty, size_t N
	> constexpr bool operator>=(const Array<Tx, N> & lhs, const Array<Ty, N> & rhs)
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
				temp[i] = static_cast<uint8_t>(value >> (i * cast<T>::eight));
			}
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_ARRAY_HPP_