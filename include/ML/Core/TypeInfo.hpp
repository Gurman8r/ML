#ifndef _ML_TYPE_INFO_HPP_
#define _ML_TYPE_INFO_HPP_

#include <ML/Core/TypeID.hpp>
#include <ML/Core/Preprocessor.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

# ifdef ML_USE_CTTI
#	define ML_TYPE_HASH(T) (ml::type_id(T).hash())
# else
#	define ML_TYPE_HASH(T) ((const std::type_info &)typeid(T)).hash_code()
# endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_assert_is_base_of(base, derived) \
static_assert( \
	std::is_base_of<base, derived>::value, \
	"Type \'" ML_str(derived) "\' must derive \'" ML_xstr(base) "\'" \
);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_TYPE_INFO_HPP_