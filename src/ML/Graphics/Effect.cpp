#include <ML/Graphics/Effect.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Graphics/RenderTarget.hpp>
#include <ML/Graphics/ShaderAPI.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Effect::Effect()
		: m_fbo			()
		, m_rbo			()
		, m_model		(NULL)
		, m_shader		(NULL)
		, m_texture		()
		, m_size		(0)
		, m_attachment	(GL::ColorAttachment0)
	{
	}

	Effect::Effect(const Effect & copy)
		: m_fbo			(copy.m_fbo)
		, m_rbo			(copy.m_rbo)
		, m_model		(copy.m_model)
		, m_shader		(copy.m_shader)
		, m_texture		(copy.m_texture)
		, m_size		(copy.m_size)
		, m_attachment	(copy.m_attachment)
	{
	}

	Effect::~Effect()
	{
		dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Effect::dispose()
	{
		if (m_fbo && m_rbo)
		{
			m_fbo.clean();
			m_rbo.clean();
		}
		return (!m_fbo && !m_rbo);
	}

	bool Effect::loadFromFile(const String & filename)
	{
		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Effect::create(const vec2i & size, uint32_t attachment)
	{
		if (!m_fbo && !m_rbo)
		{
			if (((m_size = size) != vec2i::Zero) && (m_attachment = attachment))
			{
				// Setup Framebuffer
				m_fbo.create().bind();
				{
					// Setup Renderbuffer
					m_rbo.create(m_size[0], m_size[1]);
					m_rbo.bind();
					m_rbo.bufferStorage(ml::GL::Depth24_Stencil8);
					m_rbo.setFramebuffer(ml::GL::DepthStencilAttachment);
					m_rbo.unbind();

					// Check Framebuffer Status
					if (!ML_GL.checkFramebufferStatus(ml::GL::Framebuffer))
					{
						return ml::Debug::logError("Framebuffer is not complete");
					}

					// Setup Texture
					m_texture.dispose();
					m_texture.create(size);
					m_fbo.setTexture(
						(m_attachment),
						(*m_texture),
						(m_texture.target()),
						(m_texture.level())
					);
				}
				m_fbo.unbind();

				return (m_fbo && m_rbo);
			}
		}
		return false;
	}

	bool Effect::resize(const vec2i & size)
	{
		return 
			(size != vec2i::Zero) &&
			(m_size != size) &&
			(dispose()) &&
			(create(size, m_attachment));
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Effect::bind() const
	{
		m_fbo.bind();
	}
	
	void Effect::unbind() const
	{
		m_fbo.unbind();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Effect::setModel(const Model * value)
	{
		return (m_model = value);
	}

	bool Effect::setShader(const Shader * value)
	{
		return (m_shader = value);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Effect::draw(RenderTarget & target, RenderBatch batch) const
	{
		if (m_model && m_shader && m_texture)
		{
			m_shader->setUniform(ML_FRAG_MAIN_TEX, m_texture);

			m_shader->bind();

			target.draw(m_model);

			m_shader->unbind();
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}