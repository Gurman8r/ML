#include <ML/Graphics/RenderBufferObject.hpp>
#include <ML/Graphics/OpenGL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RenderBufferObject::RenderBufferObject()
		: Handle(NULL)
	{
	}

	RenderBufferObject::RenderBufferObject(RenderBufferObject const & copy)
		: Handle(copy)
	{
	}

	RenderBufferObject::~RenderBufferObject()
	{
		clean();
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	RenderBufferObject & RenderBufferObject::clean()
	{
		if ((*this))
		{
			ML_GL.deleteRenderbuffer(*this);

			this->set_handle(NULL);
		}
		return (*this);
	}
	
	RenderBufferObject & RenderBufferObject::create(vec2i const & size)
	{
		if (this->set_handle(ML_GL.genRenderbuffer()))
		{
			m_size = size;
		}
		return (*this);
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	RenderBufferObject const & RenderBufferObject::bind() const
	{
		ML_GL.bindRenderbuffer(GL::Renderbuffer, (*this));
		return (*this);
	}
	
	RenderBufferObject const & RenderBufferObject::unbind() const
	{
		ML_GL.bindRenderbuffer(GL::Renderbuffer, NULL);
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RenderBufferObject const & RenderBufferObject::bufferStorage(GL::Format format) const
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

	RenderBufferObject const & RenderBufferObject::setFramebuffer(GL::FrameID attachment) const
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