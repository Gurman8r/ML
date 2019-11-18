#include <ML/Graphics/IndexBufferObject.hpp>
#include <ML/Graphics/OpenGL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	IndexBufferObject::IndexBufferObject()
		: Handle	(NULL)
		, m_data	(NULL)
		, m_count	(NULL)
		, m_usage	(GL::StaticDraw)
		, m_type	(GL::UnsignedInt)
	{
	}

	IndexBufferObject::IndexBufferObject(const IndexBufferObject & copy)
		: Handle	(copy)
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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	IndexBufferObject & IndexBufferObject::clean()
	{
		if ((*this))
		{
			ML_GL.deleteBuffer(*this);

			this->set_handle(NULL);
		}
		return (*this);
	}

	IndexBufferObject & IndexBufferObject::create(GL::Usage usage, GL::Type type)
	{
		if (this->set_handle(ML_GL.genBuffer()))
		{
			m_usage = usage;
			m_type = type;
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const IndexBufferObject & IndexBufferObject::bufferData(const_ptr_t<uint32_t> data, uint32_t count) const
	{
		if (*this)
		{
			m_data	= data;
			m_count = count;
			ML_GL.bufferData(
				GL::ElementArrayBuffer,
				(m_count * sizeof(uint32_t)),
				(voidptr_t)m_data,
				m_usage
			);
		}
		return (*this);
	}

	const IndexBufferObject & IndexBufferObject::bufferData(const List<uint32_t> & data) const
	{
		return bufferData(&data[0], (uint32_t)data.size());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}