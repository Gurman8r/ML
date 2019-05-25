#ifndef _ML_PREPROCESSOR_HPP_
#define _ML_PREPROCESSOR_HPP_

#include <ML/Core/Config.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_xstr(a) ML_str(a)
#define ML_str(a) #a

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_MIN(lhs, rhs) ((lhs <= rhs) ? lhs : rhs)
#define ML_MAX(lhs, rhs) ((lhs >= rhs) ? lhs : rhs)
#define ML_CLAMP(value, minVal, maxVal) (ML_MIN(ML_MAX(value, minVal), maxVal))

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_PREPROCESSOR_HPP_