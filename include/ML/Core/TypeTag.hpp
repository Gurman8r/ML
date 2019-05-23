#ifndef _ML_TYPE_TAG_HPP_
#define _ML_TYPE_TAG_HPP_

// https://github.com/Manu343726/ctti/blob/master/include/ctti/type_tag.hpp

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template<typename T>
	struct type_tag
	{
		constexpr type_tag() = default;
		using type = T;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_TYPE_TAG_HPP_