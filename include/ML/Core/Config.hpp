#ifndef _ML_CONFIG_HPP_
#define _ML_CONFIG_HPP_


// Project Information
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_PROJECT_AUTH	"Melody Gurman"
#define ML_PROJECT_NAME	"MemeLib"
#define ML_PROJECT_VER	"Beta"
#define ML_PROJECT_URL	"https://www.github.com/Gurman8r/ML"
#define ML_PROJECT_DATE	__DATE__
#define ML_PROJECT_TIME	__TIME__


// Language
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(__cplusplus)
#	if defined(_MSVC_LANG)
#		define ML_CPP _MSVC_LANG
#	else
#		define ML_CPP __cplusplus
#	endif
#	if (ML_CPP >= 201103L) 
#		define ML_HAS_CXX11 // C++11
#	endif
#	if (ML_CPP >= 201402L) 
#		define ML_HAS_CXX14 // C++14 
#	endif
#	if (ML_CPP >= 201703L)
#		define ML_HAS_CXX17 // C++17
#	endif
#	if (ML_CPP >= 201907L)
#		define ML_HAS_CXX20	// C++20
#	endif
#else
#	error This system does not support C++.
#endif

#if defined(__cpp_constexpr)
#	define ML_CPP_CONSTEXPR __cpp_constexpr
#	if (ML_CPP_CONSTEXPR >= 201907L)
#		define ML_HAS_CONSTEXPR_20	// trivial default initialization and asm-declaration in constexpr functions
#	endif
#	if (ML_CPP_CONSTEXPR >= 201603L)
#		define ML_HAS_CONSTEXPR_17	// constexpr lambda
#	endif
#	if (ML_CPP_CONSTEXPR >= 201304L)
#		define ML_HAS_CONSTEXPR_14	// relaxing constraints on constexpr functions / constexpr member functions and implicit const
#	endif
#	if (ML_CPP_CONSTEXPR >= 200704L)
#		define ML_HAS_CONSTEXPR_11	// constexpr
#	endif
#else
#	error This compiler does not support constexpr.
#endif


// System
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(_WIN32) || defined(_WIN64) \
  || defined(WIN32) || defined(WIN64) \
  || defined(__MINGW32__) || defined(__MINGW64__)
#	define ML_SYSTEM_WINDOWS
#	define ML_SYSTEM_NAME "Windows"
#elif defined(__APPLE__) && defined(__MACH__)
#	define ML_SYSTEM_APPLE
#	define ML_SYSTEM_NAME "Apple"
#elif defined(__unix__)
#	define ML_SYSTEM_UNIX
#	if defined(__ANDROID__)
#		define ML_SYSTEM_ANDROID
#		define ML_SYSTEM_NAME "Android"
#	elif defined(__linux__)
#		define ML_SYSTEM_LINUX
#		define ML_SYSTEM_NAME "Linux"
#	elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
#		define ML_SYSTEM_FREEBSD
#		define ML_SYSTEM_NAME "FreeBSD"
#	endif
#else
#	error This system is not supported.
#endif


// Platform / Architecture
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(__x86_64__) || defined(_M_X64) || defined(_x64)
#	define ML_X64
#	define ML_ARCHITECTURE 64
#	define ML_PLATFORM_TARGET "x64"
#elif defined(__i386__) || defined(_M_IX86)
#	define ML_X86
#	define ML_ARCHITECTURE 32
#	define ML_PLATFORM_TARGET "x86"
#elif defined(__arm__) || defined(_M_ARM) || defined(__aarch64__)
#	if defined(__aarch64__)
#		define ML_ARM64
#		define ML_ARCHITECTURE 64
#		define ML_PLATFORM_TARGET "arm64"
#	else
#		define ML_ARM32
#		define ML_ARCHITECTURE 32
#		define ML_PLATFORM_TARGET "arm32"
#	endif
#elif defined(ppc) || defined(_M_PPC) || defined(__ppc64__)
#	if defined(__ppc64__)
#		define ML_PPC64
#		define ML_ARCHITECTURE 64
#		define ML_PLATFORM_TARGET "ppc64"
#	else
#		define ML_PPC32
#		define ML_ARCHITECTURE 32
#		define ML_PLATFORM_TARGET "ppc32"
#	endif
#else
#	error Unable to detect platform architecture.
#endif


