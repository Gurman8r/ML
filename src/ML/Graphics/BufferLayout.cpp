#include <ML/Graphics/BufferLayout.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Graphics/Vertex.hpp>

// BufferLayout::Element
/* * * * * * * * * * * * * * * * * * * * */
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	const BufferLayout BufferLayout::Default({
		{ 0, 3, GL::Float, false, Vertex::Size, 0, sizeof(float_t) },
		{ 1, 4, GL::Float, false, Vertex::Size, 3, sizeof(float_t) },
		{ 2, 2, GL::Float, false, Vertex::Size, 7, sizeof(float_t) },
	});

	/* * * * * * * * * * * * * * * * * * * * */

	const BufferLayout::Element & BufferLayout::Element::operator()() const
	{
		ML_GL.vertexAttribPointer(
			index,
			size,
			type,
			normalized,
			stride,
			offset,
			width
		);
		ML_GL.enableVertexAttribArray(index);
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

// BufferLayout
/* * * * * * * * * * * * * * * * * * * * */
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	BufferLayout::BufferLayout()
		: BufferLayout(List<Element>())
	{
	}

	BufferLayout::BufferLayout(const List<Element> & elements)
		: m_elements(elements)
	{
	}

	BufferLayout::BufferLayout(const Initializer<Element> & elements)
		: m_elements(elements.begin(), elements.end())
	{
	}

	BufferLayout::BufferLayout(const BufferLayout & copy)
		: BufferLayout(copy.m_elements)
	{
	}

	BufferLayout::~BufferLayout()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */
	
	void BufferLayout::bind() const
	{
		for (const Element & elem : this->elements())
		{
			elem();
		}
	}

	BufferLayout & BufferLayout::push_back(const Element & value)
	{
		m_elements.push_back(value);
		return (*this);
	}
	
	/* * * * * * * * * * * * * * * * * * * * */
}