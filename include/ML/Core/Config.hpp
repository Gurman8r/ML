#ifndef _ML_CONFIG_HPP_
#define _ML_CONFIG_HPP_


//	Project Information
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

# define ML_PROJECT_AUTH	"Melody Gurman"
# define ML_PROJECT_NAME	"MemeLib"
# define ML_PROJECT_VER		"Alpha"
# define ML_PROJECT_URL		"https://www.github.com/Gurman8r/ML"
# define ML_PROJECT_DATE	__DATE__
# define ML_PROJECT_TIME	__TIME__


//	Language
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

# if defined(__cplusplus)
#	if defined(_MSVC_LANG)
#		define ML_CPLUSPLUS _MSVC_LANG
#	else
#		define ML_CPLUSPLUS __cplusplus
#	endif
#	define ML_HAS_CXX11 (ML_CPLUSPLUS >= 201103L) // 11
#	define ML_HAS_CXX14 (ML_CPLUSPLUS >= 201402L) // 14
#	define ML_HAS_CXX17 (ML_CPLUSPLUS >= 201703L) // 17
# else
#	error This system does not support C++
# endif


//	Configuration
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

# if defined(_DEBUG)
#	define ML_DEBUG			true
#	define ML_CONFIGURATION	"Debug"
# else
#	define ML_DEBUG			false
#	define ML_CONFIGURATION	"Release"
# endif


//	System
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

# if defined(_WIN32) || defined(_WIN64) \
  || defined(WIN32) || defined(WIN64) \
  || defined(__MINGW32__) || defined(__MINGW64__)
#	define ML_SYSTEM_WINDOWS
#	ifndef NOMINMAX
#		define NOMINMAX
#	endif
# elif defined(__APPLE__) && defined(__MACH__)
#	define ML_SYSTEM_APPLE
# elif defined(__unix__)
#	define ML_SYSTEM_UNIX
#	if defined(__ANDROID__)
#		define ML_SYSTEM_ANDROID
#	elif defined(__linux__)
#		define ML_SYSTEM_LINUX
#	elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)	
#		define ML_SYSTEM_FREEBSD
#	endif
# endif

# if defined(ML_SYSTEM_WINDOWS)
#	define ML_SYSTEM_NAME		 "Windows"
# elif defined(ML_SYSTEM_APPLE)
#	define ML_SYSTEM_NAME		 "Apple"
# elif defined(ML_SYSTEM_ANDROID)
#	define ML_SYSTEM_NAME		 "Android"
# elif defined(ML_SYSTEM_LINUX)
#	define ML_SYSTEM_NAME		 "Linux"
# elif defined(ML_SYSTEM_FREEBSD)
#	define ML_SYSTEM_NAME		 "FreeBSD"
# else
#	error This operating system does not support memes.
# endif


//	Platform
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

# if defined(_WIN64		) \
  || defined(WIN64		) \
  || defined(__x86_64__	) \
  || defined(__ppc64__	) \
  || defined(_x64		) \
  || defined(_M_X64		) \
  || defined(__MINGW64__)
#	define ML_ARCHITECTURE		64
#	define ML_PLATFORM_TARGET	"x64"
# else
#	define ML_ARCHITECTURE		32
#	define ML_PLATFORM_TARGET	"x86"
# endif


//	Compiler
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

# if defined(_MSC_VER)
#	define ML_CC_MSC _MSC_VER
# elif defined(__clang__)
#	define ML_CC_CLANG __clang__
# elif defined(__GNUC__) || defined(__GNUG__)
#	if defined(__GNUC__)
#		define ML_CC_GNU __GNUC__
#	else
#		define ML_CC_GNU __GNUG__
#	endif
# elif defined(__ICC) || defined(__INTEL_COMPILER)
#	if defined(__ICC)
#		define ML_CC_INTEL __ICC
#	else
#		define ML_CC_INTEL __INTEL_COMPILER
#	endif
# elif defined(__EMSCRIPTEN__)
#	define ML_CC_EMSCRIPTEN
# endif

# if defined(ML_CC_MSC)
#	define ML_CC_NAME		"Microsoft"
#	define ML_CC_VER		ML_CC_MSC
# elif defined(ML_CC_CLANG)
#	define ML_CC_NAME		"Clang"
#	define ML_CC_VER		ML_CC_CLANG
# elif defined(ML_CC_GNU)
#	define ML_CC_NAME		"GNU"
#	define ML_CC_VER		ML_CC_GNU
# elif defined(ML_CC_INTEL)
#	define ML_CC_NAME		"Intel"
#	define ML_CC_VER		ML_CC_INTEL
# elif defined(ML_CC_EMSCRIPTEN)
#	define ML_CC_NAME		"Emscripten"
#	define ML_CC_VER		ML_CC_EMSCRIPTEN
# else
#	error This compiler does not support memes.
# endif


