#ifndef _ML_DURATION_HPP_
#define _ML_DURATION_HPP_

#include <ML/Core/Ratio.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	using Hours			= typename std::chrono::duration<int32_t, Ratio<3600>>;
	using Minutes		= typename std::chrono::duration<int32_t, Ratio<60>>;
	using Seconds		= typename std::chrono::duration<int64_t>;
	using Milliseconds	= typename std::chrono::duration<int64_t, Milli>;
	using Microseconds	= typename std::chrono::duration<int64_t, Micro>;
	using Nanoseconds	= typename std::chrono::duration<int64_t, Nano>;

	/* * * * * * * * * * * * * * * * * * * * */

	class Duration final
	{
	public: // Usings
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		using base_t = typename Nanoseconds;


	private: // Data
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		uint64_t m_base;


	public: // Constructors
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		constexpr Duration() noexcept
			: m_base(0)
		{
		}

		constexpr Duration(const uint64_t value) noexcept
			: m_base(value)
		{
		}

		constexpr Duration(const Duration & copy) noexcept
			: m_base(copy.m_base)
		{
		}

		template <
			class Rep,
			class Per
		> constexpr Duration(const std::chrono::duration<Rep, Per> & value) noexcept
			: m_base(std::chrono::duration_cast<base_t>(value).count())
		{
		}


	public: // Helpers
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		constexpr operator uint64_t() const
		{
			return m_base;
		}

		constexpr base_t base() const
		{
			return static_cast<base_t>(m_base);
		}

		template <
			class Rep = typename Milliseconds,
			class Per = typename Rep::period
		> constexpr float delta() const
		{
			static_assert(0 < Per::den,
				"period negative or zero"
			);
			return
				static_cast<float>(std::chrono::duration_cast<Rep>(base()).count()) /
				static_cast<float>(Per::den);
		}


	public: // Conversions
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		constexpr uint64_t hours() const
		{
			return std::chrono::duration_cast<Hours>(base()).count();
		}

		constexpr uint64_t minutes() const
		{
			return std::chrono::duration_cast<Minutes>(base()).count();
		}

		constexpr uint64_t seconds() const
		{
			return std::chrono::duration_cast<Seconds>(base()).count();
		}

		constexpr uint64_t millis() const
		{
			return std::chrono::duration_cast<Milliseconds>(base()).count();
		}

		constexpr uint64_t micros() const
		{
			return std::chrono::duration_cast<Microseconds>(base()).count();
		}

		constexpr uint64_t nanos() const
		{
			return std::chrono::duration_cast<Nanoseconds>(base()).count();
		}

	
	public: // Operators
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		inline friend std::ostream & operator<<(std::ostream & out, const Duration & value)
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
			return static_cast<uint64_t>(lhs) == 
				static_cast<uint64_t>(static_cast<Duration>(rhs));
		}

		template <class T>
		constexpr friend bool operator <(const Duration & lhs, const T & rhs)
		{
			return static_cast<uint64_t>(lhs) <
				static_cast<uint64_t>(static_cast<Duration>(rhs));
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
			return Duration(static_cast<uint64_t>(lhs) + static_cast<uint64_t>(rhs));
		}

		template <class T>
		constexpr friend Duration operator-(const Duration & lhs, const T & rhs)
		{
			return Duration(static_cast<uint64_t>(lhs) - static_cast<uint64_t>(rhs));
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
	};
}

#endif // !_ML_DURATION_HPP_