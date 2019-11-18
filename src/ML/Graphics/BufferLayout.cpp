#include <ML/Graphics/BufferLayout.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Graphics/Vertex.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const BufferLayout::Element & BufferLayout::Element::operator()() const
	{
		ML_GL.vertexAttribPointer(index, size, type, normalize, stride, offset, width);
		ML_GL.enableVertexAttribArray(index);
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	BufferLayout::BufferLayout()
		: m_elements {}
	{
	}

	BufferLayout::BufferLayout(const List<Element> & elements)
		: m_elements { elements }
	{
	}

	BufferLayout::BufferLayout(const std::initializer_list<Element> & elements)
		: m_elements { elements.begin(), elements.end() }
	{
	}

	BufferLayout::BufferLayout(const BufferLayout & copy)
		: m_elements { copy.m_elements }
	{
	}

	BufferLayout::~BufferLayout() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const BufferLayout & BufferLayout::get_default()
	{
		static BufferLayout temp {
			{ 0, 3, GL::Float, false, Vertex::Size, 0, sizeof(float_t) },
			{ 1, 4, GL::Float, false, Vertex::Size, 3, sizeof(float_t) },
			{ 2, 2, GL::Float, false, Vertex::Size, 7, sizeof(float_t) },
		};
		return temp;
	}

	const BufferLayout & BufferLayout::bind() const
	{
		for (const auto & elem : this->elements()) 
		{ 
			elem();
		}
		return (*this);
	}

	BufferLayout & BufferLayout::push_back(const Element & value)
	{
		m_elements.push_back(value);
		return (*this);
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}