#include <ML/Graphics/Renderer.hpp>
#include <ML/Graphics/RenderTarget.hpp>
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

	Renderer::Renderer(const Model * model, const Material * material, const Shader * shader)
		: m_enabled	{ true }
		, m_model	{ model }
		, m_material{ material }
		, m_shader	{ shader }
		, m_states	{ }
	{
	}

	Renderer::Renderer(const Model * model, const Material * material, const Shader * shader, const RenderStates & states)
		: m_enabled	{ true }
		, m_model	{ model }
		, m_material{ material }
		, m_shader	{ shader }
		, m_states	{ states }
	{
	}

	Renderer::~Renderer() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Renderer & Renderer::setModel(const Model * value)
	{
		m_model = value;
		return (*this);
	}

	Renderer & Renderer::setEnabled(bool value)
	{
		m_enabled = value;
		return (*this);
	}

	Renderer & Renderer::setMaterial(const Material * value)
	{
		m_material = value;
		return (*this);
	}

	Renderer & Renderer::setShader(const Shader * value)
	{
		m_shader = value;
		return (*this);
	}

	Renderer & Renderer::setStates(const RenderStates & value)
	{
		m_states = value;
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Renderer::draw(const RenderTarget & target, RenderBatch & batch) const
	{
		if (m_enabled && m_model && m_material && m_shader)
		{
			m_states();

			m_shader->bind(false);

			for (const auto & u : (*m_material))
			{
				m_shader->setUniform(u);
			}
			
			m_shader->bind(true);

			target.draw(m_model, batch);

			m_shader->unbind();
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}