#ifndef _ML_CONFIG_HPP_
#define _ML_CONFIG_HPP_

//	Project Info
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ML_PROJECT_VER "1.0.0"
#define ML_PROJECT_URL "https://www.github.com/Gurman8r/ML"


//	C++ Version
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
# if defined(__cplusplus)
#	define ML_CPLUSPLUS __cplusplus
# else
#	define ML_CPLUSPLUS 0
# endif


//	General
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
# define _ML				::ml::
# define _ML_BEGIN			namespace ml {
# define _ML_END			}
		 
# define ML_xstr(l)			ML_str(l)
# define ML_str(l)			#l
# define ML_ARRAYSIZE(ARR)	(sizeof(ARR) / sizeof(*ARR))
		 
# define ML_TEMPLATE(...)	template<##__VA_ARGS__>
# define ML_USING_VA(...)	ML_TEMPLATE(##__VA_ARGS__) using
# define ML_USING_X			ML_USING_VA(class X)
# define ML_USING_XY		ML_USING_VA(class X, class Y)
# define ML_USING_XYZ		ML_USING_VA(class X, class Y, class Z)

//	Configuration (Debug / Release)
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
# if defined(_DEBUG)
#	define ML_DEBUG			true
#	define ML_CONFIGURATION "Debug"
# else
#	define ML_DEBUG			false
#	define ML_CONFIGURATION "Release"
# endif


//	Operating System
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
# if defined(_WIN32		) \
  || defined(_WIN64		) \
  || defined(WIN32		) \
  || defined(WIN64		) \
  || defined(__WIN32__	)
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
#	define ML_x32 true
#	define ML_x64 true
#	define ML_ARCHITECTURE 64
#	define ML_PLATFORM_TARGET "x64"
# else
#	define ML_x32 true
#	define ML_x64 false
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
#			pragma warning(disable: 4309) // truncation of constant value
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

#endif // !_ML_CONFIG_HPP_