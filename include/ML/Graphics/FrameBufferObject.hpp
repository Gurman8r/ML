#ifndef _ML_FRAMEBUFFER_HPP_
#define _ML_FRAMEBUFFER_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Graphics/GL.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/I_Handle.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API FrameBufferObject final
		: public I_Newable
		, public I_Handle<uint32_t>
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

		const FrameBufferObject & setTexture(uint32_t attchment, uint32_t value, uint32_t sampler, int32_t level) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	using FBO = typename FrameBufferObject;

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FRAMEBUFFER_HPP_