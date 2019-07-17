#ifndef _ML_DURATION_HPP_
#define _ML_DURATION_HPP_

#include <ML/Core/Ratio.hpp>
#include <ML/Core/I_NonNewable.hpp>

#define ML_duration_cast(RATIO, VALUE) (_STD chrono::duration_cast<RATIO>(VALUE).count())

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	using Nanoseconds	= typename std::chrono::duration<uint64_t,  Nano>;
	using Microseconds	= typename std::chrono::duration<uint64_t,  Micro>;
	using Milliseconds	= typename std::chrono::duration<uint64_t,  Milli>;
	using Seconds		= typename std::chrono::duration<uint64_t,  Ratio<1>>;		   	
	using Minutes		= typename std::chrono::duration<uint64_t,  Ratio<60>>;	   	
	using Hours			= typename std::chrono::duration<uint64_t,  Ratio<3600>>;	   	
	using Days			= typename std::chrono::duration<uint64_t,  Ratio<86400>>;	   
	using Weeks			= typename std::chrono::duration<uint64_t,  Ratio<604800>>;   
	using Months		= typename std::chrono::duration<uint64_t,  Ratio<2419200>>;  
	using Years			= typename std::chrono::duration<uint64_t,  Ratio<31536000>>; 

	/* * * * * * * * * * * * * * * * * * * * */

	namespace alg
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T, class Num, class Den = typename Num
		> static constexpr T delta_cast(const Num numerator, const Den denominator)
		{
			using TT = type_t<T>;
			const T num { TT(numerator) };
			const T den { TT(denominator) };
			return (((den > TT::zero) && (num < den)) ? (num / den) : TT::zero);
		}

		template <
			class T, class Rep, class Per = typename Rep::period
		> static constexpr T delta_cast(const uint64_t value)
		{
			static_assert(0 < Per::den, "period negative or zero");
			using TT = type_t<T>;
			return TT(ML_duration_cast(Rep, (Nanoseconds)(value))) / TT(Per::den);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * */

	struct Duration final : public I_NonNewable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr Duration()
			: m_count { type_t<uint64_t>::zero }
		{
		}

		constexpr Duration(const uint64_t value)
			: m_count { value }
		{
		}

		constexpr Duration(const Duration & copy)
			: m_count { copy.m_count }
		{
		}

		template <
			class Rep, class Per
		> constexpr Duration(const std::chrono::duration<Rep, Per> & value)
			: m_count { ML_duration_cast(Nanoseconds, value) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		constexpr Nanoseconds base() const 
		{ 
			return static_cast<Nanoseconds>(m_count);
		}

		constexpr operator uint64_t() const
		{
			return m_count; 
		}

		template <
			class Rep = typename Milliseconds,
			class Per = typename Rep::period
		> constexpr float_t delta() const
		{
			return alg::delta_cast<float_t, Rep, Per>(m_count);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr uint64_t nanos	() const { return ML_duration_cast(Nanoseconds, base()); }
		constexpr uint64_t micros	() const { return ML_duration_cast(Microseconds, base()); }
		constexpr uint64_t millis	() const { return ML_duration_cast(Milliseconds, base()); }
		constexpr uint64_t seconds	() const { return ML_duration_cast(Seconds, base()); }
		constexpr uint64_t minutes	() const { return ML_duration_cast(Minutes, base()); }
		constexpr uint64_t hours	() const { return ML_duration_cast(Hours, base()); }
		constexpr uint64_t days		() const { return ML_duration_cast(Days, base()); }
		constexpr uint64_t weeks	() const { return ML_duration_cast(Weeks, base()); }
		constexpr uint64_t months	() const { return ML_duration_cast(Months, base()); }
		constexpr uint64_t years	() const { return ML_duration_cast(Years, base()); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		template <
			class T
		> constexpr friend bool operator==(const Duration & lhs, const T & rhs)
		{
			return (type_t<uint64_t>(lhs) == type_t<uint64_t>(Duration(rhs)));
		}

		template <
			class T
		> constexpr friend bool operator <(const Duration & lhs, const T & rhs)
		{
			return (type_t<uint64_t>(lhs) < type_t<uint64_t>(Duration(rhs)));
		}

		template <
			class T
		> constexpr friend bool operator!=(const Duration & lhs, const T & rhs)
		{
			return !(lhs == rhs);
		}

		template <
			class T
		> constexpr friend bool operator >(const Duration & lhs, const T & rhs)
		{
			return !(lhs < rhs);
		}

		template <
			class T
		> constexpr friend bool operator>=(const Duration & lhs, const T & rhs)
		{
			return (lhs > rhs) || (lhs == rhs);
		}

		template <
			class T
		> constexpr friend bool operator<=(const Duration & lhs, const T & rhs)
		{
			return (lhs < rhs) || (lhs == rhs);
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> constexpr friend Duration operator+(const Duration & lhs, const T & rhs)
		{
			return Duration(type_t<uint64_t>(lhs) + type_t<uint64_t>(rhs));
		}

		template <
			class T
		> constexpr friend Duration operator-(const Duration & lhs, const T & rhs)
		{
			return Duration(type_t<uint64_t>(lhs) - type_t<uint64_t>(rhs));
		}

		template <
			class T
		> constexpr friend Duration & operator+=(Duration & lhs, const T & rhs)
		{
			return (lhs = (lhs + rhs));
		}
		
		template <
			class T
		> constexpr friend Duration operator-=(Duration & lhs, const T & rhs)
		{
			return (lhs = (lhs - rhs));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: uint64_t m_count; // nanoseconds

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	inline ML_SERIALIZE(Ostream & out, const Duration & value)
	{
		return out
			<< (((value.hours() % 24) / 10) % 10)
			<< ((value.hours() % 24) % 10)
			<< ':'
			<< (((value.minutes() % 60) / 10) % 10)
			<< ((value.minutes() % 60) % 10)
			<< ':'
			<< (((value.seconds() % 60) / 10) % 10)
			<< ((value.seconds() % 60) % 10)
			<< ':'
			<< ((value.millis() % 1000) / 100)
			<< ((value.millis() % 100) / 10);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	constexpr Duration operator "" _ns	(uint64_t value) { return Nanoseconds	{ value }; }
	constexpr Duration operator "" _us	(uint64_t value) { return Microseconds	{ value }; }
	constexpr Duration operator "" _ms	(uint64_t value) { return Milliseconds	{ value }; }
	constexpr Duration operator "" _s	(uint64_t value) { return Seconds		{ value }; }
	constexpr Duration operator "" _min	(uint64_t value) { return Minutes		{ value }; }
	constexpr Duration operator "" _hr	(uint64_t value) { return Hours			{ value }; }
	constexpr Duration operator "" _d	(uint64_t value) { return Days			{ value }; }
	constexpr Duration operator "" _wk	(uint64_t value) { return Weeks			{ value }; }
	constexpr Duration operator "" _mo	(uint64_t value) { return Months		{ value }; }
	constexpr Duration operator "" _yr	(uint64_t value) { return Years			{ value }; }

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DURATION_HPP_