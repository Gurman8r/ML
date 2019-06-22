#ifndef _ML_DURATION_HPP_
#define _ML_DURATION_HPP_

#include <ML/Core/Ratio.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	using Hours			= typename std::chrono::duration<time_t, Ratio<3600>>;
	using Minutes		= typename std::chrono::duration<time_t, Ratio<60>>;
	using Seconds		= typename std::chrono::duration<time_t>;
	using Milliseconds	= typename std::chrono::duration<time_t, Milli>;
	using Microseconds	= typename std::chrono::duration<time_t, Micro>;
	using Nanoseconds	= typename std::chrono::duration<time_t, Nano>;

	/* * * * * * * * * * * * * * * * * * * * */

	struct Duration final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using type = typename meta::type_t<time_t>;

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
			class Rep,
			class Per
		> constexpr Duration(const std::chrono::duration<Rep, Per> & value)
			: Duration { std::chrono::duration_cast<Nanoseconds>(value).count() }
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
		> constexpr float delta() const
		{
			static_assert(type::zero < Per::den, "period negative or zero");
			return
				static_cast<float>(std::chrono::duration_cast<Rep>(base()).count()) /
				static_cast<float>(Per::den);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr time_t hours() const
		{
			return std::chrono::duration_cast<Hours>(base()).count();
		}

		constexpr time_t minutes() const
		{
			return std::chrono::duration_cast<Minutes>(base()).count();
		}

		constexpr time_t seconds() const
		{
			return std::chrono::duration_cast<Seconds>(base()).count();
		}

		constexpr time_t millis() const
		{
			return std::chrono::duration_cast<Milliseconds>(base()).count();
		}

		constexpr time_t micros() const
		{
			return std::chrono::duration_cast<Microseconds>(base()).count();
		}

		constexpr time_t nanos() const
		{
			return std::chrono::duration_cast<Nanoseconds>(base()).count();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		inline friend ML_SERIALIZE(ostream & out, const Duration & value)
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
		
		template <class T>
		constexpr friend bool operator==(const Duration & lhs, const T & rhs)
		{
			return static_cast<time_t>(lhs) == 
				static_cast<time_t>(static_cast<Duration>(rhs));
		}

		template <class T>
		constexpr friend bool operator <(const Duration & lhs, const T & rhs)
		{
			return static_cast<time_t>(lhs) <
				static_cast<time_t>(static_cast<Duration>(rhs));
		}

		template <class T>
		constexpr friend bool operator!=(const Duration & lhs, const T & rhs)
		{
			return !(lhs == rhs);
		}

		template <class T>
		constexpr friend bool operator >(const Duration & lhs, const T & rhs)
		{
			return !(lhs < rhs);
		}

		template <class T>
		constexpr friend bool operator>=(const Duration & lhs, const T & rhs)
		{
			return (lhs > rhs) || (lhs == rhs);
		}

		template <class T>
		constexpr friend bool operator<=(const Duration & lhs, const T & rhs)
		{
			return (lhs < rhs) || (lhs == rhs);
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <class T>
		constexpr friend Duration operator+(const Duration & lhs, const T & rhs)
		{
			return Duration(static_cast<time_t>(lhs) + static_cast<time_t>(rhs));
		}

		template <class T>
		constexpr friend Duration operator-(const Duration & lhs, const T & rhs)
		{
			return Duration(static_cast<time_t>(lhs) - static_cast<time_t>(rhs));
		}

		template <class T>
		constexpr friend Duration & operator+=(Duration & lhs, const T & rhs)
		{
			return (lhs = (lhs + rhs));
		}
		
		template <class T>
		constexpr friend Duration operator-=(Duration & lhs, const T & rhs)
		{
			return (lhs = (lhs - rhs));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: time_t m_base;
	};
}

#endif // !_ML_DURATION_HPP_