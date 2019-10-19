#ifndef _ML_DURATION_HPP_
#define _ML_DURATION_HPP_

#include <ML/Core/Ratio.hpp>
#include <ML/Core/NonNewable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	//using Nanoseconds	= typename std::chrono::nanoseconds;
	//using Microseconds	= typename std::chrono::microseconds;
	//using Milliseconds	= typename std::chrono::milliseconds;
	//using Seconds		= typename std::chrono::seconds;
	//using Minutes		= typename std::chrono::minutes;
	//using Hours			= typename std::chrono::hours;

	using Nanoseconds	= typename std::chrono::duration<float64_t, Nano>;
	using Microseconds	= typename std::chrono::duration<float64_t, Micro>;
	using Milliseconds	= typename std::chrono::duration<float64_t, Milli>;
	using Seconds		= typename std::chrono::duration<float64_t, Ratio<1>>;
	using Minutes		= typename std::chrono::duration<float64_t, Ratio<60>>;
	using Hours			= typename std::chrono::duration<float64_t, Ratio<60 * 60>>;

	/* * * * * * * * * * * * * * * * * * * * */

	// Length of time stored in nanoseconds
	struct Duration final : public NonNewable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr Duration()
			: m_base { 0 }
		{
		}

		constexpr Duration(const float64_t value)
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
			: m_base { cast<Nanoseconds>(value) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T, class R, class P = typename R::period
		> static constexpr auto cast(const std::chrono::duration<R, P> & value)
		{
			return std::chrono::duration_cast<T, R, P>(value);
		}
		
		constexpr operator const float64_t &() const
		{ 
			return m_base.count();
		}

		constexpr Nanoseconds base() const 
		{ 
			return m_base;
		}

		template <
			class T = typename float_t,
			class R = typename Milliseconds,
			class P = typename R::period
		> constexpr T delta() const
		{
			static_assert(0 < P::den, "period negative or zero");
			return static_cast<T>(cast<R>(m_base).count()) / static_cast<T>(P::den);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr auto nanoseconds() const	{ return cast<Nanoseconds>(m_base); }
		constexpr auto microseconds() const { return cast<Microseconds>(m_base); }
		constexpr auto milliseconds() const { return cast<Milliseconds>(m_base); }
		constexpr auto seconds() const		{ return cast<Seconds>(m_base); }
		constexpr auto minutes() const		{ return cast<Minutes>(m_base); }
		constexpr auto hours() const		{ return cast<Hours>(m_base); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> constexpr friend bool operator==(const Duration & lhs, const T & rhs)
		{
			return !(lhs < rhs) && !(rhs < lhs);
		}

		template <class T> constexpr friend bool operator!=(const Duration & lhs, const T & rhs)
		{
			return !(lhs == rhs);
		}

		template <class T> constexpr friend bool operator <(const Duration & lhs, const T & rhs)
		{
			return ((float64_t)(lhs) < (float64_t)(Duration(rhs)));
		}

		template <class T> constexpr friend bool operator >(const Duration & lhs, const T & rhs)
		{
			return !(lhs < rhs);
		}

		template <class T> constexpr friend bool operator>=(const Duration & lhs, const T & rhs)
		{
			return (lhs > rhs) || (lhs == rhs);
		}

		template <class T> constexpr friend bool operator<=(const Duration & lhs, const T & rhs)
		{
			return (lhs < rhs) || (lhs == rhs);
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <class T> constexpr friend Duration operator+(const Duration & lhs, const T & rhs)
		{
			return Duration { (float64_t)lhs + (float64_t)rhs };
		}

		template <class T> constexpr friend Duration operator-(const Duration & lhs, const T & rhs)
		{
			return Duration { (float64_t)lhs - (float64_t)rhs };
		}

		template <class T> constexpr friend Duration & operator+=(Duration & lhs, const T & rhs)
		{
			return (lhs = (lhs + rhs));
		}
		
		template <class T> constexpr friend Duration operator-=(Duration & lhs, const T & rhs)
		{
			return (lhs = (lhs - rhs));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: Nanoseconds m_base;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	inline ML_SERIALIZE(std::ostream & out, const Duration & value)
	{
		const auto hr = (int32_t)value.hours().count();
		const auto mn = (int32_t)value.milliseconds().count();
		const auto sc = (int32_t)value.seconds().count();
		const auto ms = (int32_t)value.milliseconds().count();
		return out
			<< (((hr % 24) / 10) % 10)	<< ((hr % 24) % 10) << ':'
			<< (((mn % 60) / 10) % 10)	<< ((mn % 60) % 10)	<< ':'
			<< (((sc % 60) / 10) % 10)	<< ((sc % 60) % 10)	<< ':'
			<< ((ms % 1000) / 100)		<< ((ms % 100) / 10);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DURATION_HPP_