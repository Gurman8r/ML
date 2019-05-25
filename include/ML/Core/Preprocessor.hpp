#ifndef _ML_PREPROCESSOR_HPP_
#define _ML_PREPROCESSOR_HPP_

#include <ML/Core/Config.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_xstr(a) ML_str(a)
#define ML_str(a) #a

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_bitRead(value, bit) \
(((value) >> (bit)) & 0x01)

#define ML_bitSet(value, bit) \
((value) |= (1UL << (bit)))

#define ML_bitClear(value, bit) \
((value) &= ~(1UL << (bit)))

#define ML_bitWrite(value, bit, bitvalue) \
(bitvalue ? ML_bitSet(value, bit) : ML_bitClear(value, bit))

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_MIN(lhs, rhs) ((lhs <= rhs) ? lhs : rhs)

#define ML_MAX(lhs, rhs) ((lhs >= rhs) ? lhs : rhs)

#define ML_CLAMP(value, minVal, maxVal) (ML_MIN(ML_MAX(value, minVal), maxVal))

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_PI 3.14159265359f

#define ML_DEG2RAD(degrees) ((ML_PI / 180.f) * degrees)

#define ML_RAD2DEG(radians) ((180.f / ML_PI) * radians)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_ASPECT(w, h) (((w != 0) && (h != 0)) \
	? (static_cast<float>(w) / static_cast<float>(h)) \
	: (0.0f))

#define ML_LERP(a, b, c) (a * c + b * (1 - c))

#define ML_MAP_RANGE(value, min1, max1, min2, max2) \
(min2 + (value - min1) * (max2 - min2) / (max1 - min1))

#define ML_SIGN(value) \
(((value) == (0)) \
	? (0) \
	: (((value) > (0)) \
		? (+1) \
		: (-1)))

#define ML_TYPEOF(C) ((const std::type_info &)typeid(C)).hash_code()

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_assert_is_base_of(base, derived) \
static_assert( \
	std::is_base_of<base, derived>::value, \
	"Type \'" ML_str(derived) "\' must derive \'" ML_xstr(base) "\'" \
);

// Generate Bit Mask Operators
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ML_GENERATE_MASK_OPERATORS(NAME, TYPE) \
inline NAME operator&(const NAME a, const NAME b) { return (NAME)((TYPE)a & (TYPE)b); } \
inline NAME operator|(const NAME a, const NAME b) { return (NAME)((TYPE)a | (TYPE)b); } \
inline NAME & operator &=(NAME & a, const NAME b) { return (a = (a & b)); } \
inline NAME & operator |=(NAME & a, const NAME b) { return (a = (a | b)); }


// Generate Iterator Operators
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ML_GENERATE_ITER_OPERATORS(NAME, TYPE, ITER, MIN, MAX) \
template <class T> inline NAME operator+(const NAME a, const T b) \
{ \
	return (NAME)(ML_CLAMP(((TYPE)a + (TYPE)b), MIN, MAX)); \
} \
template <class T> inline NAME operator-(const NAME a, const T b) { return (a + (-b)); } \
template <class T> inline NAME & operator+=(NAME & a, const T b) { return (a = (a + b)); } \
template <class T> inline NAME & operator-=(NAME & a, const T b) { return (a = (a - b)); } \
inline NAME operator++(NAME & a)		{ return (a += 1); } \
inline NAME operator--(NAME & a)		{ return (a -= 1); } \
inline NAME operator++(NAME & a, ITER)	{ NAME c = a; a += 1; return c; } \
inline NAME operator--(NAME & a, ITER)	{ NAME c = a; a -= 1; return c; }

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_PREPROCESSOR_HPP_