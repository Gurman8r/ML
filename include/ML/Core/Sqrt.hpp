#ifndef _ML_SQRT_HPP_
#define _ML_SQRT_HPP_

#include <ML/Core/Limits.hpp>

namespace ml
{
	namespace alg
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		namespace impl
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			template <class ... T> struct sqrt;

			template <> struct sqrt<> final { sqrt() = delete; };

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			template <> struct sqrt<uintmax_t>
			{
				using type = typename uintmax_t;
				
				constexpr sqrt() noexcept = default;
				
				constexpr type operator()(type value) const
				{
					return sqrt()(cast<type>::one, cast<type>::three, value);
				}
				
				constexpr type operator()(type value, type curr, type prev) const
				{
					return ((value <= prev)
						? sqrt()(value + curr, curr + cast<type>::two, prev)
						: ((curr >> cast<type>::one) - cast<type>::one)
					);
				}
			};

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			template <> struct sqrt<float64_t>
			{
				using type = typename float64_t;
				
				constexpr sqrt() noexcept = default;
				
				constexpr type operator()(type value, type curr, type prev) const
				{
					return ((curr == prev)
						? curr
						: sqrt()(value, cast<type>::half * (curr + value / curr), curr)
					);
				}
				
				constexpr type operator()(type value) const
				{
					return ((value >= cast<type>::zero && value < limits<type>::infinity)
						? sqrt()(value, value, cast<type>::zero)
						: limits<type>::nan
					);
				}
			};

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			template <class To, class From> struct sqrt<To, From> : public sqrt<From>
			{
				constexpr sqrt() noexcept = default;
				
				template <class U> constexpr To operator()(const U & value) const
				{
					return static_cast<To>(sqrt<From>{}(static_cast<From>(value)));
				}
			};

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			template <> struct sqrt<intmax_t>
			{
				constexpr sqrt() noexcept = default;
				
				constexpr intmax_t operator()(intmax_t value) const
				{
					return sqrt<intmax_t, uintmax_t>{}(value);
				}
			};

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			template <> struct sqrt<float32_t>
			{
				constexpr sqrt() noexcept = default;
				
				constexpr float32_t operator()(float32_t value) const
				{
					return sqrt<float32_t, float64_t>{}(value);
				}
			};

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> struct sqrt final : public _ML alg::impl::sqrt<T>
		{
			constexpr sqrt() noexcept = default;
			
			constexpr T operator()(T value) const
			{
				return _ML alg::impl::sqrt<T>{}(value);
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}
}

#endif // !_ML_SQRT_HPP_