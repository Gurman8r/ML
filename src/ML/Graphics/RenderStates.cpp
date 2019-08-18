#include <ML/Graphics/RenderStates.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const RenderStates RenderStates::Default { {
			new AlphaState	{ true, GL::Greater, 0.01f },
			new BlendState	{ true, GL::SrcAlpha, GL::OneMinusSrcAlpha },
			new CullState	{ true, GL::Back },
			new DepthState	{ true, GL::Less, true }
		} 
	};

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

	bool RenderStates::dispose()
	{
		if (!m_map.empty())
		{
			for (auto & pair : m_map)
			{
				if (pair.second) { delete pair.second; }
			}
			m_map.clear();
		}
		return m_map.empty();
	}

	const RenderStates & RenderStates::apply() const
	{
		for (const auto & pair : (*this))
		{
			if (pair.second) { (*pair.second)(); }
		}
		return (*this);
	}

	const RenderStates & RenderStates::revert() const
	{
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const AlphaState & AlphaState::operator()() const
	{
		if (!this->enabled)
		{
			ML_GL.disable(GL::AlphaTest);
		}
		else if (ML_GL.enable(GL::AlphaTest, this->enabled))
		{
			ML_GL.alphaFunc(this->predicate, this->coeff);
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const BlendState & BlendState::operator()() const
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

	const CullState & CullState::operator()() const
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

	const DepthState & DepthState::operator()() const
	{
		if (!this->enabled)
		{
			ML_GL.disable(GL::DepthTest);
		}
		else if (ML_GL.enable(GL::DepthTest, this->enabled))
		{
			ML_GL.depthFunc(this->predicate);

			ML_GL.depthMask(this->mask);
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}