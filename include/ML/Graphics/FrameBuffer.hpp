#ifndef _ML_FRAMEBUFFER_HPP_
#define _ML_FRAMEBUFFER_HPP_

#include <ML/Graphics/GL.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/List.hpp>
#include <ML/Core/I_Handle.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API FrameBuffer final
		: public I_Newable
		, public I_Handle<uint32_t>
	{
	public:
		FrameBuffer();
		FrameBuffer(const FrameBuffer & copy);
		~FrameBuffer();

	public:
		FrameBuffer & clean();
		FrameBuffer & create();

	public:
		const FrameBuffer & bind() const;
		const FrameBuffer & unbind() const;
		const FrameBuffer & setTexture(uint32_t attchment, uint32_t value, GL::Target target, int32_t level) const;
	};

	/* * * * * * * * * * * * * * * * * * * * */

	using FBO = typename FrameBuffer;

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FRAMEBUFFER_HPP_