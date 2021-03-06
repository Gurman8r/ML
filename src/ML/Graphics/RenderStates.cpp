#include <ML/Graphics/RenderStates.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RenderStates const & RenderStates::operator()() const
	{
		if (m_enabled)
		{
			m_alpha();
			m_blend();
			m_cull();
			m_depth();
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	AlphaState const & AlphaState::operator()() const
	{
		if (!this->enabled)
		{
			ML_GL.disable(GL::AlphaTest);
		}
		else if (ML_GL.enable(GL::AlphaTest, this->enabled))
		{
			ML_GL.alphaFunc(this->func, this->coeff);
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	BlendState const & BlendState::operator()() const
	{
		if (!this->enabled)
		{
			ML_GL.disable(GL::Blend);
		}
		else if (ML_GL.enable(GL::Blend, this->enabled))
		{
			ML_GL.blendFuncSeparate(
				this->sfactorRGB, this->sfactorAlpha,
				this->dfactorRGB, this->dfactorAlpha
			);
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	CullState const & CullState::operator()() const
	{
		if (!this->enabled)
		{
			ML_GL.disable(GL::CullFace);
		}
		else if (ML_GL.enable(GL::CullFace, this->enabled))
		{
			ML_GL.cullFace(this->mode);
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	DepthState const & DepthState::operator()() const
	{
		if (!this->enabled)
		{
			ML_GL.disable(GL::DepthTest);
		}
		else if (ML_GL.enable(GL::DepthTest, this->enabled))
		{
			ML_GL.depthFunc(this->func);

			ML_GL.depthMask(this->mask);
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}