#ifndef _ML_SIZEOF_HPP_
#define _ML_SIZEOF_HPP_

#include <ML/Core/Ratio.hpp>
#include <ML/Core/String.hpp>
#include <ML/Core/I_NonNewable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct Bytes final : public I_NonNewable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			intmax_t Num,	// Numerator
			intmax_t Den	// Denominator
		> constexpr Bytes(uint64_t value, const Ratio<Num, Den> & r)
			: Bytes(ratio_cast(value, r))
		{
		}

		constexpr Bytes(const Bytes & copy)
			: m_size(copy.m_size)
			, m_name(copy.m_name)
			, m_real(copy.m_real)
		{
		}

		constexpr Bytes(const uint64_t value)
			: m_real(value)
			, m_size(((value == 0)
				? 0
				: ((value < Kilo::num)
					? ratio_cast(value, Ratio<1, 1>())
					: ((value < Mega::num)
						? ratio_cast(value, Milli())
						: ((value < Giga::num)
							? ratio_cast(value, Micro())
							: ((value < Tera::num)
								? ratio_cast(value, Nano())
								: ((value < Peta::num)
									? ratio_cast(value, Pico())
									: ((value < Exa::num)
										? ratio_cast(value, Femto())
										: ratio_cast(value, Atto())))))))))
			, m_name(((value == 0)
				? "-"
				: ((value < Kilo::num)
					? "B"
					: ((value < Mega::num)
						? "kB"
						: ((value < Giga::num)
							? "MB"
							: ((value < Tera::num)
								? "GB"
								: ((value < Peta::num)
									? "TB"
									: ((value < Exa::num)
										? "PB"
										: "EB"))))))))
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr operator	uint64_t()	const { return m_real; }
		constexpr uint64_t	real()		const { return m_real; }
		constexpr uint64_t	size()		const { return m_size; }
		constexpr C_String	name()		const { return m_name; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		uint64_t	m_real;	// Size before ratio
		uint64_t	m_size;	// Size after ratio
		C_String	m_name;	// Unit of display size
	};

	/* * * * * * * * * * * * * * * * * * * * */

	constexpr uint64_t operator "" _kB(uint64_t value) { return ratio_cast(value, Kilo()); }
	constexpr uint64_t operator "" _MB(uint64_t value) { return ratio_cast(value, Mega()); }
	constexpr uint64_t operator "" _GB(uint64_t value) { return ratio_cast(value, Giga()); }
	constexpr uint64_t operator "" _TB(uint64_t value) { return ratio_cast(value, Tera()); }
	constexpr uint64_t operator "" _PB(uint64_t value) { return ratio_cast(value, Peta()); }
	constexpr uint64_t operator "" _EB(uint64_t value) { return ratio_cast(value, Exa()); }

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SIZEOF_HPP_