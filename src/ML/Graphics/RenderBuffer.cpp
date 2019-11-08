#include <ML/Graphics/RenderBuffer.hpp>
#include <ML/Graphics/OpenGL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RenderBuffer::RenderBuffer()
		: Handle(NULL)
	{
	}

	RenderBuffer::RenderBuffer(const RenderBuffer & copy)
		: Handle(copy)
	{
	}

	RenderBuffer::~RenderBuffer()
	{
		clean();
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	RenderBuffer & RenderBuffer::clean()
	{
		if ((*this))
		{
			ML_GL.deleteRenderbuffer(*this);

			this->set_handle(NULL);
		}
		return (*this);
	}
	
	RenderBuffer & RenderBuffer::create(const vec2i & size)
	{
		if (this->set_handle(ML_GL.genRenderbuffer()))
		{
			m_size = size;
		}
		return (*this);
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const RenderBuffer & RenderBuffer::bufferStorage(GL::Format format) const
	{
		if (*this)
		{
			ML_GL.renderbufferStorage(
				GL::Renderbuffer, 
				format, 
				width(), 
				height()
			);
		}
		return (*this);
	}

	const RenderBuffer & RenderBuffer::setFramebuffer(GL::FrameID attachment) const
	{
		if (*this)
		{
			ML_GL.framebufferRenderbuffer(
				GL::Framebuffer, 
				attachment, 
				GL::Renderbuffer,
				(*this)
			);
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}