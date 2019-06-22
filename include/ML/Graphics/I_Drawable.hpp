#ifndef _ML_I_DRAWABLE_HPP_
#define _ML_I_DRAWABLE_HPP_

#include <ML/Graphics/RenderBatch.hpp>

namespace ml
{
	class RenderTarget;

	class ML_GRAPHICS_API I_Drawable
	{
	public:
		virtual ~I_Drawable() {}

		virtual void draw(RenderTarget & target, RenderBatch batch) const = 0;
	};
}

#endif // !_ML_I_DRAWABLE_HPP_