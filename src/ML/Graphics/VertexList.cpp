#include <ML/Graphics/VertexList.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	VertexList::VertexList()
		: base_type()
	{
	}

	VertexList::VertexList(const allocator_type & alloc)
		: base_type(alloc)
	{
	}

	VertexList::VertexList(const size_type count, const allocator_type & alloc)
		: base_type(count, alloc)
	{
	}

	VertexList::VertexList(const base_type & value)
		: base_type(value)
	{
	}

	VertexList::VertexList(const init_type & value)
		: base_type(value)
	{
	}

	VertexList::VertexList(const self_type & value)
		: base_type(value)
	{
	}

	VertexList::~VertexList() {}

	/* * * * * * * * * * * * * * * * * * * * */

	const List<float> & VertexList::contiguous() const
	{
		if (const size_type imax = ((*this).size() * Vertex::Size))
		{
			m_contiguous.resize(imax);

			for (size_type i = 0; i < imax; i++)
			{
				m_contiguous[i] = (*this)[i / Vertex::Size][i % Vertex::Size];
			}
		}
		else if (!m_contiguous.empty())
		{
			m_contiguous.clear();
		}
		return m_contiguous;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}