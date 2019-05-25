#ifndef _ML_BYTES_HPP_
#define _ML_BYTES_HPP_

#include <ML/Core/IObject.hpp>
#include <ML/Core/Ratio.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	constexpr uint64_t operator "" _kB(uint64_t value) { return ratio_cast(value, Kilo()); }
	constexpr uint64_t operator "" _MB(uint64_t value) { return ratio_cast(value, Mega()); }
	constexpr uint64_t operator "" _GB(uint64_t value) { return ratio_cast(value, Giga()); }
	constexpr uint64_t operator "" _TB(uint64_t value) { return ratio_cast(value, Tera()); }
	constexpr uint64_t operator "" _PB(uint64_t value) { return ratio_cast(value, Peta()); }
	constexpr uint64_t operator "" _EB(uint64_t value) { return ratio_cast(value, Exa());  }

	/* * * * * * * * * * * * * * * * * * * * */

	class Bytes final
		: public IObject
		, public IComparable<uint64_t>
	{
	private:
		/* * * * * * * * * * * * * * * * * * * * */
		uint64_t	m_real;	// Size before ratio
		uint64_t	m_size;	// Size after ratio
		CString		m_name;	// Unit of display size

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		Bytes(const Bytes & copy)
			: m_size(copy.m_size)
			, m_name(copy.m_name)
			, m_real(copy.m_real)
		{
		}

		template <
			intmax_t N, // Numerator
			intmax_t D	// Denominator
		>
		Bytes(const uint64_t value, const Ratio<N, D> & r)
			: Bytes(ratio_cast(value, r))
		{
		}

		Bytes(const uint64_t value)
			: m_real(value)
		{
			if (value == 0) // 
			{
				m_size = 0;
				m_name = "-";
			}
			else if (value < Kilo::num) // Bytes
			{
				m_size = ratio_cast(value, Ratio<1, 1>());
				m_name = "B";
			}
			else if (value < Mega::num) // Kilobytes
			{
				m_size = ratio_cast(value, Milli());
				m_name = "kB";
			}
			else if (value < Giga::num) // Megabytes
			{
				m_size = ratio_cast(value, Micro());
				m_name = "MB";
			}
			else if (value < Tera::num) // Gigabytes
			{
				m_size = ratio_cast(value, Nano());
				m_name = "GB";
			}
			else if (value < Peta::num) // Terabytes
			{
				m_size = ratio_cast(value, Pico());
				m_name = "TB";
			}
			else if (value < Exa::num) // Petabytes
			{
				m_size = ratio_cast(value, Femto());
				m_name = "TB";
			}
			else // Exabytes
			{
				m_size = ratio_cast(value, Atto());
				m_name = "EB";
			}
		}

		~Bytes() {}

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		inline operator uint64_t() const
		{
			return m_real;
		}

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		inline uint64_t real() const { return m_real; }
		inline uint64_t size() const { return m_size; }
		inline CString	name() const { return m_name; }


	public:
		/* * * * * * * * * * * * * * * * * * * * */
		inline String str() const
		{
			String temp;
			if (m_size)
			{
				temp.append(std::to_string(m_size));
			}
			temp.append(m_name);
			return temp;
		}
		
		inline bool equals(const uint64_t & other) const override
		{
			return (m_real == other);
		}

		inline bool lessThan(const uint64_t & other) const override
		{
			return (m_real < other);
		}

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_BYTES_HPP_