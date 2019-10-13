#ifndef _ML_DRAWABLE_HPP_
#define _ML_DRAWABLE_HPP_

#include <ML/Graphics/RenderBatch.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct RenderTarget;

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API Drawable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~Drawable() {}

		virtual void draw(RenderTarget & target, RenderBatch batch) const = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DRAWABLE_HPP_