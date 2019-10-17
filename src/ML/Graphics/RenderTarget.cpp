#include <ML/Graphics/RenderTarget.hpp>
#include <ML/Graphics/OpenGL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const RenderTarget & RenderTarget::draw(const Drawable * value) const
	{
		return (value) ? draw(*value) : (*this);
	}

	const RenderTarget & RenderTarget::draw(const Drawable * value, RenderBatch & batch) const
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

	const RenderTarget & RenderTarget::draw(const float_t * verts, size_t count, RenderBatch & batch) const
	{
		if (batch.mat) { batch.mat->bind(); }

		draw(verts, count, batch.vao, batch.vbo);

		if (batch.mat) { batch.mat->unbind(); }

		return (*this);
	}

	const RenderTarget & RenderTarget::draw(const float_t * verts, size_t count, const VAO * vao, const VBO * vbo) const
	{
		if (vbo) { (*vbo).bind().bufferSubData(verts, (uint32_t)count, 0).unbind(); }

		return ((vao && vbo) ? draw((*vao), (*vbo)) : (*this));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const RenderTarget & RenderTarget::draw(const VAO & vao, const VBO & vbo, const IBO & ibo) const
	{
		if (vao && vbo && ibo)
		{
			vao.bind();
			vbo.bind();
			ibo.bind();
			ML_GL.drawElements(vao.mode(), ibo.count(), ibo.type(), nullptr);
			ibo.unbind();
			vbo.unbind();
			vao.unbind();
		}
		return (*this);
	}
	
	const RenderTarget & RenderTarget::draw(const VAO & vao, const VBO & vbo) const
	{
		if (vao && vbo)
		{
			vao.bind();
			vbo.bind();
			ML_GL.drawArrays(vao.mode(), 0, vbo.size());
			vbo.unbind();
			vao.unbind();
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}