// Compiler
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(_MSC_VER)
#	define ML_CC_MSC _MSC_VER
#	define ML_CC_VER ML_CC_MSC
#	if (ML_CC_VER >= 1920)
#	define ML_CC_NAME "Visual Studio 2019"
#	elif (ML_CC_VER >= 1910)
#	define ML_CC_NAME "Visual Studio 2017"
#	elif (ML_CC_VER >= 1900)
#	define ML_CC_NAME "Visual Studio 2015"
#	elif (ML_CC_VER >= 1800)
#	define ML_CC_NAME "Visual Studio 2013"
#	else
#		error This version of Visual Studio is not supported.
#	endif
#elif defined(__clang__)
#	define ML_CC_CLANG __clang__
#	define ML_CC_VER ML_CC_CLANG
#	define ML_CC_NAME "Clang/LLVM"
#elif defined(__GNUC__) || defined(__GNUG__)
#	if defined(__GNUC__)
#		define ML_CC_GCC __GNUC__
#	else
#		define ML_CC_GCC __GNUG__
#	endif
#	define ML_CC_VER ML_CC_GCC
#	define ML_CC_NAME "GCC"
#elif defined(__ICC) || defined(__INTEL_COMPILER)
#	if defined(__ICC)
#		define ML_CC_INTEL __ICC
#	else
#		define ML_CC_INTEL __INTEL_COMPILER
#	endif
#	define ML_CC_VER ML_CC_INTEL
#	define ML_CC_NAME "Intel"
#elif defined(__EMSCRIPTEN__)
#	define ML_CC_EMSCRIPTEN
#	define ML_CC_VER ML_CC_EMSCRIPTEN
#	define ML_CC_NAME "Emscripten"
#elif defined(__MINGW32__) || defined(__MINGW64__)
#	if defined(__MINGW64__)
#		define ML_CC_MINGW __MINGW64__
#	else
#		define ML_CC_MINGW __MINGW32__
#	endif
#	define ML_CC_VER ML_CC_MINGW
#	define ML_CC_NAME "MinGW"
#elif defined(__asmjs__)
#	define ML_CC_ASMJS
#	define ML_CC_VER ML_CC_ASMJS
#	define ML_CC_NAME "asm.js"
#elif defined(__wasm__)
#	define ML_CC_WASM
#	define ML_CC_VER ML_CC_WASM
#	define ML_CC_NAME "WebAssembly"
#else
#	error This compiler is not supported.
#endif


// Types
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(ML_CC_MSC)
#	define	ML_INT8		signed __int8
#	define	ML_INT16	signed __int16
#	define	ML_INT32	signed __int32
#	define	ML_INT64	signed __int64
#	define	ML_UINT8	unsigned __int8
#	define	ML_UINT16	unsigned __int16
#	define	ML_UINT32	unsigned __int32
#	define	ML_UINT64	unsigned __int64
#else
#	define	ML_INT8		signed char
#	define	ML_INT16	signed short
#	define	ML_INT32	signed int
#	define	ML_INT64	signed long long
#	define	ML_UINT8	unsigned char
#	define	ML_UINT16	unsigned short
#	define	ML_UINT32	unsigned int
#	define	ML_UINT64	unsigned long long
#endif

#define	ML_FLOAT32	float
#define	ML_FLOAT64	double
#define	ML_FLOAT80	long double // 8, 10, 12, or 16 bytes (CC Dependant)

#if (ML_ARCHITECTURE == 32)
#	define ML_INTMAX	ML_INT32
#	define ML_UINTMAX	ML_UINT32
#else
#	define ML_INTMAX	ML_INT64
#	define ML_UINTMAX	ML_UINT64
#endif


