#include <ML/Graphics/RenderBuffer.hpp>
#include <ML/Graphics/OpenGL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	RenderBuffer::RenderBuffer()
		: IHandle(NULL)
	{
	}

	RenderBuffer::RenderBuffer(const RenderBuffer & copy)
		: IHandle(copy)
	{
	}

	RenderBuffer::~RenderBuffer()
	{
		clean();
	}
	
	/* * * * * * * * * * * * * * * * * * * * */
	
	RenderBuffer & RenderBuffer::clean()
	{
		if ((*this))
		{
			ML_GL.deleteRenderbuffers(1, (*this));
			get_reference() = NULL;
		}
		return (*this);
	}
	
	RenderBuffer & RenderBuffer::create(int32_t width, int32_t height)
	{
		if (set_handle(ML_GL.genRenderbuffers(1)))
		{
			m_width = width;
			m_height = height;
		}
		return (*this);
	}
	
	/* * * * * * * * * * * * * * * * * * * * */
	
	const RenderBuffer & RenderBuffer::bind() const
	{
		ML_GL.bindRenderbuffer(GL::Renderbuffer, (*this));
		return (*this);
	}
	
	const RenderBuffer & RenderBuffer::unbind() const
	{
		ML_GL.bindRenderbuffer(GL::Renderbuffer, NULL);
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	const RenderBuffer & RenderBuffer::bufferStorage(GL::Format format) const
	{
		ML_GL.renderbufferStorage(GL::Renderbuffer, format, m_width, m_height);
		return (*this);
	}

	const RenderBuffer & RenderBuffer::setFramebuffer(GL::FrameAttachment attachment) const
	{
		ML_GL.framebufferRenderbuffer(
			GL::Framebuffer, 
			attachment, 
			GL::Renderbuffer,
			(*this));
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}