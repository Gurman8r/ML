#ifndef _ML_FRAMEBUFFER_HPP_
#define _ML_FRAMEBUFFER_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Graphics/GL.hpp>
#include <ML/Core/Trackable.hpp>
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
		FrameBufferObject(const FrameBufferObject & copy);
		~FrameBufferObject();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		FrameBufferObject & clean();

		FrameBufferObject & create();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const FrameBufferObject & bind() const;

		const FrameBufferObject & unbind() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const FrameBufferObject & setTexture(uint32_t attachment, uint32_t texture, uint32_t sampler, int32_t level) const;

		const FrameBufferObject & setTexture(uint32_t attachment, const Texture & value) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	using FBO = typename FrameBufferObject;

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FRAMEBUFFER_HPP_