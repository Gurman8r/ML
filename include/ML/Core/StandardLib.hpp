#ifndef _ML_STANDARD_LIB_HPP_
#define _ML_STANDARD_LIB_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/Config.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <chrono>
#include <csignal>
#include <cstddef>
#include <ctime>
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

/* * * * * * * * * * * * * * * * * * * * */

# ifndef strdup
# define strdup _strdup // strdup is deprecated
# endif

/* * * * * * * * * * * * * * * * * * * * */

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
	using float80_t = typename long double;			// 10 bytes (only in some compilers)

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
	using int_t		= typename int32_t;				// Basic Signed Int Type
	using intptr_t	= typename intmax_t;			// Int Pointer Type
	using ptrdiff_t	= typename intmax_t;			// Pointer Difference Type
	using size_t	= typename uintmax_t;			// Size Type
	using time_t	= typename uint64_t;			// Time Type
	using uint_t	= typename uint32_t;			// Basic Unsigned Int Type

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class K, class V> using HashMap	= typename std::unordered_map	<K, V>;
	template <class K, class V> using Map		= typename std::map				<K, V>;
	template <class K, class V> using MultiMap	= typename std::multimap		<K, V>;
	template <class K, class V> using Pair		= typename std::pair			<K, V>;
	template <class T>			using HashSet	= typename std::unordered_set	<T>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Basic Ostream
	template <
		class Elem, class Traits
	> using BasicOstream = typename std::basic_ostream<Elem, Traits>;

	// Basic IstreamS
	template <
		class Elem, class Traits
	> using BasicIstream = typename std::basic_istream<Elem, Traits>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using streamoff		= typename int64_t;				// Stream Offset
	using streamsize	= typename int64_t;				// Stream Size
	using Ostream		= typename std::ostream;		// Output Stream
	using Istream		= typename std::istream;		// Input Stream
	using Ofstream		= typename std::ofstream;		// Output File Stream 
	using Ifstream		= typename std::ifstream;		// Input File Stream 
	using StreamBuf		= typename std::streambuf;		// Stream Buffer

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_SERIALIZE	_ML Ostream & operator<<
#define ML_DESERIALIZE	_ML Istream & operator>>

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static Ostream & cout	{ std::cout };
	static Ostream & cerr	{ std::cerr };
	static Istream & cin	{ std::cin	};

	template <
		class Elem, class Traits
	> inline BasicOstream<Elem, Traits> & endl(BasicOstream<Elem, Traits> & out)
	{
		// insert newline and flush stream
		out.put(out.widen('\n'));
		out.flush();
		return (out);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_STANDARD_LIB_HPP_