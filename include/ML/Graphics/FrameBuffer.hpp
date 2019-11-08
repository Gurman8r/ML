#ifndef _ML_FRAMEBUFFER_HPP_
#define _ML_FRAMEBUFFER_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Graphics/GL.hpp>
#include <ML/Core/Newable.hpp>
#include <ML/Core/Handle.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct Texture;

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API FrameBuffer final
		: public Newable
		, public Handle<uint32_t>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		FrameBuffer();
		FrameBuffer(const FrameBuffer & copy);
		~FrameBuffer();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		FrameBuffer & clean();

		FrameBuffer & create();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const FrameBuffer & bind() const;

		const FrameBuffer & unbind() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const FrameBuffer & setTexture(uint32_t attachment, uint32_t texture, uint32_t sampler, int32_t level) const;

		const FrameBuffer & setTexture(uint32_t attachment, const Texture & value) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	using FBO = typename FrameBuffer;

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FRAMEBUFFER_HPP_