// Types
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

# define	ML_BOOL		bool
# define	ML_CHAR		char
# define	ML_WCHAR	wchar_t
# define	ML_CHAR16	char16_t
# define	ML_CHAR32	char32_t

# ifdef ML_CC_MSC
# define	ML_INT8		signed __int8
# define	ML_INT16	signed __int16
# define	ML_INT32	signed __int32
# define	ML_INT64	signed __int64
# define	ML_UINT8	unsigned __int8
# define	ML_UINT16	unsigned __int16
# define	ML_UINT32	unsigned __int32
# define	ML_UINT64	unsigned __int64
# else
# define	ML_INT8		signed char
# define	ML_INT16	signed short
# define	ML_INT32	signed int
# define	ML_INT64	signed long long
# define	ML_UINT8	unsigned char
# define	ML_UINT16	unsigned short
# define	ML_UINT32	unsigned int
# define	ML_UINT64	unsigned long long
# endif

# define	ML_FLOAT32	float
# define	ML_FLOAT64	double
# define	ML_FLOAT80	long double // 8, 10, 12, or 16 bytes (CC Dependant)

# if (ML_ARCHITECTURE == 32)
#	define ML_INTMAX	ML_INT32
#	define ML_UINTMAX	ML_UINT32
# else
#	define ML_INTMAX	ML_INT64
#	define ML_UINTMAX	ML_UINT64
# endif


//	Preprocessor
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

# define _ML ::ml::

# define ML_ADDRESSOF(ptr)		((void *)(ML_INTMAX)ptr)
# define ML_ARRAYSIZE(arr)		(sizeof(arr) / sizeof(*arr))
# define ML_CONCAT(a, b)		a##b
# define ML_STRINGIFY(str)		ML_TOSTRING(str)
# define ML_TOSTRING(str)		#str

# define ML_TEMPLATE(...)		template<##__VA_ARGS__>
# define ML_USING_VA(...)		ML_TEMPLATE(##__VA_ARGS__) using
# define ML_USING_X				ML_USING_VA(class X)
# define ML_USING_XY			ML_USING_VA(class X, class Y)
# define ML_USING_XYZ			ML_USING_VA(class X, class Y, class Z)
# define ML_USING_TS			ML_USING_VA(class ... Ts)

# define ML_TRUE_EXPR(expr)		(([&](){ expr; return true; })())
# define ML_FALSE_EXPR(expr)	(([&](){ expr; return false; })())


//	Build
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

# ifndef ML_STATIC
#	if defined(ML_SYSTEM_WINDOWS)
#		define ML_API_EXPORT __declspec(dllexport)
#		define ML_API_IMPORT __declspec(dllimport)
#		if defined(ML_CC_MSC)
#			pragma warning(disable: 4031)	// second formal parameter list longer than the first list
#			pragma warning(disable: 4067)	// unexpected tokens following preprocessor directive - expected a newline
#			pragma warning(disable: 4251)	// type1 needs to have dll-interface to be used by type2
#			pragma warning(disable: 4307)	// integral constant overflow
#			pragma warning(disable: 4308)	// negative integral constant converted to unsigned type
#			pragma warning(disable: 4309)	// truncation of constant value
#			pragma warning(disable: 4723)	// potential divide by zero
#			pragma warning(disable: 6282)	// incorrect operator
#			pragma warning(disable: 6301)	// return value ignored
#			pragma warning(disable: 26437)	// do not slice
#			pragma warning(disable: 26451)	// arithmetic overflow
#			pragma warning(disable: 26495)	// value may be uninitialized
#			pragma warning(disable: 26812)	// unscoped enum
#		endif
#	elif defined(ML_CC_GNU)
#		if ML_CC_GNU >= 4
#			define ML_API_EXPORT __attribute__ ((__visibility__ ("default")))
#			define ML_API_IMPORT __attribute__ ((__visibility__ ("default")))
#		else
#			define ML_API_EXPORT
#			define ML_API_IMPORT
#		endif
#	endif
# else
#	define ML_API_EXPORT
#	define ML_API_IMPORT
# endif


# endif // !_ML_CONFIG_HPP_