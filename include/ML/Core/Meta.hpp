#ifndef _ML_META_HPP_
#define _ML_META_HPP_

// https://github.com/Manu343726/ctti/blob/master/include/ctti/detail/meta.hpp

#include <ML/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace meta
	{
		template<typename T>
		using void_t = typename std::conditional<sizeof(T*) >= 0, void, T>::type;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_META_HPP_