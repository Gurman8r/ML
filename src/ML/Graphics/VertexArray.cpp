#include <ML/Graphics/VertexArray.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	VertexArray::VertexArray()
		: Handle(NULL)
	{
	}

	VertexArray::VertexArray(const VertexArray & copy)
		: Handle(copy)
	{
	}

	VertexArray::~VertexArray()
	{
		clean();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	VertexArray & VertexArray::clean()
	{
		if ((*this))
		{
			ML_GL.deleteVertexArray(*this);

			this->set_handle(NULL);
		}
		return (*this);
	}

	VertexArray & VertexArray::create(GL::Mode mode)
	{
		if (this->set_handle(ML_GL.genVertexArray()))
		{
			m_mode = mode;
		}
		return (*this);
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const VertexArray & VertexArray::bind() const
	{
		ML_GL.bindVertexArray((*this));
		return (*this);
	}

	const VertexArray & VertexArray::unbind() const
	{
		ML_GL.bindVertexArray(NULL);
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

}