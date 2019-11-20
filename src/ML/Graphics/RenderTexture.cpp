#include <ML/Graphics/RenderTexture.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Graphics/RenderTarget.hpp>
#include <ML/Graphics/ScopedBinder.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RenderTexture::RenderTexture()
		: RenderTexture { nullptr, nullptr, nullptr }
	{
	}

	RenderTexture::RenderTexture(Model const * model, Material const * material, Shader const * shader)
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

	RenderTexture::RenderTexture(RenderTexture const & copy)
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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool RenderTexture::dispose()
	{
		if (m_fbo) { m_fbo.clean(); }
		if (m_rbo) { m_rbo.clean(); }
		return (!m_fbo && !m_rbo);
	}

	bool RenderTexture::create()
	{
		if (m_fbo || m_rbo || (m_size[0] == 0) || (m_size[1] == 0))
		{
			return false;
		}

		// Setup Framebuffer
		if (ML_BIND_SCOPE_EX(FBO, _fb, m_fbo.create(m_size)))
		{
			// Setup Renderbuffer
			if (ML_BIND_SCOPE_EX(RBO, _rb, m_rbo.create(m_size)))
			{
				_rb->bufferStorage(m_storage);

				_fb->renderBuffer(m_frameID, (uint32_t)(*_rb));
			}

			// Check Framebuffer Status
			if (ML_GL.checkFramebufferStatus(GL::Framebuffer))
			{
				// Setup Texture
				m_texture.dispose();
				
				m_texture.create(m_size);
				
				_fb->texture2D(m_colorID, m_texture);
			}
		}
		
		return (m_fbo && m_rbo);
	}

	bool RenderTexture::update(vec2i const & size)
	{
		if ((size[0] != 0 && size[1] != 0) && (m_size != size))
		{
			return setSize(size).dispose() && create();
		}
		return false;
	}

	void RenderTexture::draw(RenderTarget const & target, RenderBatch & batch) const
	{
		if (ML_BIND_SCOPE_EX(Shader, _sh, m_shader, false))
		{
			if (m_material)
			{
				for (auto const & u : (*m_material))
				{
					_sh->setUniform(u);
				}
			}

			_sh->setUniform(ML_UNI_MAIN_TEX, m_texture);

			_sh->bind(true); // bind textures

			target.draw(m_model, batch);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RenderTexture const & RenderTexture::bind() const
	{
		m_fbo.bind();
		return (*this);
	}
	
	RenderTexture const & RenderTexture::unbind() const
	{
		m_fbo.unbind();
		return (*this);
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RenderTexture & RenderTexture::setColorID(GL::ColorID value)
	{
		m_colorID = value;
		return (*this);
	}

	RenderTexture & RenderTexture::setFrameID(GL::FrameID value)
	{
		m_frameID = value;
		return (*this);
	}

	RenderTexture & RenderTexture::setMaterial(Material const * value)
	{
		m_material = value;
		return (*this);
	}

	RenderTexture & RenderTexture::setShader(Shader const * value)
	{
		m_shader = value;
		return (*this);
	}

	RenderTexture & RenderTexture::setModel(Model const * value)
	{
		m_model = value;
		return (*this);
	}

	RenderTexture & RenderTexture::setSize(vec2i const & value)
	{
		m_size = value;
		return (*this);
	}

	RenderTexture & RenderTexture::setStorage(GL::Format value)
	{
		m_storage = value;
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}