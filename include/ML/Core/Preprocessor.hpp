#ifndef _ML_PREPROCESSOR_HPP_
#define _ML_PREPROCESSOR_HPP_

// Stringify
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ML_xstr(l) ML_str(l)
#define ML_str(l) #l


// Utility
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ML_MIN(l, r) ((l <= r) ? l : r)
#define ML_MAX(l, r) ((l >= r) ? l : r)
#define ML_CLAMP(Val, Min, Max) (ML_MIN(ML_MAX(Val, Min), Max))


// Generate Bit Mask Operators
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ML_GENERATE_BITMASK_OPERATORS(Pref, Type, Base) \
Pref Type operator&(const Type l, const Type r) { return (Type)((Base)l & (Base)r); } \
Pref Type operator|(const Type l, const Type r) { return (Type)((Base)l | (Base)r); } \
Pref Type & operator &=(Type & l, const Type r) { return (l = (l & r)); } \
Pref Type & operator |=(Type & l, const Type r) { return (l = (l | r)); }


// Generate Iterator Operators
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ML_GENERATE_ITERATOR_OPERATORS(Pref, Type, Base, Min, Max) \
template <class T> Pref Type operator+(const Type l, T r) \
{ \
	return (Type)(ML_CLAMP(((Base)l + (Base)r), Min, Max)); \
} \
template <class T> Pref Type operator-(const Type l, T r) \
{ \
	return (l + (-r)); \
} \
template <class T> Pref Type & operator+=(Type & l, T r) \
{ \
	return (l = (l + r)); \
} \
template <class T> Pref Type & operator-=(Type & l, T r) \
{ \
	return (l = (l - r)); \
} \
Pref Type operator++(Type & value) { return (value += 1); } \
Pref Type operator--(Type & value) { return (value -= 1); } \
\
Pref Type operator++(Type & value, int)	{ Type temp = value; value += 1; return temp; } \
Pref Type operator--(Type & value, int)	{ Type temp = value; value -= 1; return temp; }


#endif // !_ML_PREPROCESSOR_HPP_