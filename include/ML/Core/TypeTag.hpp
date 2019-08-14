#ifndef _ML_TYPE_TAG_HPP_
#define _ML_TYPE_TAG_HPP_

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class T
	> struct type_tag final
	{
		using type = typename T;

		constexpr type_tag() = default;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TYPE_TAG_HPP_