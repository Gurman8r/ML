#include <ML/Graphics/Surface.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Graphics/RenderTarget.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Surface::Surface()
		: Surface { nullptr, nullptr, nullptr }
	{
	}

	Surface::Surface(const_ptr_t<Model> model, const_ptr_t<Material> material, const_ptr_t<Shader> shader)
		: m_colorID	{ GL::ColorAttachment0 }
		, m_frameID { GL::DepthStencil }
		, m_fbo		{ }
		, m_material{ material }
		, m_model	{ model }
		, m_shader	{ shader }
		, m_rbo		{ }
		, m_size	{ 0, 0 }
		, m_storage	{ GL::Depth24_Stencil8 }
		, m_texture	{ }
	{
	}

	Surface::Surface(const Surface & copy)
		: m_colorID	{ copy.m_colorID }
		, m_frameID	{ copy.m_frameID }
		, m_fbo		{ }
		, m_material{ copy.m_material }
		, m_model	{ copy.m_model }
		, m_rbo		{ }
		, m_size	{ copy.m_size }
		, m_storage	{ copy.m_storage }
		, m_texture	{ }
	{
	}

	Surface::~Surface() { this->dispose(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Surface::dispose()
	{
		if (m_fbo) { m_fbo.clean(); }
		if (m_rbo) { m_rbo.clean(); }
		return (!m_fbo && !m_rbo);
	}

	bool Surface::create()
	{
		if (m_fbo || m_rbo || (m_size[0] == 0) || (m_size[1] == 0))
		{
			return false;
		}

		// Setup Framebuffer
		if (m_fbo.create().bind())
		{
			// Setup Renderbuffer
			m_rbo
				.create(m_size)
				.bind()
				.bufferStorage(m_storage)
				.setFramebuffer(m_frameID)
				.unbind();

			// Check Framebuffer Status
			if (ML_GL.checkFramebufferStatus(GL::Framebuffer))
			{
				// Create Texture
				m_texture.dispose();

				m_texture.create(m_size);

				// Attach to FBO
				m_fbo.setTexture(m_colorID, m_texture);
			}

			// Unbind
			m_fbo.unbind();
		}
		
		return (m_fbo && m_rbo);
	}

	bool Surface::update(const vec2i & size)
	{
		if ((size[0] != 0 && size[1] != 0) && (m_size != size))
		{
			return setSize(size).dispose() && create();
		}
		return false;
	}

	void Surface::draw(const RenderTarget & target, RenderBatch & batch) const
	{
		if (m_model && m_material && m_shader)
		{
			m_shader->bind(false);

			for (const auto & u : (*m_material))
			{
				m_shader->setUniform(u);
			}

			m_shader->setUniform(ML_UNI_MAIN_TEX, m_texture);

			m_shader->bind(true);

			target.draw(m_model, batch);

			m_shader->unbind();
		}
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

	Surface & Surface::setColorID(GL::ColorID value)
	{
		m_colorID = value;
		return (*this);
	}

	Surface & Surface::setFrameID(GL::FrameID value)
	{
		m_frameID = value;
		return (*this);
	}

	Surface & Surface::setMaterial(const_ptr_t<Material> value)
	{
		m_material = value;
		return (*this);
	}

	Surface & Surface::setShader(const_ptr_t<Shader> value)
	{
		m_shader = value;
		return (*this);
	}

	Surface & Surface::setModel(const_ptr_t<Model> value)
	{
		m_model = value;
		return (*this);
	}

	Surface & Surface::setSize(const vec2i & value)
	{
		m_size = value;
		return (*this);
	}

	Surface & Surface::setStorage(GL::Format value)
	{
		m_storage = value;
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}