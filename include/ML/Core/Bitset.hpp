#ifndef _ML_BITSET_HPP_
#define _ML_BITSET_HPP_

#include <ML/Core/Array.hpp>

#define ML_BITREAD(value, bit)		((value >> bit) & 1)
#define ML_BITSET(value, bit)		(value |= (1 << bit))
#define ML_BITCLEAR(value, bit)		(value &= ~(1 << bit))
#define ML_BITWRITE(value, bit, b)	(b ? ML_BITSET(value, bit) : ML_BITCLEAR(value, bit))

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <class T> struct bitmask final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { Size = sizeof(T) * 8 };

		using value_type		= typename detail::decay_t<T>;
		using self_type			= typename bitmask<value_type>;
		using array_type		= typename Array<bool, Size>;
		using pointer			= typename value_type *;
		using reference			= typename value_type &;
		using const_pointer		= typename const value_type *;
		using const_reference	= typename const value_type &;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr bitmask() noexcept
			: m_value { 0 }
		{
		}

		constexpr bitmask(const value_type value) noexcept
			: m_value { value }
		{
		}

		constexpr bitmask(const array_type & value) noexcept
			: m_value { from_bits<array_type, T, value.size()>(value) }
		{
		}

		template <
			class U, size_t N
		> constexpr bitmask(const U(&value)[N]) noexcept
			: m_value { from_bits<const U(&)[N], U, N>(value) }
		{
		}

		constexpr bitmask(const self_type & copy) noexcept
			: m_value { copy.m_value }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr auto bits() const -> const array_type &	{ return to_bits(*this); }
		constexpr auto size() const							{ return Size; }
		inline	  auto data()		-> reference			{ return m_value; }
		constexpr auto data() const -> const_reference		{ return m_value; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr bool operator[](size_t i) const { return ML_BITREAD(m_value, i); }

		constexpr operator const_reference() const { return m_value; }

		template <
			class In, class U, size_t N
		> static constexpr value_type from_bits(const In & value)
		{
			value_type temp { 0 };
			for (size_t i = 0; i < N; i++)
			{
				ML_BITWRITE(temp, i, value[i]);
			}
			return temp;
		}

		static constexpr array_type to_bits(const self_type & value)
		{
			array_type temp { 0 };
			for (size_t i = 0; i < Size; i++)
			{
				temp[i] = ML_BITREAD(value.m_value, i);
			}
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: value_type m_value;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	using bitmask_8		= typename bitmask<uint8_t>;
	using bitmask_16	= typename bitmask<uint16_t>;
	using bitmask_32	= typename bitmask<uint32_t>;
	using bitmask_64	= typename bitmask<uint64_t>;

	/* * * * * * * * * * * * * * * * * * * * */

	template <class T>
	inline ML_SERIALIZE(std::ostream & out, const bitmask<T> & value)
	{
		return out << value.bits();
	}

	template <class T>
	inline ML_DESERIALIZE(std::istream & in, bitmask<T> & value)
	{
		return in.good() ? (in >> value.data()) : in;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_BITSET_HPP_