#ifndef _ML_PREPROCESSOR_HPP_
#define _ML_PREPROCESSOR_HPP_

#include <ML/Core/Config.hpp>

// Bit Math Macros
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ML_bitRead(value, bit) \
(((value) >> (bit)) & 0x01)

#define ML_bitSet(value, bit) \
((value) |= (1UL << (bit)))

#define ML_bitClear(value, bit) \
((value) &= ~(1UL << (bit)))

#define ML_bitWrite(value, bit, bitvalue) \
(bitvalue ? ML_bitSet(value, bit) : ML_bitClear(value, bit))


// Type Hash
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ML_typeof(value) ((const std::type_info *)(&typeid(value)))->hash_code()


// Aspect Ratio
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ML_ASPECT(w, h) (((w != 0) && (h != 0)) \
	? (static_cast<float>(w) / static_cast<float>(h)) \
	: (0.0f))


// Clamp
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ML_CLAMP(value, min, max) \
((value > max) \
	? ((value < min) \
		? (min) \
		: (value)) \
	: (max))


// Lerp
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ML_LERP(a, b, c) (a * c + b * (1 - c))


// Map Range
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ML_MAP_RANGE(value, min1, max1, min2, max2) \
(min2 + (value - min1) * (max2 - min2) / (max1 - min1))


// Sign
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ML_SIGN(value) \
(((value) == (0)) \
	? (0) \
	: (((value) > (0)) \
		? (+1) \
		: (-1)))


// Stringify
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ML_xstr(a) ML_str(a)
#define ML_str(a) #a


//	Library Strings
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ML_DLL_STR(name) name "_" ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".dll"
#define ML_LIB_STR(name) name "_" ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib"


// Generate Bit Mask Operators
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ML_GENERATE_MASK_OPERATORS(name, type) \
inline name operator&(const name a, const name b) { return (name)((type)a & (type)b); } \
inline name operator|(const name a, const name b) { return (name)((type)a | (type)b); } \
inline name & operator &=(name & a, const name b) { return (a = (a & b)); } \
inline name & operator |=(name & a, const name b) { return (a = (a | b)); }


// Generate Iterator Operators
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ML_GENERATE_ITER_OPERATORS_T(T, name, type, iter, min, max) \
template <class T> inline name operator+(const name a, const T b) \
{ \
	return (name)(ML_CLAMP(((type)a + (type)b), min, max)); \
} \
template <class T> inline name operator-(const name a, const T b) { return (a + (-b)); } \
template <class T> inline name & operator+=(name & a, const T b) { return (a = (a + b)); } \
template <class T> inline name & operator-=(name & a, const T b) { return (a = (a - b)); } \
inline name operator++(name & a)		{ return (a += 1); } \
inline name operator--(name & a)		{ return (a -= 1); } \
inline name operator++(name & a, iter)	{ name c = a; a += 1; return c; } \
inline name operator--(name & a, iter)	{ name c = a; a -= 1; return c; }

#define ML_GENERATE_ITER_OPERATORS(name, type, iter, min, max) \
ML_GENERATE_ITER_OPERATORS_T(T, name, type, iter, min, max)


// Assertion Macros
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ML_assert_is_base_of(base, derived) \
static_assert( \
	std::is_base_of<base, derived>::value, \
	"Type \'" ML_str(derived) "\' must derive \'" ML_xstr(base) "\'" \
);


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_PREPROCESSOR_HPP_