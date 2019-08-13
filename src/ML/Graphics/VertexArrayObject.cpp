#include <ML/Graphics/VertexArrayObject.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	VertexArrayObject::VertexArrayObject()
		: I_Handle(NULL)
	{
	}

	VertexArrayObject::VertexArrayObject(const VertexArrayObject & copy)
		: I_Handle(copy)
	{
	}

	VertexArrayObject::~VertexArrayObject()
	{
		clean();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	VertexArrayObject & VertexArrayObject::clean()
	{
		if ((*this))
		{
			ML_GL.deleteVertexArrays(1, (*this));
		}
		return (*this);
	}

	VertexArrayObject & VertexArrayObject::create(GL::Mode mode)
	{
		if (set_handle(ML_GL.genVertexArrays(1)))
		{
			m_mode = mode;
		}
		return (*this);
	}
	
	/* * * * * * * * * * * * * * * * * * * * */

	const VertexArrayObject & VertexArrayObject::bind() const
	{
		ML_GL.bindVertexArray((*this));
		return (*this);
	}

	const VertexArrayObject & VertexArrayObject::unbind() const
	{
		ML_GL.bindVertexArray(NULL);
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

}