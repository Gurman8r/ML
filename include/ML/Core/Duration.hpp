#ifndef _ML_DURATION_HPP_
#define _ML_DURATION_HPP_

#include <ML/Core/Ratio.hpp>
#include <ML/Core/NonNewable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	using Nanoseconds	= typename std::chrono::duration<time_t, Nano>;
	using Microseconds	= typename std::chrono::duration<time_t, Micro>;
	using Milliseconds	= typename std::chrono::duration<time_t, Milli>;
	using Seconds		= typename std::chrono::duration<time_t, Ratio<1>>;
	using Minutes		= typename std::chrono::duration<time_t, Ratio<60>>;
	using Hours			= typename std::chrono::duration<time_t, Ratio<60 * 60>>;
	using Days			= typename std::chrono::duration<time_t, Ratio<60 * 60 * 24>>;

	/* * * * * * * * * * * * * * * * * * * * */

	// Unit of time stored in nanoseconds
	struct Duration final : public NonNewable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using unit_type = typename Nanoseconds;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr Duration()
			: m_base { NULL }
		{
		}

		constexpr Duration(const time_t value)
			: m_base { value }
		{
		}

		constexpr Duration(const Duration & copy)
			: m_base { copy.m_base }
		{
		}

		template <
			class R, class P = typename R::period
		> constexpr Duration(const std::chrono::duration<R, P> & value)
			: m_base { cast<unit_type>(value) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T, class R, class P = typename R::period
		> static constexpr auto cast(const std::chrono::duration<R, P> & value)
		{
			return std::chrono::duration_cast<T, R, P>(value).count();
		}
		
		constexpr operator const time_t &() const
		{ 
			return m_base;
		}

		constexpr unit_type base() const
		{
			return static_cast<Nanoseconds>(m_base);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr time_t nanos()	const { return cast<Nanoseconds>(base()); }
		constexpr time_t micros()	const { return cast<Microseconds>(base()); }
		constexpr time_t millis()	const { return cast<Milliseconds>(base()); }
		constexpr time_t seconds()	const { return cast<Seconds>(base()); }
		constexpr time_t minutes()	const { return cast<Minutes>(base()); }
		constexpr time_t hours()	const { return cast<Hours>(base()); }
		constexpr time_t days()		const { return cast<Days>(base()); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		template <
			class T = typename float_t,
			class R = typename Milliseconds,
			class P = typename R::period
		> constexpr float_t delta() const
		{
			static_assert(0 < P::den, "period negative or zero");
			return static_cast<T>(cast<R>(base())) / static_cast<T>(P::den);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		template <
			class T
		> constexpr friend bool operator==(const Duration & lhs, const T & rhs)
		{
			return ((time_t)(lhs) == (time_t)(Duration(rhs)));
		}

		template <
			class T
		> constexpr friend bool operator <(const Duration & lhs, const T & rhs)
		{
			return ((time_t)(lhs) < (time_t)(Duration(rhs)));
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
			return Duration((time_t)(lhs) + (time_t)(rhs));
		}

		template <
			class T
		> constexpr friend Duration operator-(const Duration & lhs, const T & rhs)
		{
			return Duration((time_t)(lhs) - (time_t)(rhs));
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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: time_t m_base; // nanoseconds

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	inline ML_SERIALIZE(std::ostream & out, const Duration & value)
	{
		const auto hr	= value.hours();
		const auto m	= value.minutes();
		const auto s	= value.seconds();
		const auto ms	= value.millis();
		return out
			<< (((hr % 24) / 10) % 10)	<< ((hr % 24) % 10) << ':'
			<< (((m % 60) / 10) % 10)	<< ((m % 60) % 10)	<< ':'
			<< (((s % 60) / 10) % 10)	<< ((s % 60) % 10)	<< ':'
			<< ((ms % 1000) / 100)		<< ((ms % 100) / 10);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	constexpr Duration operator "" _ns	(time_t value) { return { Nanoseconds	{ value } }; }
	constexpr Duration operator "" _us	(time_t value) { return { Microseconds	{ value } }; }
	constexpr Duration operator "" _ms	(time_t value) { return { Milliseconds	{ value } }; }
	constexpr Duration operator "" _sec	(time_t value) { return { Seconds	{ value } }; }
	constexpr Duration operator "" _min	(time_t value) { return { Minutes	{ value } }; }
	constexpr Duration operator "" _hr	(time_t value) { return { Hours		{ value } }; }
	constexpr Duration operator "" _dy	(time_t value) { return { Days		{ value } }; }

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DURATION_HPP_