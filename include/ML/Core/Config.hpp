#ifndef _ML_CONFIG_HPP_
#define _ML_CONFIG_HPP_

// Project Info
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ML_PROJECT_VER "1.0.0"
#define ML_PROJECT_URL "https://www.github.com/Gurman8r/ML"

// Namespace
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define _ML ::ml::

// Stringify
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ML_xstr(l) ML_str(l)
#define ML_str(l) #l

//	C/C++
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
# if defined(__cplusplus)
#	define ML_CPP
# endif

//	Variable Templates
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
# if defined(__cpp_variable_templates)
#	define ML_HAS_VARIABLE_TEMPLATES
# endif

//	Configuration (Debug/Release)
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
# if defined(_DEBUG)
#	define ML_DEBUG			1
#	define ML_CONFIGURATION "Debug"
# else
#	define ML_DEBUG			0
#	define ML_CONFIGURATION "Release"
# endif

//	Operating System
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
# if defined(_WIN32		) \
  || defined(_WIN64		) \
  || defined(WIN32		) \
  || defined(WIN64		) \
  || defined(__WIN32__	) \
  || defined(__MINGW32__)
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
#	else
#		error This unix operating system does not support memes.
#	endif
# else
#	error This operating system does not support memes.
# endif

//	Architecture / Platform Target
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
# if defined(_WIN64		)\
  || defined(WIN64		)\
  || defined(__x86_64__	)\
  || defined(__ppc64__	)\
  || defined(_x64		)\
  || defined(_M_X64		)
#	define ML_x32 1
#	define ML_x64 1
#	define ML_ARCHITECTURE 64
#	define ML_PLATFORM_TARGET "x64"
# else
#	define ML_x32 1
#	define ML_x64 0
#	define ML_ARCHITECTURE 32
#	define ML_PLATFORM_TARGET "x86"
# endif

//	Compiler
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
# if defined(_MSC_VER)
//	Microsoft Compiler
#	define ML_CC_NAME	"Microsoft"
#	define ML_CC_MSC	_MSC_VER
# elif defined(__clang__)
//	Clang Compiler
#	define ML_CC_NAME	"Clang"
#	define ML_CC_CLANG	__clang_
# elif defined(__GNUC__)
//	GNU Compiler
#	define ML_CC_NAME	"GNU"
#	define ML_CC_GNUC	__GNUC__
#else
#	error This compiler does not support memes.
# endif

//	Export / Import
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
# ifndef ML_STATIC
#	ifdef ML_SYSTEM_WINDOWS
#		define ML_API_EXPORT __declspec(dllexport)
#		define ML_API_IMPORT __declspec(dllimport)
#		ifdef ML_CC_MSC
#			pragma warning(disable: 4031) // second formal parameter list longer than the first list
#			pragma warning(disable: 4099) // PDB was not found
#			pragma warning(disable: 4251) // type1 needs to have dll-interface to be used by type2
#			pragma warning(disable: 4307) // integral constant overflow
#			pragma warning(disable: 4723) // potential divide by zero
#		endif
#	else
#		if ML_CC_GNUC >= 4
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

#define ML_DLL_STR(name) name "_" ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".dll"
#define ML_LIB_STR(name) name "_" ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_CONFIG_HPP_