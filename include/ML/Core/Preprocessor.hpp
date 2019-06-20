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


#endif // !_ML_PREPROCESSOR_HPP_