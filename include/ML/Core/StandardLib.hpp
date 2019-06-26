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
# define strdup _strdup
# endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	// Integral Types
	/* * * * * * * * * * * * * * * * * * * * */

	using int8_t	= typename signed char;			// 1 byte
	using int16_t	= typename signed short;		// 2 bytes
	using int32_t	= typename signed int;			// 4 bytes
	using int64_t	= typename signed long long;	// 8 bytes

	using uint8_t	= typename unsigned char;		// 1 byte
	using uint16_t	= typename unsigned short;		// 2 bytes
	using uint32_t	= typename unsigned int;		// 4 bytes
	using uint64_t	= typename unsigned long long;	// 8 bytes

	using float32_t	= typename float;				// 4 bytes
	using float64_t	= typename long double;			// 8 bytes

# if ML_x64
	using size_t	= typename uint64_t;			// Size Type (64-Bit)
	using intmax_t	= typename int64_t;				// Int Max Type (64-Bit)
# else
	using size_t	= typename uint32_t;			// Size Type (32-Bit)
	using intmax_t	= typename int32_t;				// Int Max Type (32-Bit)
# endif

	using byte_t	= typename uint8_t;				// Byte Type
	using float_t	= typename float32_t;			// Float Type
	using intptr_t	= typename intmax_t;			// Int Pointer Type
	using ptrdiff_t	= typename intmax_t;			// Pointer Diff Type
	using time_t	= typename uint64_t;			// Time Type

	/* * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	// Container Types
	/* * * * * * * * * * * * * * * * * * * * */

	template <class K, class V	> using HashMap		= typename std::unordered_map	<K, V>;
	template <class T			> using Initializer	= typename std::initializer_list<T>;
	template <class K, class V	> using Map			= typename std::map				<K, V>;
	template <class K, class V	> using MultiMap	= typename std::multimap		<K, V>;
	template <class K, class V	> using Pair		= typename std::pair			<K, V>;
	template <class T			> using HashSet		= typename std::unordered_set	<T>;

	/* * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	// Stream Types
	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class Elem, class Traits
	> using basic_ostream = typename std::basic_ostream<Elem, Traits>;

	template <
		class Elem, class Traits
	> using basic_istream = typename std::basic_istream<Elem, Traits>;

	using streamoff		= typename int64_t;				// Stream Offset
	using streamsize	= typename int64_t;				// Stream Size
	using ostream		= typename std::ostream;		// Output Stream
	using istream		= typename std::istream;		// Input Stream
	using ofstream		= typename std::ofstream;		// Output File Stream 
	using ifstream		= typename std::ifstream;		// Input File Stream 
	using streambuf		= typename std::streambuf;		// Stream Buffer

	static ostream & cout	{ std::cout };
	static ostream & cerr	{ std::cerr };
	static istream & cin	{ std::cin	};

	template <
		class Elem, class Traits
	> inline basic_ostream<Elem, Traits> & endl(basic_ostream<Elem, Traits> & out)
	{
		// insert newline and flush stream
		out.put(out.widen('\n'));
		out.flush();
		return (out);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_SERIALIZE	ml::ostream & operator<<
#define ML_DESERIALIZE	ml::istream & operator>>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_STANDARD_LIB_HPP_