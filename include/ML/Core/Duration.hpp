#ifndef _ML_DURATION_HPP_
#define _ML_DURATION_HPP_

#include <ML/Core/Ratio.hpp>
#include <ML/Core/NonNewable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	using Nanoseconds	= typename std::chrono::duration<float64_t, Nano>;
	using Microseconds	= typename std::chrono::duration<float64_t, Micro>;
	using Milliseconds	= typename std::chrono::duration<float64_t, Milli>;
	using Seconds		= typename std::chrono::duration<float64_t, Ratio<1>>;
	using Minutes		= typename std::chrono::duration<float64_t, Ratio<60>>;
	using Hours			= typename std::chrono::duration<float64_t, Ratio<60 * 60>>;
	using Days			= typename std::chrono::duration<float64_t, Ratio<60 * 60 * 24>>;

	/* * * * * * * * * * * * * * * * * * * * */

	struct Duration final : public NonNewable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using base_type = typename std::chrono::duration<float64_t>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr Duration() : m_base { 0 } {}

		constexpr Duration(float64_t value) : m_base { value } {}

		constexpr Duration(const Duration & copy) : m_base { copy.m_base } {}

		template <
			class R, class P = typename R::period
		> constexpr Duration(const std::chrono::duration<R, P> & value)
			: m_base { std::chrono::duration_cast<base_type>(value) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr base_type base() const { return m_base; }

		constexpr float64_t count() const { return m_base.count(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr Nanoseconds nanoseconds() const
		{
			return std::chrono::duration_cast<Nanoseconds>(m_base);
		}

		constexpr Microseconds microseconds() const
		{
			return std::chrono::duration_cast<Microseconds>(m_base);
		}

		constexpr Milliseconds milliseconds() const
		{
			return std::chrono::duration_cast<Milliseconds>(m_base);
		}

		constexpr Seconds seconds() const
		{
			return std::chrono::duration_cast<Seconds>(m_base);
		}

		constexpr Minutes minutes() const
		{
			return std::chrono::duration_cast<Minutes>(m_base);
		}

		constexpr Hours hours() const
		{
			return std::chrono::duration_cast<Hours>(m_base);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: base_type m_base;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DURATION_HPP_