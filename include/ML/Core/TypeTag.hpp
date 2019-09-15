#ifndef _ML_TYPE_TAG_HPP_
#define _ML_TYPE_TAG_HPP_

namespace ml
{
	template <class T> struct TypeTag final
	{
		using type = typename T;
		constexpr TypeTag() = default;
	};
}

#endif // !_ML_TYPE_TAG_HPP_