#include <ML/Graphics/Renderer.hpp>
#include <ML/Graphics/RenderTarget.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Renderer::Renderer()
		: m_drawable(nullptr)
		, m_material(nullptr)
		, m_states()
	{
	}

	Renderer::Renderer(const I_Drawable * drawable, const Material * material)
		: m_drawable(drawable)
		, m_material(material)
		, m_states()
	{
	}

	Renderer::Renderer(const I_Drawable * drawable, const Material * material, const RenderStates & states)
		: m_drawable(drawable)
		, m_material(material)
		, m_states(states)
	{
	}

	Renderer::~Renderer() {}

	/* * * * * * * * * * * * * * * * * * * * */

	void Renderer::draw(RenderTarget & target, RenderBatch batch) const
	{
		if (m_drawable && m_material)
		{
			m_states.apply();

			if (m_material->shader() && (*m_material->shader()))
			{
				m_material->bind();

				target.draw(m_drawable);

				m_material->unbind();
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}