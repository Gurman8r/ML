#include <ML/Graphics/Renderer.hpp>
#include <ML/Graphics/RenderTarget.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Renderer::Renderer()
		: m_enabled	(true)
		, m_model	(nullptr)
		, m_material(nullptr)
		, m_states	()
	{
	}

	Renderer::Renderer(const Model * model, const Material * material)
		: m_enabled	(true)
		, m_model	(model)
		, m_material(material)
		, m_states	()
	{
	}

	Renderer::Renderer(const Model * model, const Material * material, const RenderStates & states)
		: m_enabled	(true)
		, m_model	(model)
		, m_material(material)
		, m_states	(states)
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
		if (auto m { material() })
		{
			m->setShader(value);
		}
		return (*this);
	}

	Renderer & Renderer::setStates(const RenderStates & value)
	{
		m_states = value;
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Renderer::draw(const RenderTarget & target, RenderBatch batch) const
	{
		if (m_enabled && m_model && m_material)
		{
			m_states.apply();

			m_material->bind();

			target.draw(m_model);

			m_material->unbind();
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}