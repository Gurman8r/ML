#include <ML/Graphics/Renderer.hpp>
#include <ML/Graphics/RenderTarget.hpp>
#include <ML/Graphics/ScopedBinder.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Renderer::Renderer()
		: m_enabled { true }
		, m_model	{ nullptr }
		, m_material{ nullptr }
		, m_shader	{ nullptr }
		, m_states	{}
	{
	}

	Renderer::Renderer(Model const * model, Material const * material, Shader const * shader)
		: m_enabled	{ true }
		, m_model	{ model }
		, m_material{ material }
		, m_shader	{ shader }
		, m_states	{}
	{
	}

	Renderer::Renderer(Model const * model, Material const * material, Shader const * shader, RenderStates const & states)
		: m_enabled	{ true }
		, m_model	{ model }
		, m_material{ material }
		, m_shader	{ shader }
		, m_states	{ states }
	{
	}

	Renderer::~Renderer() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Renderer & Renderer::setModel(Model const * value)
	{
		m_model = value;
		return (*this);
	}

	Renderer & Renderer::setEnabled(bool value)
	{
		m_enabled = value;
		return (*this);
	}

	Renderer & Renderer::setMaterial(Material const * value)
	{
		m_material = value;
		return (*this);
	}

	Renderer & Renderer::setShader(Shader const * value)
	{
		m_shader = value;
		return (*this);
	}

	Renderer & Renderer::setStates(RenderStates const & value)
	{
		m_states = value;
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Renderer::draw(RenderTarget const & target, RenderBatch & batch) const
	{
		if (m_enabled && m_model && m_material && m_shader)
		{
			m_states();

			if (ML_BIND_SCOPE_EX(Shader, _sh, m_shader, false))
			{
				for (auto const & u : (*m_material))
				{
					_sh->setUniform(u);
				}
				
				_sh->bind(true); // bind textures

				target.draw(m_model, batch);
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}