#include <ML/Graphics/FrameBufferObject.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Graphics/Texture.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	FrameBufferObject::FrameBufferObject()
		: Handle{ NULL }
		, m_size{ 0 }
	{
	}

	FrameBufferObject::FrameBufferObject(FrameBufferObject const & copy)
		: Handle{ copy }
		, m_size{ copy.m_size }
	{
	}

	FrameBufferObject::~FrameBufferObject()
	{
		this->clean();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	FrameBufferObject & FrameBufferObject::clean()
	{
		if (*this)
		{
			ML_GL.deleteFramebuffer(*this);

			this->set_handle(NULL);
		}
		return (*this);
	}

	FrameBufferObject & FrameBufferObject::create(vec2i const & value)
	{
		if (this->set_handle(ML_GL.genFramebuffer()))
		{
			m_size = value;
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	FrameBufferObject const & FrameBufferObject::bind() const
	{
		ML_GL.bindFramebuffer(GL::Framebuffer, (*this));
		return (*this);
	}

	FrameBufferObject const & FrameBufferObject::unbind() const
	{
		ML_GL.bindFramebuffer(GL::Framebuffer, NULL);
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	FrameBufferObject const & FrameBufferObject::texture2D(uint32_t attachment, uint32_t texture, uint32_t sampler, int32_t level) const
	{
		if (*this)
		{
			ML_GL.framebufferTexture2D(
				GL::Framebuffer,
				attachment,
				sampler,
				texture,
				level
			);
		}
		return (*this);
	}

	FrameBufferObject const & FrameBufferObject::texture2D(uint32_t attachment, Texture const & value) const
	{
		return texture2D(attachment, value, value.sampler(), value.level());
	}

	FrameBufferObject const & FrameBufferObject::renderBuffer(GL::FrameID attachment, uint32_t value) const
	{
		if (*this)
		{
			ML_GL.framebufferRenderbuffer(
				GL::Framebuffer,
				attachment,
				GL::Renderbuffer,
				value
			);
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}