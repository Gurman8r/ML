#ifndef _ML_META_HPP_
#define _ML_META_HPP_

// Sources:
// https://github.com/Manu343726/ctti/blob/master/include/ctti/detail/meta.hpp
// https://stackoverflow.com/a/39348287/2977234

#include <ML/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace meta
	{
		/* * * * * * * * * * * * * * * * * * * * */

		template<typename T>
		using void_t = typename std::conditional<sizeof(T*) >= 0, void, T>::type;

		/* * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace meta
	{
		/* * * * * * * * * * * * * * * * * * * * */

		template<class X, class Y, class Op>
		struct op_valid_impl
		{
			template<class U, class L, class R>
			static auto test(int32_t) -> decltype(
				std::declval<U>()(std::declval<L>(), std::declval<R>()),
				void(),
				std::true_type()
			);

			template<class U, class L, class R>
			static auto test(...)->std::false_type;

			using type = decltype(test<Op, X, Y>(0));
		};

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			class X, class Y, class Op
		> using op_valid = typename op_valid_impl<X, Y, Op>::type;

		/* * * * * * * * * * * * * * * * * * * * */

		namespace notstd
		{
			struct left_shift
			{
				template <class L, class R>
				constexpr auto operator()(L && l, R && r) const
					noexcept(noexcept(std::forward<L>(l) << std::forward<R>(r)))
					-> decltype(std::forward<L>(l) << std::forward<R>(r))
				{
					return std::forward<L>(l) << std::forward<R>(r);
				}
			};

			struct right_shift
			{
				template <class L, class R>
				constexpr auto operator()(L && l, R && r) const
					noexcept(noexcept(std::forward<L>(l) >> std::forward<R>(r)))
					-> decltype(std::forward<L>(l) >> std::forward<R>(r))
				{
					return std::forward<L>(l) >> std::forward<R>(r);
				}
			};
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <class X, class Y> using has_equality		= op_valid<X, Y, std::equal_to<>>;
		template <class X, class Y> using has_inequality	= op_valid<X, Y, std::not_equal_to<>>;
		template <class X, class Y> using has_less_than		= op_valid<X, Y, std::less<>>;
		template <class X, class Y> using has_less_equal	= op_valid<X, Y, std::less_equal<>>;
		template <class X, class Y> using has_greater_than	= op_valid<X, Y, std::greater<>>;
		template <class X, class Y> using has_greater_equal = op_valid<X, Y, std::greater_equal<>>;
		template <class X, class Y> using has_bit_xor		= op_valid<X, Y, std::bit_xor<>>;
		template <class X, class Y> using has_bit_or		= op_valid<X, Y, std::bit_or<>>;
		template <class X, class Y> using has_left_shift	= op_valid<X, Y, notstd::left_shift>;
		template <class X, class Y> using has_right_shift	= op_valid<X, Y, notstd::right_shift>;

		/* * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_META_HPP_