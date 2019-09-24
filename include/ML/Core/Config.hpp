#ifndef _ML_CONFIG_HPP_
#define _ML_CONFIG_HPP_

//	C++ Version
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


//	Configuration (Debug / Release)
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

# if defined(_DEBUG)
#	define ML_DEBUG			true
#	define ML_CONFIGURATION	"Debug"
# else
#	define ML_DEBUG			false
#	define ML_CONFIGURATION	"Release"
# endif


//	Operating System
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

# if defined(_WIN32) || defined(_WIN64) \
  || defined(WIN32) || defined(WIN64) \
  || defined(__MINGW32__) || defined(__MINGW64__)
#	define ML_SYSTEM_WINDOWS
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


//	Architecture
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
# else
#	error This compiler does not support memes.
# endif


// Types
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_INT8			signed char			// 1 byte
#define ML_INT16		signed short		// 2 bytes
#define ML_INT32		signed int			// 4 bytes
#define ML_INT64		signed long long	// 8 bytes

#define ML_UINT8		unsigned char		// 1 byte
#define ML_UINT16		unsigned short		// 2 bytes
#define ML_UINT32		unsigned int		// 4 bytes
#define ML_UINT64		unsigned long long	// 8 bytes

#define ML_FLOAT32		float				// 4 bytes
#define ML_FLOAT64		double				// 8 bytes
#define ML_FLOAT80		long double			// 8 or 10 bytes (CC dependant)

# if (ML_ARCHITECTURE == 32)
#	define ML_INTMAX	ML_INT32			// Max Signed	(32-Bit)
#	define ML_UINTMAX	ML_UINT32			// Max Unsigned	(32-Bit)
# else
#	define ML_INTMAX	ML_INT64			// Max Signed	(64-Bit)
#	define ML_UINTMAX	ML_UINT64			// Max Unsigned	(64-Bit)
# endif


//	Preprocessor
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_ADDRESSOF(ptr)	((void *)(ML_INTMAX)ptr)
#define ML_ARRAYSIZE(arr)	(sizeof(arr) / sizeof(*arr))
#define ML_CONCAT(a, b)		a##b
#define ML_STRINGIFY(str)	ML_TOSTRING(str)
#define ML_TOSTRING(str)	#str

#define ML_TEMPLATE(...)	template<##__VA_ARGS__>
#define ML_USING_VA(...)	ML_TEMPLATE(##__VA_ARGS__) using
#define ML_USING_X			ML_USING_VA(class X)
#define ML_USING_XY			ML_USING_VA(class X, class Y)
#define ML_USING_XYZ		ML_USING_VA(class X, class Y, class Z)

#define ML_TRUE_EXPR(expr)	(([&](){ expr; return true; })())
#define ML_FALSE_EXPR(expr)	(([&](){ expr; return false; })())


//	Version Information
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_PROJECT_AUTH	"Melody Gurman"
#define ML_PROJECT_NAME "MemeLib"
#define ML_PROJECT_VER	"Alpha"
#define ML_PROJECT_URL	"https://www.github.com/Gurman8r/ML"
#define ML_PROJECT_DATE	__DATE__
#define ML_PROJECT_TIME	__TIME__

// N-V (C / P) was created by A on D at T
#define ML_GET_VERSION_STRING(N, V, C, P, A, D, T) \
"" N "-" V " (" C "/" P ") was created by " A " on " D " at " T ""

#define MEMELIB_VERSION ML_GET_VERSION_STRING( \
	ML_PROJECT_NAME, \
	ML_PROJECT_VER, \
	ML_CONFIGURATION, \
	ML_PLATFORM_TARGET, \
	ML_PROJECT_AUTH, \
	ML_PROJECT_DATE, \
	ML_PROJECT_TIME \
)


//	Export / Import
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

# if not defined(ML_STATIC)
#	if defined(ML_SYSTEM_WINDOWS)
#		define ML_API_EXPORT __declspec(dllexport)
#		define ML_API_IMPORT __declspec(dllimport)
#		if not defined(NOMINMAX)
#			define NOMINMAX
#		endif
#		if defined(ML_CC_MSC)
#			pragma warning(disable: 4031) // second formal parameter list longer than the first list
#			pragma warning(disable: 4099) // PDB was not found
#			pragma warning(disable: 4251) // type1 needs to have dll-interface to be used by type2
#			pragma warning(disable: 4307) // integral constant overflow
#			pragma warning(disable: 4309) // truncation of constant value
#			pragma warning(disable: 4723) // potential divide by zero
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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

# endif // !_ML_CONFIG_HPP_