#include <ML/Graphics/BufferLayout.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Graphics/Vertex.hpp>

// BufferLayout::Element
/* * * * * * * * * * * * * * * * * * * * */
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	const BufferLayout BufferLayout::Default({
		{ 0, 3, ml::GL::Float, false, ml::Vertex::Size, 0, sizeof(float) },
		{ 1, 4, ml::GL::Float, false, ml::Vertex::Size, 3, sizeof(float) },
		{ 2, 2, ml::GL::Float, false, ml::Vertex::Size, 7, sizeof(float) },
	});

	/* * * * * * * * * * * * * * * * * * * * */

	BufferLayout::Element::Element()
		: Element(0, 0, GL::Byte, false, 0, 0, 0)
	{
	}

	BufferLayout::Element::Element(uint32_t index, uint32_t size, GL::Type type, bool normalized, uint32_t stride, uint32_t offset, uint32_t width)
		: index		(index)
		, size		(size)
		, type		(type)
		, normalized(normalized)
		, stride	(stride)
		, offset	(offset)
		, width		(width)
	{
	}

	BufferLayout::Element::Element(const Element & copy) : Element(
		copy.index, 
		copy.size, 
		copy.type, 
		copy.normalized, 
		copy.stride, 
		copy.offset, 
		copy.width)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void BufferLayout::Element::use() const
	{
		ML_GL.vertexAttribPointer(
			index,
			size,
			type,
			normalized,
			stride,
			offset,
			width);
		ML_GL.enableVertexAttribArray(index);
	}

	bool BufferLayout::Element::equals(const Element & other) const
	{
		return
			this->index		 == other.index &&
			this->size		 == other.size &&
			this->type		 == other.type &&
			this->normalized == other.normalized &&
			this->stride	 == other.stride &&
			this->offset	 == other.offset &&
			this->width		 == other.width;
	}

	bool BufferLayout::Element::lessThan(const Element & other) const
	{
		return
			this->index		 < other.index ||
			this->size		 < other.size ||
			this->type		 < other.type ||
			this->normalized < other.normalized ||
			this->stride	 < other.stride ||
			this->offset	 < other.offset ||
			this->width		 < other.width;
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
		for (const Element & e : elements())
		{
			e.use();
		}
	}

	BufferLayout & BufferLayout::push_back(const Element & value)
	{
		m_elements.push_back(value);
		return (*this);
	}
	
	/* * * * * * * * * * * * * * * * * * * * */
}