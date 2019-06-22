#include <ML/Graphics/VertexArray.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	VertexArray::VertexArray()
		: I_Handle(NULL)
	{
	}

	VertexArray::VertexArray(const VertexArray & copy)
		: I_Handle(copy)
	{
	}

	VertexArray::~VertexArray()
	{
		clean();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	VertexArray & VertexArray::clean()
	{
		if ((*this))
		{
			ML_GL.deleteVertexArrays(1, (*this));
		}
		return (*this);
	}

	VertexArray & VertexArray::create(GL::Mode mode)
	{
		if (set_handle(ML_GL.genVertexArrays(1)))
		{
			m_mode = mode;
		}
		return (*this);
	}
	
	/* * * * * * * * * * * * * * * * * * * * */

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

	/* * * * * * * * * * * * * * * * * * * * */

}