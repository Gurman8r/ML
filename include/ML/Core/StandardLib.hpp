#ifndef _ML_STANDARD_LIB_HPP_
#define _ML_STANDARD_LIB_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/Config.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(ML_HAS_CXX20)
#	include <concepts>
#	include <ranges>
#endif

#if defined(ML_HAS_CXX17)
#	include <any>
#	include <filesystem>
#	include <optional>
#	include <string_view>
#	include <variant>
#endif

#if defined(ML_HAS_CXX14)
#	include <algorithm>
#	include <array>
#	include <atomic>
#	include <cassert>
#	include <cctype>
#	include <chrono>
#	include <csignal>
#	include <cstddef>
#	include <ctime>
#	include <fstream>
#	include <future>
#	include <initializer_list>
#	include <iomanip>
#	include <iostream>
#	include <iterator>
#	include <limits>
#	include <locale>
#	include <map>
#	include <mutex>
#	include <queue>
#	include <random>
#	include <regex>
#	include <stack>
#	include <set>
#	include <sstream>
#	include <string>
#	include <thread>
#	include <type_traits>
#	include <typeindex>
#	include <typeinfo>
#	include <unordered_map>
#	include <unordered_set>
#	include <utility>
#	include <vector>
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_ARGC		__argc
#define ML_ARGV		__argv
#define ML_WARGV		__wargv

#define ML_DESERIALIZE	::std::istream & operator >>
#define ML_SERIALIZE	::std::ostream & operator <<

# ifdef ML_CC_MSC
#	define popen		_popen
#	define pclose		_pclose
#	define strdup		_strdup
#	define snprintf		_snprintf
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_USING	int8_t		= typename ML_INT8;
	ML_USING	int16_t		= typename ML_INT16;
	ML_USING	int32_t		= typename ML_INT32;
	ML_USING	int64_t		= typename ML_INT64;

	ML_USING	uint8_t		= typename ML_UINT8;
	ML_USING	uint16_t	= typename ML_UINT16;
	ML_USING	uint32_t	= typename ML_UINT32;
	ML_USING	uint64_t	= typename ML_UINT64;

	ML_USING	float32_t	= typename ML_FLOAT32;
	ML_USING	float64_t	= typename ML_FLOAT64;
	ML_USING	float80_t	= typename ML_FLOAT80;

	ML_USING	intmax_t	= typename ML_INTMAX;
	ML_USING	uintmax_t	= typename ML_UINTMAX;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_USING	byte_t		= typename uint8_t;
	ML_USING	float_t		= typename float32_t;
	ML_USING	hash_t		= typename uint64_t;
	ML_USING	intptr_t	= typename intmax_t;
	ML_USING	ptrdiff_t	= typename intptr_t;
	ML_USING	size_t		= typename uintmax_t;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_USING_X	ptr_t		= typename X *;
	ML_USING_X	const_ptr_t	= typename X const *;
	ML_USING	voidptr_t 	= typename ptr_t<void>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_USING	C_String	= typename const_ptr_t<char>;
	ML_USING	CW_String	= typename const_ptr_t<wchar_t>;
	ML_USING	C16_String	= typename const_ptr_t<char16_t>;
	ML_USING	C32_String	= typename const_ptr_t<char32_t>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_USING_Ts HashMap		= typename _STD unordered_map<Ts...>;
	ML_USING_Ts List		= typename _STD vector<Ts...>;
	ML_USING_Ts Map			= typename _STD map<Ts...>;
	ML_USING_Ts Shared		= typename _STD shared_ptr<Ts...>;
	ML_USING_Ts Unique		= typename _STD unique_ptr<Ts...>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class Ch, class Tr = typename _STD char_traits<char>
	> inline _STD basic_ostream<Ch, Tr> & endl(_STD basic_ostream<Ch, Tr> & out)
	{
		out.put(out.widen('\n'));
		out.flush();
		return out;
	}

	static _STD ostream & cout { _STD cout };
	static _STD ostream & cerr { _STD cerr };
	static _STD istream & cin  { _STD cin  };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_STANDARD_LIB_HPP_