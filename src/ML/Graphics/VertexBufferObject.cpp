#include <ML/Graphics/VertexBufferObject.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Graphics/Vertex.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	VertexBufferObject::VertexBufferObject()
		: Handle(NULL)
	{
	}

	VertexBufferObject::VertexBufferObject(VertexBufferObject const & copy)
		: Handle(copy)
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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	VertexBufferObject & VertexBufferObject::clean()
	{
		if ((*this))
		{
			ML_GL.deleteBuffer(*this);

			this->set_handle(NULL);
		}
		return (*this);
	}

	VertexBufferObject & VertexBufferObject::create(GL::Usage usage)
	{
		if (this->set_handle(ML_GL.genBuffer()))
		{
			m_usage = usage;
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	VertexBufferObject const & VertexBufferObject::bind() const
	{
		ML_GL.bindBuffer(GL::ArrayBuffer, (*this));
		return (*this);
	}
	
	VertexBufferObject const & VertexBufferObject::unbind() const
	{
		ML_GL.bindBuffer(GL::ArrayBuffer, NULL);
		return (*this);
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	VertexBufferObject const & VertexBufferObject::bufferData(void * data, uint32_t size) const
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
	
	VertexBufferObject const & VertexBufferObject::bufferData(ArrayList<float_t> const & data) const
	{
		return bufferData((void *)&data[0], (uint32_t)data.size());
	}
	
	VertexBufferObject const & VertexBufferObject::bufferSubData(void * data, uint32_t size, uint32_t offset) const
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
	
	VertexBufferObject const & VertexBufferObject::bufferSubData(ArrayList<float_t> const & data, uint32_t offset) const
	{
		return bufferSubData((void *)&data[0], (uint32_t)data.size(), offset);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}