// Preprocessor
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Build
#if defined(_DEBUG)
#	define ML_DEBUG			1
#	define ML_CONFIGURATION	"Debug"
#else
#	define ML_DEBUG			0
#	define ML_CONFIGURATION	"Release"
#endif

// Utility
#define _ML						::ml::
#define ML_ADDRESSOF(ptr)		((void *)(ML_INTMAX)ptr)
#define ML_ARRAYSIZE(arr)		(sizeof(arr) / sizeof(*arr))
#define ML_CONCAT_IMPL(a, b)	a##b
#define ML_CONCAT(a, b)			ML_CONCAT_IMPL(a, b)
#define ML_TOSTRING(str)		#str
#define ML_STRINGIFY(str)		ML_TOSTRING(str)
#define ML_TRUE_EXPR(expr)		(([&](){ expr; return true; })())
#define ML_FALSE_EXPR(expr)		(([&](){ expr; return false; })())

// Typedefs
#define ML_USING				using
#define ML_USING_T_(...)		template <##__VA_ARGS__> ML_USING
#define ML_USING_X				ML_USING_T_(class X)
#define ML_USING_XY				ML_USING_T_(class X, class Y)
#define ML_USING_XYZ			ML_USING_T_(class X, class Y, class Z)
#define ML_USING_Ts				ML_USING_T_(class ... Ts)

// Anonymous ( _str_# )
#ifdef __COUNTER__
#	define ML_ANON(str) ML_CONCAT(_, ML_CONCAT(str, ML_CONCAT(_, __COUNTER__)))
#else
#	define ML_ANON(str) ML_CONCAT(_, ML_CONCAT(str, ML_CONCAT(_, __LINE__)))
#endif

#define ML_ANON_T(T, ...) auto ML_ANON(T) { T { ##__VA_ARGS__ } }

// Inlining
#ifdef ML_CC_MSC
#	define ML_ALWAYS_INLINE __forceinline
#	define ML_NEVER_INLINE __declspec(noinline)
#elif defined(ML_CC_GCC) || defined(ML_CC_CLANG)
#	define ML_ALWAYS_INLINE inline __attribute__((__always_inline__))
#	define ML_NEVER_INLINE __attribute__((__noinline__))
#else
#	define ML_ALWAYS_INLINE
#	define ML_NEVER_INLINE
#endif

// Export
#ifdef ML_CC_MSC
#	define ML_API_EXPORT __declspec(dllexport)
#	define ML_API_IMPORT __declspec(dllimport)
#elif ((defined(ML_CC_GCC) || defined(ML_CC_CLANG)) && (ML_CC_VER >= 4))
#	define ML_API_EXPORT __attribute__ ((__visibility__ ("default")))
#	define ML_API_IMPORT __attribute__ ((__visibility__ ("default")))
#else
#	define ML_API_EXPORT
#	define ML_API_IMPORT
#endif


// Warnings
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef ML_CC_MSC
#	pragma warning(disable: 4031)	// second formal parameter list longer than the first list
#	pragma warning(disable: 4067)	// unexpected tokens following preprocessor directive - expected a newline
#	pragma warning(disable: 4251)	// type1 needs to have dll-interface to be used by type2
#	pragma warning(disable: 4307)	// integral constant overflow
#	pragma warning(disable: 4308)	// negative integral constant converted to unsigned type
#	pragma warning(disable: 4309)	// truncation of constant value
#	pragma warning(disable: 4723)	// potential divide by zero
#	pragma warning(disable: 6282)	// incorrect operator
#	pragma warning(disable: 6301)	// return value ignored
#	pragma warning(disable: 26437)	// do not slice
#	pragma warning(disable: 26451)	// arithmetic overflow
#	pragma warning(disable: 26495)	// value may be uninitialized
#	pragma warning(disable: 26812)	// unscoped enum
#endif

#endif // !_ML_CONFIG_HPP_