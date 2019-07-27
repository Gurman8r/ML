#ifndef _ML_STANDARD_LIB_HPP_
#define _ML_STANDARD_LIB_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/Config.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <chrono>
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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

# ifndef strdup
# define strdup _strdup // strdup is deprecated
# endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using int8_t	= typename signed char;			// 1 byte
	using int16_t	= typename signed short;		// 2 bytes
	using int32_t	= typename signed int;			// 4 bytes
	using int64_t	= typename signed long long;	// 8 bytes

	using uint8_t	= typename unsigned char;		// 1 byte
	using uint16_t	= typename unsigned short;		// 2 bytes
	using uint32_t	= typename unsigned int;		// 4 bytes
	using uint64_t	= typename unsigned long long;	// 8 bytes

	using float32_t = typename float;				// 4 bytes
	using float64_t = typename double;				// 8 bytes
	using float80_t = typename long double;			// 8 or 10 bytes (Compiler dependant)

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if not ML_x64
	using intmax_t	= typename int32_t;				// Max Signed	(32-Bit)
	using uintmax_t = typename uint32_t;			// Max Unsigned	(32-Bit)
#else
	using intmax_t	= typename int64_t;				// Max Signed	(64-Bit)
	using uintmax_t	= typename uint64_t;			// Max Unsigned	(64-Bit)
#endif

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using byte_t	= typename uint8_t;				// Byte Type
	using float_t	= typename float32_t;			// Float Type
	using hash_t	= typename uintmax_t;			// Hash Type
	using intptr_t	= typename intmax_t;			// Int Pointer Type
	using ptrdiff_t	= typename intmax_t;			// Pointer Difference Type
	using size_t	= typename uintmax_t;			// Size Type

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	using		Ifstream		= typename std::ifstream;
	using		Istream			= typename std::istream;
	using		Ofstream		= typename std::ofstream;
	using		Ostream			= typename std::ostream;
	using		StreamBuf		= typename std::streambuf;
	ML_USING_X	Allocator		= typename std::allocator<X>;
	ML_USING_X	CharTraits		= typename std::char_traits<X>;
	ML_USING_XY HashSet			= typename std::unordered_set<X, Y>;
	ML_USING_XY BasicIstream	= typename std::basic_istream<X, Y>;
	ML_USING_XY BasicOstream	= typename std::basic_ostream<X, Y>;
	ML_USING_XY HashMap			= typename std::unordered_map<X, Y>;
	ML_USING_XY MultiMap		= typename std::multimap<X, Y>;
	ML_USING_XY Pair			= typename std::pair<X, Y>;
	ML_USING_XY Tree			= typename std::map<X, Y>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static Ostream & cout { _STD cout }; // Standard Output Handle
	static Ostream & cerr { _STD cerr }; // Standard Error Handle
	static Istream & cin  { _STD cin  }; // Standard Input Handle

#define ML_SERIALIZE	_ML Ostream & operator<< // Stream Insertion Operator
#define ML_DESERIALIZE	_ML Istream & operator>> // Stream Extraction Operator

	template <
		class Ch, class Traits = typename CharTraits<Ch>
	> inline BasicOstream<Ch, Traits> & endl(BasicOstream<Ch, Traits> & out)
	{
		// insert newline and flush stream
		out.put(out.widen('\n'));
		out.flush();
		return out;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_STANDARD_LIB_HPP_