#include <ML/Graphics/VertexArrayObject.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	VertexArrayObject::VertexArrayObject()
		: Handle{ NULL }
	{
	}

	VertexArrayObject::VertexArrayObject(VertexArrayObject const & copy)
		: Handle{ copy }
	{
	}

	VertexArrayObject::~VertexArrayObject()
	{
		this->clean();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	VertexArrayObject & VertexArrayObject::clean()
	{
		if (*this)
		{
			ML_GL.deleteVertexArray(*this);

			this->set_handle(NULL);
		}
		return (*this);
	}

	VertexArrayObject & VertexArrayObject::create(GL::Mode mode)
	{
		if (this->set_handle(ML_GL.genVertexArray()))
		{
			m_mode = mode;
		}
		return (*this);
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	VertexArrayObject const & VertexArrayObject::bind() const
	{
		ML_GL.bindVertexArray((*this));
		return (*this);
	}

	VertexArrayObject const & VertexArrayObject::unbind() const
	{
		ML_GL.bindVertexArray(NULL);
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

}