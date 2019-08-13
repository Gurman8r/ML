#include <ML/Graphics/VertexBufferObject.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Graphics/Vertex.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	VertexBufferObject::VertexBufferObject()
		: I_Handle(NULL)
	{
	}

	VertexBufferObject::VertexBufferObject(const VertexBufferObject & copy)
		: I_Handle(copy)
		, m_usage(copy.m_usage)
		, m_data(copy.m_data)
		, m_size(copy.m_size)
		, m_count(copy.m_count)
	{
	}

	VertexBufferObject::~VertexBufferObject()
	{
		clean();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	VertexBufferObject & VertexBufferObject::clean()
	{
		if ((*this))
		{
			ML_GL.deleteBuffers(1, (*this));
		}
		return (*this);
	}

	VertexBufferObject & VertexBufferObject::create(GL::Usage usage)
	{
		if (set_handle(ML_GL.genBuffers(1)))
		{
			m_usage = usage;
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	const VertexBufferObject & VertexBufferObject::bind() const
	{
		ML_GL.bindBuffer(GL::ArrayBuffer, (*this));
		return (*this);
	}
	
	const VertexBufferObject & VertexBufferObject::unbind() const
	{
		ML_GL.bindBuffer(GL::ArrayBuffer, NULL);
		return (*this);
	}
	
	/* * * * * * * * * * * * * * * * * * * * */
	
	const VertexBufferObject & VertexBufferObject::bufferData(const void * data, uint32_t size) const
	{
		if (*this)
		{
			m_data	= data;
			m_size	= size;
			m_count = size / Vertex::Size;
			ML_GL.bufferData(
				GL::ArrayBuffer, 
				(sizeof(float_t) * m_size), 
				m_data, 
				m_usage
			);
		}
		return (*this);
	}
	
	const VertexBufferObject & VertexBufferObject::bufferData(const List<float_t> & data) const
	{
		return bufferData(&data[0], (uint32_t)data.size());
	}
	
	const VertexBufferObject & VertexBufferObject::bufferSubData(const void * data, uint32_t size, uint32_t offset) const
	{
		if (*this)
		{
			m_data	= data;
			m_size	= size;
			m_count = size / Vertex::Size;
			ML_GL.bufferSubData(
				GL::ArrayBuffer,
				offset,
				(sizeof(float_t) * m_size),
				m_data
			);
		}
		return (*this);
	}
	
	const VertexBufferObject & VertexBufferObject::bufferSubData(const List<float_t> & data, uint32_t offset) const
	{
		return bufferSubData(&data[0], (uint32_t)data.size(), offset);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}