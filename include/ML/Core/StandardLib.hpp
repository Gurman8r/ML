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
	// Numeric Types
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using int8_t	= typename signed char;				// 1 byte
	using int16_t	= typename signed short;			// 2 bytes
	using int32_t	= typename signed int;				// 4 bytes
	using int64_t	= typename signed long long;		// 8 bytes

	using uint8_t	= typename unsigned char;			// 1 byte
	using uint16_t	= typename unsigned short;			// 2 bytes
	using uint32_t	= typename unsigned int;			// 4 bytes
	using uint64_t	= typename unsigned long long;		// 8 bytes

	using float32_t = typename float;					// 4 bytes
	using float64_t = typename double;					// 8 bytes
	using float80_t = typename long double;				// 8 or 10 bytes (CC dependant)

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if not ML_x64
	using intmax_t	= typename int32_t;					// Max Signed	(32-Bit)
	using uintmax_t = typename uint32_t;				// Max Unsigned	(32-Bit)
#else
	using intmax_t	= typename int64_t;					// Max Signed	(64-Bit)
	using uintmax_t	= typename uint64_t;				// Max Unsigned	(64-Bit)
#endif

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using byte_t	= typename uint8_t;					// Byte Type
	using float_t	= typename float32_t;				// Float Type
	using hash_t	= typename uintmax_t;				// Hash Type
	using intptr_t	= typename intmax_t;				// Int Pointer Type
	using ptrdiff_t	= typename intmax_t;				// Pointer Difference Type
	using size_t	= typename uintmax_t;				// Size Type

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ml
{
	// Standard Types
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_GEN_STD_TYPE(NAME, BASE) \
	using NAME = typename _STD BASE;

#define ML_GEN_STD_TYPE_X(NAME, BASE) \
	template <class X> using NAME = typename _STD BASE<X>

#define ML_GEN_STD_TYPE_XY(NAME, BASE) \
	template <class X, class Y> using NAME = typename _STD BASE<X, Y>

#define ML_GEN_STD_TYPE_XYZ(NAME, BASE) \
	template <class X, class Y, class Z> using NAME = typename _STD BASE<X, Y, Z>

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_GEN_STD_TYPE_X	(Allocator,			allocator);			// Allocator
	ML_GEN_STD_TYPE_XY	(BasicIstream,		basic_istream);		// Basic Output Stream
	ML_GEN_STD_TYPE_XY	(BasicOstream,		basic_ostream);		// Basic Input Stream
	ML_GEN_STD_TYPE_X	(CharTraits,		char_traits);		// Character Traits
	ML_GEN_STD_TYPE_XY	(HashMap,			unordered_map);		// Hash Map
	ML_GEN_STD_TYPE_X	(HashSet,			unordered_set);		// Hash Set
	ML_GEN_STD_TYPE		(Ifstream,			ifstream);			// Input File Stream 
	ML_GEN_STD_TYPE_X	(Initializer,		initializer_list);	// Initializer List
	ML_GEN_STD_TYPE		(Istream,			istream);			// Input Stream
	ML_GEN_STD_TYPE_XY	(Map,				map);				// Map
	ML_GEN_STD_TYPE_XY	(MultiMap,			multimap);			// Multimap
	ML_GEN_STD_TYPE		(Ofstream,			ofstream);			// Output File Stream 
	ML_GEN_STD_TYPE		(Ostream,			ostream);			// Output Stream
	ML_GEN_STD_TYPE_XY	(Pair,				pair);				// Pair
	ML_GEN_STD_TYPE		(StreamBuf,			streambuf);			// Stream Buffer

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	// IO
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static Ostream & cout { _STD cout }; // Standard Output Handle
	static Ostream & cerr { _STD cerr }; // Standard Error Handle
	static Istream & cin  { _STD cin  }; // Standard Input Handle
	
	template <
		class Elem, class Traits = typename CharTraits<Elem>
	> inline BasicOstream<Elem, Traits> & endl(BasicOstream<Elem, Traits> & out)
	{
		// insert newline and flush stream
		out.put(out.widen('\n'));
		out.flush();
		return out;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_SERIALIZE	_ML Ostream & operator<< // Ostream Insertion Operator
#define ML_DESERIALIZE	_ML Istream & operator>> // IStream Extraction Operator

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_STANDARD_LIB_HPP_