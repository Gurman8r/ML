#ifndef _ML_STANDARD_LIB_HPP_
#define _ML_STANDARD_LIB_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/Config.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <chrono>
#include <codecvt>
#include <csignal>
#include <cstddef>
#include <ctime>
#include <experimental/filesystem>
#include <fstream>
#include <future>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <locale>
#include <map>
#include <mutex>
#include <queue>
#include <random>
#include <regex>
#include <stack>
#include <sstream>
#include <stdarg.h>
#include <stdint.h>
#include <string>
#include <thread>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_ARGC	__argc	// Pointer to number of command line arguments
#define ML_ARGV	__argv	// Pointer to table of narrow command line arguments

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

# ifdef ML_SYSTEM_WINDOWS
#	define popen	_popen
#	define pclose	_pclose
#	define strdup	_strdup
#	define snprintf	_snprintf
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
	using CU16_String	= typename const char16_t *;
	using CU32_String	= typename const char32_t *;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using float_t	= typename float32_t;	// Float Type
	using hash_t	= typename uint64_t;	// Hash Type
	using intptr_t	= typename intmax_t;	// Int Pointer Type
	using ptrdiff_t	= typename intmax_t;	// Pointer Difference Type
	using size_t	= typename uintmax_t;	// Size Type
	using time_t	= typename uint64_t;	// Time Type

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	ML_USING_XY HashMap			= typename _STD unordered_map	<X, Y>;
	ML_USING_XY HashSet			= typename _STD unordered_set	<X, Y>;
	ML_USING_XY MultiMap		= typename _STD multimap		<X, Y>;
	ML_USING_XY Pair			= typename _STD pair			<X, Y>;
	ML_USING_XY Tree			= typename _STD map				<X, Y>;
	ML_USING_X	BasicIstream	= typename _STD basic_istream	<X, _STD char_traits<X>>;
	ML_USING_X	BasicOstream	= typename _STD basic_ostream	<X, _STD char_traits<X>>;
	ML_USING_X	BasicIfstream	= typename _STD basic_ifstream	<X, _STD char_traits<X>>;
	ML_USING_X	BasicOfstream	= typename _STD basic_ofstream	<X, _STD char_traits<X>>;
	ML_USING_X	BasicStreamBuf	= typename _STD basic_streambuf	<X, _STD char_traits<X>>;
	using		Ofstream		= typename		BasicOfstream	<char>;
	using		Ifstream		= typename		BasicIfstream	<char>;
	using		Ostream			= typename		BasicOstream	<char>;
	using		Istream			= typename		BasicIstream	<char>;
	using		StreamBuf		= typename		BasicStreamBuf	<char>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_SERIALIZE	::ml::Ostream & operator<<
#define ML_DESERIALIZE	::ml::Istream & operator>>

	static Ostream & cout { _STD cout }; // Output Handle
	static Ostream & cerr { _STD cerr }; // Error Handle
	static Istream & cin  { _STD cin  }; // Input Handle

	template <class Ch> inline BasicOstream<Ch> & endl(BasicOstream<Ch> & out)
	{
		// insert newline and flush stream
		out.put(out.widen('\n'));
		out.flush();
		return out;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_STANDARD_LIB_HPP_