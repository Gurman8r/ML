#include <ML/Graphics/RenderTarget.hpp>
#include <ML/Graphics/OpenGL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RenderTarget & RenderTarget::draw(const Drawable * value)
	{
		return (value) ? this->draw(*value) : (*this);
	}

	RenderTarget & RenderTarget::draw(const Drawable * value, const RenderBatch & batch)
	{
		return (value) ? this->draw(*value, batch) : (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RenderTarget & RenderTarget::draw(const Drawable & value)
	{
		RenderBatch batch;
		return this->draw(value, batch);
	}

	RenderTarget & RenderTarget::draw(const Drawable & value, const RenderBatch & batch)
	{
		value.draw((*this), batch);
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RenderTarget & RenderTarget::draw(const List<Vertex> & verts, const RenderBatch & batch)
	{
		return this->draw(alg::contiguous(verts), batch);
	}

	RenderTarget & RenderTarget::draw(const List<float_t> & verts, const RenderBatch & batch)
	{
		return this->draw(verts.data(), verts.size(), batch);
	}

	RenderTarget & RenderTarget::draw(const float_t * verts, size_t count, const RenderBatch & batch)
	{
		if (batch.mat) { batch.mat->bind(); }

		this->draw(verts, count, batch.vao, batch.vbo);

		if (batch.mat) { batch.mat->unbind(); }

		return (*this);
	}

	RenderTarget & RenderTarget::draw(const float_t * verts, size_t count, const VAO * vao, const VBO * vbo)
	{
		if (vbo) { (*vbo).bind().bufferSubData(verts, (uint32_t)count, 0).unbind(); }

		return ((vao && vbo) ? this->draw((*vao), (*vbo)) : (*this));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RenderTarget & RenderTarget::draw(const VAO & vao, const VBO & vbo, const IBO & ibo)
	{
		if (vao && vbo && ibo)
		{
			vao.bind();
			vbo.bind();
			ibo.bind();
			{
				ML_GL.drawElements(vao.mode(), ibo.count(), ibo.type(), nullptr);
			}
			ibo.unbind();
			vbo.unbind();
			vao.unbind();
		}
		return (*this);
	}
	
	RenderTarget & RenderTarget::draw(const VAO & vao, const VBO & vbo)
	{
		if (vao && vbo)
		{
			vao.bind();
			vbo.bind();
			{
				ML_GL.drawArrays(vao.mode(), 0, vbo.size());
			}
			vbo.unbind();
			vao.unbind();
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}