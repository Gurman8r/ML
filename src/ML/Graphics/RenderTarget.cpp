#include <ML/Graphics/RenderTarget.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Graphics/ScopedBinder.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const RenderTarget & RenderTarget::draw(Drawable const * value) const
	{
		return (value) ? draw(*value) : (*this);
	}

	const RenderTarget & RenderTarget::draw(Drawable const * value, RenderBatch & batch) const
	{
		return (value) ? draw(*value, batch) : (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const RenderTarget & RenderTarget::draw(const Drawable & value) const
	{
		RenderBatch batch;
		return draw(value, batch);
	}

	const RenderTarget & RenderTarget::draw(const Drawable & value, RenderBatch & batch) const
	{
		value.draw((*this), batch);
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const RenderTarget & RenderTarget::draw(const List<Vertex> & verts, RenderBatch & batch) const
	{
		return draw(alg::contiguous(verts), batch);
	}

	const RenderTarget & RenderTarget::draw(const List<float_t> & verts, RenderBatch & batch) const
	{
		return draw(verts.data(), verts.size(), batch);
	}

	const RenderTarget & RenderTarget::draw(float_t const * verts, size_t count, RenderBatch & batch) const
	{
		return draw(verts, count, batch.vao, batch.vbo);
	}

	const RenderTarget & RenderTarget::draw(float_t const * verts, size_t count, VAO const * vao, VBO const * vbo) const
	{
		if (const ScopedBinder<VBO> b{ vbo })
		{
			b->bufferSubData((void *)verts, (uint32_t)count, 0);
		}
		return (vao && vbo) ? draw(*vao, *vbo) : (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const RenderTarget & RenderTarget::draw(const VAO & vao, const VBO & vbo, const IBO & ibo) const
	{
		if (const ScopedBinder<VAO> a{ vao })
		{
			if (const ScopedBinder<VBO> b{ vbo })
			{
				if (const ScopedBinder<IBO> i{ ibo })
				{
					ML_GL.drawElements(vao.mode(), ibo.count(), ibo.type(), nullptr);
				}
			}
		}
		return (*this);
	}
	
	const RenderTarget & RenderTarget::draw(const VAO & vao, const VBO & vbo) const
	{
		if (const ScopedBinder<VAO> a{ vao })
		{
			if (const ScopedBinder<VBO> b{ vbo })
			{
				ML_GL.drawArrays(vao.mode(), 0, vbo.size());
			}
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}