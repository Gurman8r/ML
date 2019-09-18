#ifndef _ML_TYPE_TAG_HPP_
#define _ML_TYPE_TAG_HPP_

namespace ml
{
	template <class ... T> struct TypeTag;

	template <class T> struct TypeTag<T>
	{
		using type = typename T;
		constexpr TypeTag() = default;
	};

	template <> struct TypeTag<>
	{
		using type = typename void;
	};
}

#endif // !_ML_TYPE_TAG_HPP_