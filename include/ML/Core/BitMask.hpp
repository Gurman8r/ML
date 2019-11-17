#ifndef _ML_BITMASK_HPP_
#define _ML_BITMASK_HPP_

#include <ML/Core/Array.hpp>

#define ML_BITREAD(value, bit)		((value >> bit) & 1)
#define ML_BITSET(value, bit)		(value |= (1 << bit))
#define ML_BITCLEAR(value, bit)		(value &= ~(1 << bit))
#define ML_BITWRITE(value, bit, b)	(b ? ML_BITSET(value, bit) : ML_BITCLEAR(value, bit))

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <class T> struct BitMask final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { Size = sizeof(T) * 8 };

		using value_type		= typename detail::decay_t<T>;
		using self_type			= typename BitMask<value_type>;
		using array_type		= typename Array<bool, Size>;
		using pointer			= typename value_type *;
		using reference			= typename value_type &;
		using const_pointer		= typename const value_type *;
		using const_reference	= typename const value_type &;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr BitMask() noexcept
			: m_value { 0 }
		{
		}

		constexpr BitMask(const value_type value) noexcept
			: m_value { value }
		{
		}

		constexpr BitMask(const array_type & value) noexcept
			: m_value { from_bits<array_type, T, value.size()>(value) }
		{
		}

		template <
			class U, size_t N
		> constexpr BitMask(const U(&value)[N]) noexcept
			: m_value { from_bits<const U(&)[N], U, N>(value) }
		{
		}

		constexpr BitMask(const self_type & copy) noexcept
			: m_value { copy.m_value }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr auto bits() const -> const array_type &	{ return to_bits(*this); }
		constexpr auto size() const							{ return Size; }
		inline	  auto data()		-> reference			{ return m_value; }
		constexpr auto data() const -> const_reference		{ return m_value; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr operator const_reference() const { return m_value; }

		constexpr bool operator[](size_t i) const { return this->read(i); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr const self_type & read(size_t i) const
		{
			ML_BITREAD(m_value, i);
			return (*this);
		}

		inline self_type & clear(size_t i)
		{
			ML_BITCLEAR(m_value, i);
			return (*this);
		}

		inline self_type & set(size_t i)
		{
			ML_BITSET(m_value, i);
			return (*this);
		}

		inline self_type & write(size_t i, bool value)
		{
			ML_BITWRITE(m_value, i, value);
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

	using BitMask_8		= typename BitMask<uint8_t>;
	using BitMask_16	= typename BitMask<uint16_t>;
	using BitMask_32	= typename BitMask<uint32_t>;
	using BitMask_64	= typename BitMask<uint64_t>;

	/* * * * * * * * * * * * * * * * * * * * */

	template <class T>
	inline ML_SERIALIZE(std::ostream & out, const BitMask<T> & value)
	{
		return out << value.bits();
	}

	template <class T>
	inline ML_DESERIALIZE(std::istream & in, BitMask<T> & value)
	{
		return in.good() ? (in >> value.data()) : in;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_BITSET_HPP_