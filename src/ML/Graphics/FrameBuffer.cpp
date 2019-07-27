#include <ML/Graphics/FrameBuffer.hpp>
#include <ML/Graphics/OpenGL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	FrameBuffer::FrameBuffer()
		: I_Handle(NULL)
	{
	}

	FrameBuffer::FrameBuffer(const FrameBuffer & copy)
		: I_Handle(copy)
	{
	}

	FrameBuffer::~FrameBuffer()
	{
		clean();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	FrameBuffer & FrameBuffer::clean()
	{
		if ((*this))
		{
			ML_GL.deleteFramebuffers(1, (*this));
			get_reference() = NULL;
		}
		return (*this);
	}

	FrameBuffer & FrameBuffer::create()
	{
		if (set_handle(ML_GL.genFramebuffers(1)))
		{
			// good
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */
	
	const FrameBuffer & FrameBuffer::bind() const
	{
		ML_GL.bindFramebuffer(GL::Framebuffer, (*this));
		return (*this);
	}

	const FrameBuffer & FrameBuffer::unbind() const
	{
		ML_GL.bindFramebuffer(GL::Framebuffer, NULL);
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	const FrameBuffer & FrameBuffer::setTexture(uint32_t attchment, uint32_t value, uint32_t sampler, int32_t level) const
	{
		if (*this)
		{
			ML_GL.framebufferTexture2D(
				GL::Framebuffer,
				attchment,
				sampler,
				value, 
				level
			);
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}