#include <ML/Graphics/FrameBufferObject.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Graphics/Texture.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	FrameBufferObject::FrameBufferObject()
		: Handle(NULL)
	{
	}

	FrameBufferObject::FrameBufferObject(const FrameBufferObject & copy)
		: Handle(copy)
	{
	}

	FrameBufferObject::~FrameBufferObject()
	{
		clean();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	FrameBufferObject & FrameBufferObject::clean()
	{
		if ((*this))
		{
			ML_GL.deleteFramebuffer(*this);
			this->set_handle(NULL);
		}
		return (*this);
	}

	FrameBufferObject & FrameBufferObject::create()
	{
		if (this->set_handle(ML_GL.genFramebuffer()))
		{
			// good
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	const FrameBufferObject & FrameBufferObject::bind() const
	{
		ML_GL.bindFramebuffer(GL::Framebuffer, (*this));
		return (*this);
	}

	const FrameBufferObject & FrameBufferObject::unbind() const
	{
		ML_GL.bindFramebuffer(GL::Framebuffer, NULL);
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const FrameBufferObject & FrameBufferObject::setTexture(uint32_t attachment, uint32_t texture, uint32_t sampler, int32_t level) const
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

	const FrameBufferObject & FrameBufferObject::setTexture(uint32_t attachment, const Texture & value) const
	{
		return setTexture(attachment, value, value.sampler(), value.level());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}