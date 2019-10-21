#ifndef _ML_DURATION_HPP_
#define _ML_DURATION_HPP_

#include <ML/Core/Ratio.hpp>
#include <ML/Core/NonNewable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	namespace chrono = _STD chrono;

	/* * * * * * * * * * * * * * * * * * * * */

	using Nanoseconds	= typename chrono::duration<float64_t, Nano>;
	using Microseconds	= typename chrono::duration<float64_t, Micro>;
	using Milliseconds	= typename chrono::duration<float64_t, Milli>;
	using Seconds		= typename chrono::duration<float64_t, Ratio<1>>;
	using Minutes		= typename chrono::duration<float64_t, Ratio<60>>;
	using Hours			= typename chrono::duration<float64_t, Ratio<60 * 60>>;
	using Days			= typename chrono::duration<float64_t, Ratio<60 * 60 * 24>>;

	/* * * * * * * * * * * * * * * * * * * * */

	struct Duration final : public NonNewable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using base_type = typename chrono::duration<float64_t>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr Duration() : m_base { 0 } {}

		constexpr Duration(float64_t value) : m_base { value } {}

		constexpr Duration(const Duration & copy) : m_base { copy.m_base } {}

		template <
			class R, class P = typename R::period
		> constexpr Duration(const chrono::duration<R, P> & value)
			: m_base { chrono::duration_cast<base_type>(value) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr base_type base() const { return m_base; }

		constexpr float64_t count() const { return m_base.count(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr auto nanoseconds() const { return chrono::duration_cast<Nanoseconds>(m_base); }
		constexpr auto microseconds() const { return chrono::duration_cast<Microseconds>(m_base); }
		constexpr auto milliseconds() const { return chrono::duration_cast<Milliseconds>(m_base); }
		constexpr auto seconds() const { return chrono::duration_cast<Seconds>(m_base); }
		constexpr auto minutes() const { return chrono::duration_cast<Minutes>(m_base); }
		constexpr auto hours() const { return chrono::duration_cast<Hours>(m_base); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: base_type m_base;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	inline ML_SERIALIZE(std::ostream & out, const Duration & value)
	{
		const auto hr = (int32_t)value.hours().count();
		const auto m = (int32_t)value.minutes().count();
		const auto s = (int32_t)value.seconds().count();
		const auto ms = (int32_t)value.milliseconds().count();
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