#ifndef _ML_SIZE_OF_HPP_
#define _ML_SIZE_OF_HPP_

#include <ML/Core/Ratio.hpp>
#include <ML/Core/String.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct size_of final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			intmax_t Num,	// Numerator
			intmax_t Den	// Denominator
		> constexpr size_of(uint64_t value, const Ratio<Num, Den> & r)
			: size_of(alg::ratio_cast(value, r))
		{
		}

		constexpr size_of(size_of const & copy)
			: m_size(copy.m_size)
			, m_name(copy.m_name)
			, m_real(copy.m_real)
		{
		}

		constexpr size_of(const uint64_t value)
			: m_real(value)
			, m_size(((value == 0)
				? 0
				: ((value < Kilo::num)
					? alg::ratio_cast(value, Ratio<1, 1>())
					: ((value < Mega::num)
						? alg::ratio_cast(value, Milli())
						: ((value < Giga::num)
							? alg::ratio_cast(value, Micro())
							: ((value < Tera::num)
								? alg::ratio_cast(value, Nano())
								: ((value < Peta::num)
									? alg::ratio_cast(value, Pico())
									: ((value < Exa::num)
										? alg::ratio_cast(value, Femto())
										: alg::ratio_cast(value, Atto())))))))))
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

		inline String str() const
		{
			return m_size ? ((String)std::to_string(m_size) + m_name) : m_name;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		uint64_t	m_real;	// Size before ratio
		uint64_t	m_size;	// Size after ratio
		C_String	m_name;	// Unit of display size
	};

	/* * * * * * * * * * * * * * * * * * * * */

	inline ML_SERIALIZE(std::ostream & out, size_of const & value)
	{
		return out << value.str();
	}

	constexpr uint64_t operator "" _KB(uint64_t value) { return alg::ratio_cast(value, Kilo()); }
	constexpr uint64_t operator "" _MB(uint64_t value) { return alg::ratio_cast(value, Mega()); }
	constexpr uint64_t operator "" _GB(uint64_t value) { return alg::ratio_cast(value, Giga()); }
	constexpr uint64_t operator "" _TB(uint64_t value) { return alg::ratio_cast(value, Tera()); }
	constexpr uint64_t operator "" _PB(uint64_t value) { return alg::ratio_cast(value, Peta()); }
	constexpr uint64_t operator "" _EB(uint64_t value) { return alg::ratio_cast(value, Exa()); }

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SIZE_OF_HPP_