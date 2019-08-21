#ifndef _ML_DURATION_HPP_
#define _ML_DURATION_HPP_

#include <ML/Core/Ratio.hpp>
#include <ML/Core/I_NonNewable.hpp>

#define ML_duration_cast(RATIO, VALUE) (_STD chrono::duration_cast<RATIO>(VALUE).count())

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	namespace detail
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using Seconds	= Ratio< 1 >;
		using Minutes	= Ratio< Seconds::num	* 60>;
		using Hours		= Ratio< Minutes::num	* 60>;
		using Days		= Ratio< Hours::num		* 24>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * */

	using Nanosec	= typename std::chrono::duration<time_t,  Nano>;
	using Microsec	= typename std::chrono::duration<time_t,  Micro>;
	using Millisec	= typename std::chrono::duration<time_t,  Milli>;
	using Seconds	= typename std::chrono::duration<time_t,  detail::Seconds>;		   	
	using Minutes	= typename std::chrono::duration<time_t,  detail::Minutes>;	   	
	using Hours		= typename std::chrono::duration<time_t,  detail::Hours>;	   	
	using Days		= typename std::chrono::duration<time_t,  detail::Days>;

	/* * * * * * * * * * * * * * * * * * * * */

	// Unit of time stored in nanoseconds
	struct Duration final : public I_NonNewable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type = typename time_t;
		using unit_type  = typename Nanosec;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr Duration()
			: m_base { uninit }
		{
		}

		constexpr Duration(const value_type value)
			: m_base { value }
		{
		}

		constexpr Duration(const Duration & copy)
			: m_base { copy.m_base }
		{
		}

		template <
			class Rep, class Per = typename Rep::period
		> constexpr Duration(const std::chrono::duration<Rep, Per> & value)
			: m_base { ML_duration_cast(unit_type, value) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		constexpr operator value_type() const
		{ 
			return m_base; 
		}

		constexpr unit_type base() const
		{
			return static_cast<Nanosec>((value_type)(*this));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr value_type nanos	() const { return ML_duration_cast(Nanosec,	base()); }
		constexpr value_type micros	() const { return ML_duration_cast(Microsec,base()); }
		constexpr value_type millis	() const { return ML_duration_cast(Millisec,base()); }
		constexpr value_type seconds() const { return ML_duration_cast(Seconds, base()); }
		constexpr value_type minutes() const { return ML_duration_cast(Minutes, base()); }
		constexpr value_type hours	() const { return ML_duration_cast(Hours,	base()); }
		constexpr value_type days	() const { return ML_duration_cast(Days,	base()); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		template <
			class Rep = typename Millisec,
			class Per = typename Rep::period
		> constexpr float_t delta() const
		{
			static_assert(0 < Per::den, "period negative or zero");
			using cast = numeric<float_t>;
			return cast(ML_duration_cast(Rep, base())) / cast(Per::den);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		template <
			class T
		> constexpr friend bool operator==(const Duration & lhs, const T & rhs)
		{
			return ((value_type)(lhs) == (value_type)(Duration(rhs)));
		}

		template <
			class T
		> constexpr friend bool operator <(const Duration & lhs, const T & rhs)
		{
			return ((value_type)(lhs) < (value_type)(Duration(rhs)));
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
			return Duration((value_type)(lhs) + (value_type)(rhs));
		}

		template <
			class T
		> constexpr friend Duration operator-(const Duration & lhs, const T & rhs)
		{
			return Duration((value_type)(lhs) - (value_type)(rhs));
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

	private: value_type m_base; // nanoseconds

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	inline ML_SERIALIZE(Ostream & out, const Duration & value)
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

	constexpr Duration operator "" _ns	(time_t value) { return { Nanosec	{ value } }; }
	constexpr Duration operator "" _us	(time_t value) { return { Microsec	{ value } }; }
	constexpr Duration operator "" _ms	(time_t value) { return { Millisec	{ value } }; }
	constexpr Duration operator "" _sec	(time_t value) { return { Seconds	{ value } }; }
	constexpr Duration operator "" _min	(time_t value) { return { Minutes	{ value } }; }
	constexpr Duration operator "" _hr	(time_t value) { return { Hours		{ value } }; }
	constexpr Duration operator "" _dy	(time_t value) { return { Days		{ value } }; }

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DURATION_HPP_