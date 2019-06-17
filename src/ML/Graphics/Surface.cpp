#include <ML/Graphics/Surface.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Graphics/RenderTarget.hpp>
#include <ML/Graphics/ShaderAPI.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Surface::Surface()
		: m_fbo			()
		, m_rbo			()
		, m_model		(nullptr)
		, m_shader		(nullptr)
		, m_texture		()
		, m_size		(0)
		, m_attachment	(GL::ColorAttachment0)
	{
	}

	Surface::Surface(const Surface & copy)
		: m_fbo			(copy.m_fbo)
		, m_rbo			(copy.m_rbo)
		, m_model		(copy.m_model)
		, m_shader		(copy.m_shader)
		, m_texture		(copy.m_texture)
		, m_size		(copy.m_size)
		, m_attachment	(copy.m_attachment)
	{
	}

	Surface::~Surface()
	{
		dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Surface::dispose()
	{
		if (m_fbo && m_rbo)
		{
			m_fbo.clean();
			m_rbo.clean();
		}
		return (!m_fbo && !m_rbo);
	}

	bool Surface::loadFromFile(const String & filename)
	{
		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Surface::create(const vec2i & size, uint32_t attachment)
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
					m_rbo.setFramebuffer(ml::GL::DepthStencil);
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

	bool Surface::resize(const vec2i & size)
	{
		return 
			(size != vec2i::Zero) &&
			(m_size != size) &&
			(dispose()) &&
			(create(size, m_attachment));
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Surface::bind() const
	{
		m_fbo.bind();
	}
	
	void Surface::unbind() const
	{
		m_fbo.unbind();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Surface::setModel(const Model * value)
	{
		return (m_model = value);
	}

	bool Surface::setShader(const Shader * value)
	{
		return (m_shader = value);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Surface::draw(RenderTarget & target, RenderBatch batch) const
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