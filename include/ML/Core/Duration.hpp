#ifndef _ML_DURATION_HPP_
#define _ML_DURATION_HPP_

#include <ML/Core/Ratio.hpp>
#include <ML/Core/I_NonNewable.hpp>

#define ML_TIME_BASE		::std::chrono::duration
#define ML_TIME_CAST(T, V)	::std::chrono::duration_cast<T>(V).count()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	using Nanoseconds	= typename ML_TIME_BASE<time_t, Nano>;				// 1	: 1000000000
	using Microseconds	= typename ML_TIME_BASE<time_t, Micro>;				// 1	: 1000000
	using Milliseconds	= typename ML_TIME_BASE<time_t, Milli>;				// 1	: 1000
	using Seconds		= typename ML_TIME_BASE<time_t>;					// 1	: 1
	using Kiloseconds	= typename ML_TIME_BASE<time_t, Kilo>;				// 1000 : 1
	using Minutes		= typename ML_TIME_BASE<time_t, Ratio<60>>;			// s	* 60
	using Hours			= typename ML_TIME_BASE<time_t, Ratio<3600>>;		// m	* 60
	using Days			= typename ML_TIME_BASE<time_t, Ratio<86400>>;		// hr	* 24
	using Weeks			= typename ML_TIME_BASE<time_t, Ratio<604800>>;		// d	* 7
	using Months		= typename ML_TIME_BASE<time_t, Ratio<2419200>>;	// wk	* 4
	using Years			= typename ML_TIME_BASE<time_t, Ratio<31536000>>;	// d	* 365

	/* * * * * * * * * * * * * * * * * * * * */

	struct Duration final : public I_NonNewable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using type = typename type_t<time_t>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr Duration()
			: Duration { type::zero }
		{
		}

		constexpr Duration(const time_t value)
			: m_base { value }
		{
		}

		constexpr Duration(const Duration & copy)
			: Duration { copy.m_base }
		{
		}

		template <
			class Rep, class Per
		> constexpr Duration(const ML_TIME_BASE<Rep, Per> & value)
			: Duration { ML_TIME_CAST(Nanoseconds, value) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr operator time_t() const 
		{ 
			return m_base; 
		}
		
		constexpr Nanoseconds base() const 
		{ 
			return static_cast<Nanoseconds>(m_base); 
		}

		template <
			class Rep = typename Milliseconds,
			class Per = typename Rep::period
		> constexpr float_t delta() const
		{
			static_assert(type::zero < Per::den, "period negative or zero");
			return (
				static_cast<float_t>(ML_TIME_CAST(Rep, base())) /
				static_cast<float_t>(Per::den)
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr time_t nanos	() const { return ML_TIME_CAST(Nanoseconds, base()); }
		constexpr time_t micros	() const { return ML_TIME_CAST(Microseconds, base()); }
		constexpr time_t millis	() const { return ML_TIME_CAST(Milliseconds, base()); }
		constexpr time_t kilosec() const { return ML_TIME_CAST(Kiloseconds, base()); }
		constexpr time_t seconds() const { return ML_TIME_CAST(Seconds, base()); }
		constexpr time_t minutes() const { return ML_TIME_CAST(Minutes, base()); }
		constexpr time_t hours	() const { return ML_TIME_CAST(Hours, base()); }
		constexpr time_t days	() const { return ML_TIME_CAST(Days, base()); }
		constexpr time_t weeks	() const { return ML_TIME_CAST(Weeks, base()); }
		constexpr time_t months	() const { return ML_TIME_CAST(Months, base()); }
		constexpr time_t years	() const { return ML_TIME_CAST(Years, base()); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		template <
			class T
		> constexpr friend bool operator==(const Duration & lhs, const T & rhs)
		{
			return (type(lhs) == type(Duration(rhs)));
		}

		template <
			class T
		> constexpr friend bool operator <(const Duration & lhs, const T & rhs)
		{
			return (type(lhs) < type(Duration(rhs)));
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
			return Duration(type(lhs) + type(rhs));
		}

		template <
			class T
		> constexpr friend Duration operator-(const Duration & lhs, const T & rhs)
		{
			return Duration(type(lhs) - type(rhs));
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

	private: time_t m_base;

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

	constexpr Duration operator "" _ns	(time_t value) { return Nanoseconds	(value); }
	constexpr Duration operator "" _us	(time_t value) { return Microseconds(value); }
	constexpr Duration operator "" _ms	(time_t value) { return Milliseconds(value); }
	constexpr Duration operator "" _ks	(time_t value) { return Kiloseconds	(value); }
	constexpr Duration operator "" _s	(time_t value) { return Seconds		(value); }
	constexpr Duration operator "" _min	(time_t value) { return Minutes		(value); }
	constexpr Duration operator "" _hr	(time_t value) { return Hours		(value); }
	constexpr Duration operator "" _d	(time_t value) { return Days		(value); }
	constexpr Duration operator "" _wk	(time_t value) { return Weeks		(value); }
	constexpr Duration operator "" _mo	(time_t value) { return Months		(value); }
	constexpr Duration operator "" _yr	(time_t value) { return Years		(value); }

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DURATION_HPP_