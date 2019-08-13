#include <ML/Graphics/IndexBufferObject.hpp>
#include <ML/Graphics/OpenGL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	IndexBufferObject::IndexBufferObject()
		: I_Handle	(NULL)
		, m_data	(NULL)
		, m_count	(NULL)
		, m_usage	(GL::StaticDraw)
		, m_type	(GL::UnsignedInt)
	{
	}

	IndexBufferObject::IndexBufferObject(const IndexBufferObject & copy)
		: I_Handle	(copy)
		, m_data	(copy.m_data)
		, m_count	(copy.m_count)
		, m_usage	(copy.m_usage)
		, m_type	(copy.m_type)
	{
	}

	IndexBufferObject::~IndexBufferObject()
	{
		clean();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	IndexBufferObject & IndexBufferObject::clean()
	{
		if ((*this))
		{
			ML_GL.deleteBuffers(1, (*this));
		}
		return (*this);
	}

	IndexBufferObject & IndexBufferObject::create(GL::Usage usage, GL::Type type)
	{
		if (set_handle(ML_GL.genBuffers(1)))
		{
			m_usage = usage;
			m_type = type;
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	const IndexBufferObject & IndexBufferObject::bind() const
	{
		ML_GL.bindBuffer(GL::ElementArrayBuffer, (*this));
		return (*this);
	}

	const IndexBufferObject & IndexBufferObject::unbind() const
	{
		ML_GL.bindBuffer(GL::ElementArrayBuffer, NULL);
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	const IndexBufferObject & IndexBufferObject::bufferData(const uint32_t * data, uint32_t count) const
	{
		if (*this)
		{
			m_data	= data;
			m_count = count;
			ML_GL.bufferData(
				GL::ElementArrayBuffer,
				(m_count * sizeof(uint32_t)),
				m_data,
				m_usage
			);
		}
		return (*this);
	}

	const IndexBufferObject & IndexBufferObject::bufferData(const List<uint32_t> & data) const
	{
		return bufferData(&data[0], (uint32_t)data.size());
	}

	/* * * * * * * * * * * * * * * * * * * * */
}