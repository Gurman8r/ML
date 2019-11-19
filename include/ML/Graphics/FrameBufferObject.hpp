#ifndef _ML_FRAMEBUFFER_HPP_
#define _ML_FRAMEBUFFER_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Graphics/GL.hpp>
#include <ML/Core/MemoryTracker.hpp>
#include <ML/Core/Handle.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct Texture;

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API FrameBufferObject final : public Trackable, public Handle<uint32_t>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		FrameBufferObject();
		FrameBufferObject(FrameBufferObject const & copy);
		~FrameBufferObject();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		FrameBufferObject & clean();

		FrameBufferObject & create();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		FrameBufferObject const & bind() const;

		FrameBufferObject const & unbind() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		FrameBufferObject const & setTexture(uint32_t attachment, uint32_t texture, uint32_t sampler, int32_t level) const;

		FrameBufferObject const & setTexture(uint32_t attachment, Texture const & value) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	ML_USING FBO = typename FrameBufferObject;

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FRAMEBUFFER_HPP_