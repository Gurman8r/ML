#ifndef _ML_DURATION_HPP_
#define _ML_DURATION_HPP_

#include <ML/Core/Ratio.hpp>
#include <ML/Core/NonNewable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	using Nanoseconds	= typename std::chrono::duration<int64_t, Nano>;
	using Microseconds	= typename std::chrono::duration<int64_t, Micro>;
	using Milliseconds	= typename std::chrono::duration<int64_t, Milli>;
	using Seconds		= typename std::chrono::duration<int64_t, Ratio<1>>;
	using Minutes		= typename std::chrono::duration<int64_t, Ratio<60>>;
	using Hours			= typename std::chrono::duration<int64_t, Ratio<60 * 60>>;
	using Days			= typename std::chrono::duration<int64_t, Ratio<60 * 60 * 24>>;

	/* * * * * * * * * * * * * * * * * * * * */

	// Length of time stored in nanoseconds
	struct Duration final : public NonNewable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using base_type = typename Nanoseconds;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr Duration() : m_base { 0 } {}

		constexpr Duration(uint64_t value) : m_base { static_cast<int64_t>(value) } {}

		constexpr Duration(const Duration & copy) : m_base { copy.m_base } {}

		template <
			class R, class P = typename R::period
		> constexpr Duration(const std::chrono::duration<R, P> & value)
			: m_base { cast<base_type>(value) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T, class R, class P = typename R::period
		> static constexpr int64_t cast(const std::chrono::duration<R, P> & value)
		{
			return std::chrono::duration_cast<T, R, P>(value).count();
		}
		
		constexpr operator const int64_t &() const { return m_base; }

		constexpr base_type base() const { return static_cast<Nanoseconds>(m_base); }

		template <
			class T = typename float_t,
			class R = typename Milliseconds,
			class P = typename R::period
		> constexpr T delta() const
		{
			static_assert(0 < P::den, "period negative or zero");
			return static_cast<T>(cast<R>(base())) / static_cast<T>(P::den);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr auto nanoseconds()	const { return cast<Nanoseconds>(base()); }
		constexpr auto microseconds()	const { return cast<Microseconds>(base()); }
		constexpr auto milliseconds()	const { return cast<Milliseconds>(base()); }
		constexpr auto seconds()		const { return cast<Seconds>(base()); }
		constexpr auto minutes()		const { return cast<Minutes>(base()); }
		constexpr auto hours()			const { return cast<Hours>(base()); }
		constexpr auto days()			const { return cast<Days>(base()); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> constexpr friend bool operator==(const Duration & lhs, const T & rhs)
		{
			return !(lhs < rhs) && !(rhs < lhs);
		}

		template <
			class T
		> constexpr friend bool operator!=(const Duration & lhs, const T & rhs)
		{
			return !(lhs == rhs);
		}

		template <
			class T
		> constexpr friend bool operator <(const Duration & lhs, const T & rhs)
		{
			return ((int64_t)(lhs) < (int64_t)(Duration(rhs)));
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
			return Duration { (int64_t)lhs + (int64_t)rhs };
		}

		template <
			class T
		> constexpr friend Duration operator-(const Duration & lhs, const T & rhs)
		{
			return Duration { (int64_t)lhs - (int64_t)rhs };
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

	private: int64_t m_base; // nanoseconds

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	inline ML_SERIALIZE(std::ostream & out, const Duration & value)
	{
		const auto hr	= value.hours();
		const auto m	= value.minutes();
		const auto s	= value.seconds();
		const auto ms	= value.milliseconds();
		return out
			<< (((hr % 24) / 10) % 10)	<< ((hr % 24) % 10) << ':'
			<< (((m % 60) / 10) % 10)	<< ((m % 60) % 10)	<< ':'
			<< (((s % 60) / 10) % 10)	<< ((s % 60) % 10)	<< ':'
			<< ((ms % 1000) / 100)		<< ((ms % 100) / 10);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	constexpr Duration operator "" _ns	(uint64_t value) { return { Nanoseconds { value } }; }
	constexpr Duration operator "" _us	(uint64_t value) { return { Microseconds { value } }; }
	constexpr Duration operator "" _ms	(uint64_t value) { return { Milliseconds { value } }; }
	constexpr Duration operator "" _s	(uint64_t value) { return { Seconds { value } }; }
	constexpr Duration operator "" _min	(uint64_t value) { return { Minutes { value } }; }
	constexpr Duration operator "" _h	(uint64_t value) { return { Hours { value } }; }

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DURATION_HPP_