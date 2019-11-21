#include <ML/Graphics/RenderBufferObject.hpp>
#include <ML/Graphics/OpenGL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RenderBufferObject::RenderBufferObject()
		: Handle{ NULL }
		, m_size{ 0 }
		, m_storage{ (GL::Format)0 }
	{
	}

	RenderBufferObject::RenderBufferObject(RenderBufferObject const & copy)
		: Handle{ copy }
		, m_size{ copy.m_size }
		, m_storage{ copy.m_storage }
	{
	}

	RenderBufferObject::~RenderBufferObject()
	{
		clean();
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	RenderBufferObject & RenderBufferObject::clean()
	{
		if (*this)
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

	RenderBufferObject const & RenderBufferObject::bufferStorage(GL::Format value) const
	{
		if ((*this) && value)
		{
			ML_GL.renderbufferStorage(
				GL::Renderbuffer, 
				(m_storage = value),
				width(), 
				height()
			);
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}