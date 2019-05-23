#ifndef _ML_CONFIG_HPP_
#define _ML_CONFIG_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Project Info
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ML_PROJECT_VER "1.0.0"
#define ML_PROJECT_URL "https://www.github.com/Gurman8r/ML"


// Disable C-Runtime Warnings
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
# ifndef _CRT_SECURE_NO_WARNINGS
#	define _CRT_SECURE_NO_WARNINGS 1
# endif


//	C/C++
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
# if defined(__cplusplus)
#	define ML_CPP
# endif

# if defined(__cpp_variable_templates)
#	define ML_VARIABLE_TEMPLATES
# endif


//	Configuration (Debug/Release)
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
# if defined(_DEBUG)
#	define ML_DEBUG			1
#	define ML_RELEASE		0
#	define ML_CONFIGURATION "Debug"
# else
#	define ML_DEBUG			0
#	define ML_RELEASE		1
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
#	define ML_x64
#	define ML_ARCHITECTURE 64
#	define ML_PLATFORM_TARGET "x64"
# else
#	define ML_x32
#	define ML_ARCHITECTURE 32
#	define ML_PLATFORM_TARGET "x86"
# endif


//	Compiler
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
# if defined(_MSC_VER)
#	define ML_CC_MS		_MSC_VER	// Microsoft Compiler
# elif defined(__clang__)
#	define ML_CC_CLANG	__clang__	// Clang Compiler
# elif defined(__GNUC__)
#	define ML_CC_GNU	__GNUC__	// GNU Compiler
#else
#	error This compiler does not support memes.
# endif


//	Export / Import
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
# ifndef ML_STATIC
#	ifdef ML_SYSTEM_WINDOWS
#		define ML_API_EXPORT __declspec(dllexport)
#		define ML_API_IMPORT __declspec(dllimport)
#		ifdef ML_CC_MS
#			pragma warning(disable: 4031)
#			pragma warning(disable: 4099)
#			pragma warning(disable: 4251)
#			pragma warning(disable: 4723)
#		endif
#	else
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


#endif // !_ML_CONFIG_HPP_