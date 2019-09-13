#include <ML/Graphics/RenderStates.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const RenderStates & RenderStates::get_default()
	{
		static RenderStates temp { {
			new AlphaState	{ true, GL::Greater, 0.01f },
			new BlendState	{ true, GL::SrcAlpha, GL::OneMinusSrcAlpha },
			new CullState	{ true, GL::Back },
			new DepthState	{ true, GL::Less, true }
		} };
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RenderStates::RenderStates() 
		: m_map	{}
	{
	}

	RenderStates::RenderStates(List<mapped_type> && data)
		: RenderStates()
	{
		for (auto & elem : data)
		{
			if (elem && (this->find(elem->get_type_hash()) == this->end()))
			{
				m_map[elem->get_type_hash()] = std::move(elem);
			}
		}
	}

	RenderStates::RenderStates(const RenderStates & copy)
		: RenderStates()
	{
		for (const auto & pair : copy)
		{
			if (pair.second && (this->find(pair.second->get_type_hash()) == this->end()))
			{
				m_map[pair.second->get_type_hash()] = pair.second->clone();
			}
		}
	}

	RenderStates::~RenderStates() { this->dispose(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

	RenderStates * RenderStates::clone() const
	{
		RenderStates * temp { new RenderStates {} };
		for (const auto & pair : (*this))
		{
			if (const RenderSetting * elem { pair.second })
			{
				temp->m_map[elem->get_type_hash()] = elem->clone();
			}
		}
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	AlphaState * AlphaState::clone() const
	{
		return new AlphaState { enabled, predicate, coeff };
	}

	AlphaState * AlphaState::reverted() const
	{
		AlphaState * temp { clone() };
		temp->enabled = !temp->enabled;
		return temp;
	}

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

	BlendState * BlendState::clone() const
	{
		return new BlendState { enabled, srcRGB, srcAlpha, dstRGB, dstAlpha };
	}

	BlendState * BlendState::reverted() const
	{
		BlendState * temp { clone() };
		temp->enabled = !temp->enabled;
		return temp;
	}

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

	CullState * CullState::clone() const
	{
		return new CullState { enabled, face };
	}

	CullState * CullState::reverted() const
	{
		CullState * temp { clone() };
		temp->enabled = !temp->enabled;
		return temp;
	}

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

	DepthState * DepthState::clone() const
	{
		return new DepthState { enabled, predicate, mask };
	}

	DepthState * DepthState::reverted() const
	{
		DepthState * temp { clone() };
		temp->enabled = !temp->enabled;
		return temp;
	}

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