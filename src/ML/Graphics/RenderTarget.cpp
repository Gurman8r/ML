#include <ML/Graphics/RenderTarget.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Graphics/ScopedBinder.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RenderTarget const & RenderTarget::draw(Drawable const * value) const
	{
		return (value) ? draw(*value) : (*this);
	}

	RenderTarget const & RenderTarget::draw(Drawable const * value, RenderBatch & batch) const
	{
		return (value) ? draw(*value, batch) : (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RenderTarget const & RenderTarget::draw(Drawable const & value) const
	{
		RenderBatch batch;
		return draw(value, batch);
	}

	RenderTarget const & RenderTarget::draw(Drawable const & value, RenderBatch & batch) const
	{
		value.draw((*this), batch);
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RenderTarget const & RenderTarget::draw(std::vector<Vertex> const & verts, RenderBatch & batch) const
	{
		return draw(alg::contiguous(verts), batch);
	}

	RenderTarget const & RenderTarget::draw(std::vector<float_t> const & verts, RenderBatch & batch) const
	{
		return draw(verts.data(), verts.size(), batch);
	}

	RenderTarget const & RenderTarget::draw(float_t const * verts, size_t count, RenderBatch & batch) const
	{
		return draw(verts, count, batch.vao, batch.vbo);
	}

	RenderTarget const & RenderTarget::draw(float_t const * verts, size_t count, VAO const * vao, VBO const * vbo) const
	{
		if (ML_BIND_SCOPE_EX(VBO, _vb, vbo))
		{
			_vb->bufferSubData((void *)verts, (uint32_t)count, 0);
		}
		return (vao && vbo) ? draw(*vao, *vbo) : (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RenderTarget const & RenderTarget::draw(VAO const & vao, VBO const & vbo, IBO const & ibo) const
	{
		if (ML_BIND_SCOPE(VAO, vao))
		{
			if (ML_BIND_SCOPE(VBO, vbo))
			{
				if (ML_BIND_SCOPE(IBO, ibo))
				{
					ML_GL.drawElements(vao.mode(), ibo.count(), ibo.type(), nullptr);

					ML_GL.flush();
				}
			}
		}
		return (*this);
	}
	
	RenderTarget const & RenderTarget::draw(VAO const & vao, VBO const & vbo) const
	{
		if (ML_BIND_SCOPE(VAO, vao))
		{
			if (ML_BIND_SCOPE(VBO, vbo))
			{
				ML_GL.drawArrays(vao.mode(), 0, vbo.size());

				ML_GL.flush();
			}
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}