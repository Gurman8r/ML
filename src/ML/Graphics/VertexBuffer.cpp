#include <ML/Graphics/VertexBuffer.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Graphics/Vertex.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	VertexBuffer::VertexBuffer()
		: I_Handle(NULL)
	{
	}

	VertexBuffer::VertexBuffer(const VertexBuffer & copy)
		: I_Handle(copy)
		, m_usage(copy.m_usage)
		, m_data(copy.m_data)
		, m_size(copy.m_size)
		, m_count(copy.m_count)
	{
	}

	VertexBuffer::~VertexBuffer()
	{
		clean();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	VertexBuffer & VertexBuffer::clean()
	{
		if ((*this))
		{
			ML_GL.deleteBuffers(1, (*this));
		}
		return (*this);
	}

	VertexBuffer & VertexBuffer::create(GL::Usage usage)
	{
		if (set_handle(ML_GL.genBuffers(1)))
		{
			m_usage = usage;
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	const VertexBuffer & VertexBuffer::bind() const
	{
		ML_GL.bindBuffer(GL::ArrayBuffer, (*this));
		return (*this);
	}
	
	const VertexBuffer & VertexBuffer::unbind() const
	{
		ML_GL.bindBuffer(GL::ArrayBuffer, NULL);
		return (*this);
	}
	
	/* * * * * * * * * * * * * * * * * * * * */
	
	const VertexBuffer & VertexBuffer::bufferData(const void * data, uint32_t size) const
	{
		m_data = data;
		m_size = size;
		m_count = size / Vertex::Size;
		ML_GL.bufferData(GL::ArrayBuffer, (sizeof(float) * m_size), m_data, m_usage);
		return (*this);
	}
	
	const VertexBuffer & VertexBuffer::bufferData(const List<float> & data) const
	{
		return bufferData(&data[0], (uint32_t)data.size());
	}
	
	const VertexBuffer & VertexBuffer::bufferSubData(const void * data, uint32_t size, uint32_t offset) const
	{
		m_data = data;
		m_size = size;
		m_count = size / Vertex::Size;
		ML_GL.bufferSubData(GL::ArrayBuffer, offset, (sizeof(float) * m_size), m_data);
		return (*this);
	}
	
	const VertexBuffer & VertexBuffer::bufferSubData(const List<float> & data, uint32_t offset) const
	{
		return bufferSubData(&data[0], (uint32_t)data.size(), offset);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}