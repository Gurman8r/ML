#ifndef _ML_BYTE_ARRAY_HPP_
#define _ML_BYTE_ARRAY_HPP_

#include <ML/Core/Array.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <size_t N> struct ByteArray
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static_assert(0 < N, "size negative or zero");

		enum : size_t { Size = N };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename byte_t;
		using base_type			= typename Array<value_type, Size>;
		using pointer			= typename base_type::pointer;
		using reference			= typename base_type::reference;
		using const_pointer		= typename base_type::const_pointer;
		using const_reference	= typename base_type::const_reference;
		using iterator			= typename base_type::iterator;
		using const_iterator	= typename base_type::const_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		base_type m_data; // aggregate initializer

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr size_t size() { return m_data.size(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr auto at(size_t i)			-> reference		{ return m_data.at(i); }
		constexpr auto at(size_t i) const	-> const_reference	{ return m_data.at(i); }
		constexpr auto back()				-> reference		{ return m_data.back(); }
		constexpr auto back()		const	-> const_reference	{ return m_data.back(); }
		constexpr auto begin()				-> iterator			{ return m_data.begin(); }
		constexpr auto begin()		const	-> const_iterator	{ return m_data.begin(); }
		constexpr auto cbegin()		const	-> const_iterator	{ return m_data.cbegin(); }
		constexpr auto cend()		const	-> const_iterator	{ return m_data.cend(); }
		constexpr auto data()				-> pointer			{ return m_data.data(); }
		constexpr auto data()		const	-> const_pointer	{ return m_data.data(); }
		constexpr auto end()				-> iterator			{ return m_data.end(); }
		constexpr auto end()		const	-> const_iterator	{ return m_data.end(); }
		constexpr auto front()				-> reference		{ return m_data.front(); }
		constexpr auto front()		const	-> const_reference	{ return m_data.front(); }
		constexpr auto hash()		const	-> hash_t			{ return m_data.hash(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr reference operator[](size_t i)
		{
			return m_data[i];
		}

		constexpr const_reference operator[](size_t i) const
		{
			return m_data[i];
		}

		constexpr operator base_type & ()
		{
			return m_data;
		}

		constexpr operator base_type const & () const
		{
			return m_data;
		}

		template <class U> constexpr operator std::array<U, Size>() const
		{
			return (std::array<U, Size>)(base_type)(*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_BYTE_ARRAY_HPP_