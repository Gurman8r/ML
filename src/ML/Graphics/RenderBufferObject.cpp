#include <ML/Graphics/RenderBufferObject.hpp>
#include <ML/Graphics/OpenGL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	RenderBufferObject::RenderBufferObject()
		: I_Handle(NULL)
	{
	}

	RenderBufferObject::RenderBufferObject(const RenderBufferObject & copy)
		: I_Handle(copy)
	{
	}

	RenderBufferObject::~RenderBufferObject()
	{
		clean();
	}
	
	/* * * * * * * * * * * * * * * * * * * * */
	
	RenderBufferObject & RenderBufferObject::clean()
	{
		if ((*this))
		{
			ML_GL.deleteRenderbuffers(1, (*this));
			get_reference() = NULL;
		}
		return (*this);
	}
	
	RenderBufferObject & RenderBufferObject::create(int32_t width, int32_t height)
	{
		if (set_handle(ML_GL.genRenderbuffers(1)))
		{
			m_width = width;
			m_height = height;
		}
		return (*this);
	}
	
	/* * * * * * * * * * * * * * * * * * * * */
	
	const RenderBufferObject & RenderBufferObject::bind() const
	{
		ML_GL.bindRenderbuffer(GL::Renderbuffer, (*this));
		return (*this);
	}
	
	const RenderBufferObject & RenderBufferObject::unbind() const
	{
		ML_GL.bindRenderbuffer(GL::Renderbuffer, NULL);
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	const RenderBufferObject & RenderBufferObject::bufferStorage(GL::Format format) const
	{
		if (*this)
		{
			ML_GL.renderbufferStorage(
				GL::Renderbuffer, 
				format, 
				m_width, 
				m_height
			);
		}
		return (*this);
	}

	const RenderBufferObject & RenderBufferObject::setFramebuffer(GL::FrameAttachment attachment) const
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

	/* * * * * * * * * * * * * * * * * * * * */
}