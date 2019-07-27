#include <ML/Graphics/RenderStates.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RenderStates::RenderStates() : m_map() {}

	RenderStates::RenderStates(List<mapped_type> && data)
		: RenderStates()
	{
		for (auto & elem : data)
		{
			if (elem && (this->find(elem->get_id()) == this->end()))
			{
				m_map[elem->get_id()] = std::move(elem);
			}
		}
	}

	RenderStates::RenderStates(const RenderStates & copy)
		: RenderStates()
	{
		for (const auto & pair : copy)
		{
			if (pair.second && (this->find(pair.second->get_id()) == this->end()))
			{
				m_map[pair.second->get_id()] = pair.second->clone();
			}
		}
	}

	RenderStates::~RenderStates() { this->dispose(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const AlphaTest & AlphaTest::operator()() const
	{
		if (!this->enabled)
		{
			ML_GL.disable(GL::AlphaTest);
		}
		else if (ML_GL.enable(GL::AlphaTest, this->enabled))
		{
			ML_GL.alphaFunc(this->comp, this->coeff);
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const BlendFunc & BlendFunc::operator()() const
	{
		if (!this->enabled)
		{
			ML_GL.disable(GL::Blend);
		}
		else if (ML_GL.enable(GL::Blend, this->enabled))
		{
			ML_GL.blendFuncSeparate(
				this->srcRGB, this->srcAlpha,
				this->dstRGB, this->dstAlpha
			);
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const CullFace & CullFace::operator()() const
	{
		if (!this->enabled)
		{
			ML_GL.disable(GL::CullFace);
		}
		else if (ML_GL.enable(GL::CullFace, this->enabled))
		{
			ML_GL.cullFace(this->face);
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const DepthMask & DepthMask::operator()() const
	{
		ML_GL.depthMask(this->enabled);
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const DepthTest & DepthTest::operator()() const
	{
		if (!this->enabled)
		{
			ML_GL.disable(GL::DepthTest);
		}
		else if (ML_GL.enable(GL::DepthTest, this->enabled))
		{
			ML_GL.depthFunc(this->comp);
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const PolygonMode & PolygonMode::operator()() const
	{
		if (this->enabled)
		{
			ML_GL.polygonMode(this->face, this->mode);
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const ScissorTest & ScissorTest::operator()() const
	{
		if (!this->enabled)
		{
			ML_GL.disable(GL::ScissorTest);
		}
		else if (ML_GL.enable(GL::ScissorTest, this->enabled))
		{
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}