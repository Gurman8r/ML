#ifndef _ML_ICON_HPP_
#define _ML_ICON_HPP_

#include <ML/Window/Export.hpp>
#include <ML/Core/IObject.hpp>
#include <ML/Core/IComparable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API Icon final
		: public IObject
		, public IComparable<Icon>
	{
		/* * * * * * * * * * * * * * * * * * * * */

		int32_t		width;
		int32_t		height;
		uint8_t *	pixels;

		/* * * * * * * * * * * * * * * * * * * * */

		Icon();
		Icon(const int32_t width, const int32_t height, uint8_t * pixels);
		Icon(const int32_t width, const int32_t height, const uint8_t * pixels);
		Icon(const Icon & copy);
		~Icon();

		/* * * * * * * * * * * * * * * * * * * * */

		bool equals(const Icon & other) const override;
		bool lessThan(const Icon & other) const override;

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ICON_HPP_