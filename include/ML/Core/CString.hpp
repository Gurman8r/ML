#ifndef _ML_C_STRING_HPP_
#define _ML_C_STRING_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/StandardLib.hpp>

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	using CString		= typename const char *;
	using CWString		= typename const wchar_t *;
	using CU16String	= typename const char16_t *;
	using CU32String	= typename const char32_t *;
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_C_STRING_HPP_