#include <ML/Graphics/RenderStates.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const RenderStates & RenderStates::apply() const
	{
		m_alpha();
		m_blend();
		m_cull();
		m_depth();
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const AlphaState & AlphaState::operator()() const
	{
		if (!this->enabled)
		{
			ML_GL.disable(GL::AlphaTest);
		}
		else if (ML_GL.enable(GL::AlphaTest, this->enabled))
		{
			if (this->predicate)
			{
				ML_GL.alphaFunc(this->predicate, this->coeff);
			}
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const BlendState & BlendState::operator()() const
	{
		if (!this->enabled)
		{
			ML_GL.disable(GL::Blend);
		}
		else if (ML_GL.enable(GL::Blend, this->enabled))
		{
			if (this->srcRGB && this->srcAlpha && this->dstRGB && this->dstAlpha)
			{
				ML_GL.blendFuncSeparate(
					this->srcRGB, this->srcAlpha,
					this->dstRGB, this->dstAlpha
				);
			}
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const CullState & CullState::operator()() const
	{
		if (!this->enabled)
		{
			ML_GL.disable(GL::CullFace);
		}
		else if (ML_GL.enable(GL::CullFace, this->enabled))
		{
			if (this->face)
			{
				ML_GL.cullFace(this->face);
			}
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const DepthState & DepthState::operator()() const
	{
		if (!this->enabled)
		{
			ML_GL.disable(GL::DepthTest);
		}
		else if (ML_GL.enable(GL::DepthTest, this->enabled))
		{
			if (this->predicate)
			{
				ML_GL.depthFunc(this->predicate);
			}

			ML_GL.depthMask(this->mask);
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}