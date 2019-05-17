#include <ML/Graphics/Canvas.hpp>
#include <ML/Graphics/BufferLayout.hpp>
#include <ML/Graphics/Shapes.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Canvas::Canvas()
		: m_vao()
		, m_vbo()
	{
	}

	Canvas::~Canvas()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Canvas::create()
	{
		if (!m_vao && !m_vbo)
		{
			m_vao.create(GL::Triangles).bind();
			m_vbo.create(GL::DynamicDraw).bind();
			m_vbo.bufferData(NULL, Shapes::RectQuad::Size);
			BufferLayout::Default.bind();
			m_vbo.unbind();
			m_vao.unbind();
			return (m_vao && m_vbo);
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}