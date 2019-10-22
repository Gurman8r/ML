#ifndef _ML_STANDARD_LIB_HPP_
#define _ML_STANDARD_LIB_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/Config.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

# if defined(ML_HAS_CXX20)
#	include <concepts>
# endif

# if defined(ML_HAS_CXX17)
#	include <any>
#	include <filesystem>
#	include <optional>
#	include <string_view>
#	include <variant>
# endif

# if defined(ML_HAS_CXX14)
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
# endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_ARGC	__argc	// Pointer to number of command line arguments
#define ML_ARGV	__argv	// Pointer to table of narrow command line arguments

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

# ifdef ML_CC_MSC // Deprecated Functions
#	define popen	_popen
#	define pclose	_pclose
#	define strdup	_strdup
#	define snprintf	_snprintf
# else
# endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using int8_t		= typename ML_INT8;
	using int16_t		= typename ML_INT16;
	using int32_t		= typename ML_INT32;
	using int64_t		= typename ML_INT64;

	using uint8_t		= typename ML_UINT8;
	using uint16_t		= typename ML_UINT16;
	using uint32_t		= typename ML_UINT32;
	using uint64_t		= typename ML_UINT64;

	using float32_t		= typename ML_FLOAT32;
	using float64_t		= typename ML_FLOAT64;
	using float80_t		= typename ML_FLOAT80;

	using intmax_t		= typename ML_INTMAX;
	using uintmax_t		= typename ML_UINTMAX;

	using C_String		= typename const char *;
	using CW_String		= typename const wchar_t *;
	using C16_String	= typename const char16_t *;
	using C32_String	= typename const char32_t *;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using byte_t		= typename uint8_t;		// Byte Type
	using float_t		= typename float32_t;	// Float Type
	using hash_t		= typename uint64_t;	// Hash Type
	using intptr_t		= typename intmax_t;	// Int Pointer Type
	using ptrdiff_t		= typename intptr_t;	// Pointer Difference Type
	using size_t		= typename uintmax_t;	// Size Type

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	ML_USING_XY HashMap	= typename _STD unordered_map	<X, Y>;
	ML_USING_X	List	= typename _STD vector			<X>;
	ML_USING_XY Map		= typename _STD map				<X, Y>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_SERIALIZE	_STD ostream & operator<<
#define ML_DESERIALIZE	_STD istream & operator>>

	static _STD ostream & cout { _STD cout }; // Output Handle
	static _STD ostream & cerr { _STD cerr }; // Error Handle
	static _STD istream & cin  { _STD cin  }; // Input Handle

	template <
		class Ch, class Tr = typename _STD char_traits<char>
	> inline _STD basic_ostream<Ch, Tr> & endl(_STD basic_ostream<Ch, Tr> & out)
	{
		out.put(out.widen('\n'));
		out.flush();
		return out;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_STANDARD_LIB_HPP_