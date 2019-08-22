#include <ML/Graphics/Surface.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Graphics/RenderTarget.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Surface::Surface()
		: m_shader		{ nullptr }
		, m_model		{ nullptr }
		, m_texture		{}
		, m_size		{ 0, 0 }
		, m_attachment	{ GL::ColorAttachment0 }
		, m_fbo			{}
		, m_rbo			{}
	{
	}

	Surface::Surface(const Model * model, const Shader * shader)
		: m_shader		{ shader }
		, m_model		{ model }
		, m_texture		{}
		, m_size		{ 0, 0 }
		, m_attachment	{ GL::ColorAttachment0 }
		, m_fbo			{}
		, m_rbo			{}
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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Surface::dispose()
	{
		if (m_fbo) { m_fbo.clean(); }
		if (m_rbo) { m_rbo.clean(); }
		return (!m_fbo && !m_rbo);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Surface::create(const vec2i & size, uint32_t attachment)
	{
		if (!m_fbo && !m_rbo)
		{
			if ((size != vec2i { 0, 0 }) && (m_attachment = attachment))
			{
				m_size = size;

				// Setup Framebuffer
				if (m_fbo.create().bind())
				{
					// Setup Renderbuffer
					m_rbo
						.create(m_size)
						.bind()
						.bufferStorage(GL::Depth24_Stencil8)
						.setFramebuffer(GL::DepthStencil)
						.unbind();

					// Check Framebuffer Status
					if (ML_GL.checkFramebufferStatus(GL::Framebuffer))
					{
						// Create Texture
						m_texture.dispose();
						m_texture.create(m_size);

						// Attach to FBO
						m_fbo.setTexture(
							m_attachment,
							(*m_texture),
							m_texture.sampler(),
							m_texture.level()
						);
					}
					
					// Unbind
					m_fbo.unbind();
				}

				return (m_fbo && m_rbo);
			}
		}
		return false;
	}

	bool Surface::update(const vec2i & size)
	{
		return 
			(size[0] != 0 && size[1] != 0) &&
			(m_size != size) &&
			(dispose()) &&
			(create(size, m_attachment));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const Surface & Surface::bind() const
	{
		m_fbo.bind();
		return (*this);
	}
	
	const Surface & Surface::unbind() const
	{
		m_fbo.unbind();
		return (*this);
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Surface & Surface::setModel(const Model * value)
	{
		m_model = value;
		return (*this);
	}

	Surface & Surface::setShader(const Shader * value)
	{
		m_shader = value;
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Surface::draw(RenderTarget & target, RenderBatch batch) const
	{
		if (*this)
		{
			m_shader->setUniform(ML_UNI_MAIN_TEX, m_texture);
			
			m_shader->bind();
			
			target.draw(m_model);
			
			m_shader->unbind();
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}