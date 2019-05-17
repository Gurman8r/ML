#ifndef _ML_DETECTION_HPP_
#define _ML_DETECTION_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/StandardLib.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// https://www.youtube.com/watch?v=W3ViIBnTTKA
// https://en.cppreference.com/w/cpp/experimental/is_detected
// https://en.cppreference.com/w/cpp/experimental/nonesuch

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace std
{
	namespace detail
	{
		template <
			class Default,
			class AlwaysVoid,
			template <class ...> class Op,
			class ... Args
		> struct detector
		{
			using value_t = std::false_type;
			using type = Default;
		};

		template <
			class Default,
			template <class ...> class Op,
			class ... Args
		> struct detector<Default, std::void_t<Op<Args...>>, Op, Args...>
		{
			// Note that std::void_t is a C++17 feature
			using value_t = std::true_type;
			using type = Op<Args...>;
		};
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace std
{
	struct nonesuch
	{
		~nonesuch() = delete;
		nonesuch(nonesuch const &) = delete;
		void operator=(nonesuch const &) = delete;
	};

	template <
		template <class ...> class Op,
		class ... Args
	> using is_detected = typename detail::detector<nonesuch, void, Op, Args...>::value_t;

	template <
		template <class ...> class Op,
		class ... Args
	> using detected_t = typename detail::detector<nonesuch, void, Op, Args...>::type;

	template <
		class Default,
		template <class ...> class Op,
		class ... Args
	> using detected_or = typename detail::detector<Default, void, Op, Args...>;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_GENERATE_DETECT_HAS_VOID_MEMFUN(FUN) \
template <typename T> \
using FUN##_detector = decltype(std::declval<T&>().FUN()); \
template <typename T> \
using has_##FUN = std::is_detected<FUN##_detector, T>; \
template <typename T> \
constexpr bool has_##FUN##_v = has_##FUN<T>::value;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	ML_GENERATE_DETECT_HAS_VOID_MEMFUN(Update);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_DETECTION_HPP_