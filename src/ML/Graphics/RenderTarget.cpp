#include <ML/Graphics/RenderTarget.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */
	
	RenderTarget & RenderTarget::clear()
	{
		ML_GL.clear(GL::DepthBufferBit);
		return (*this);
	}
	
	RenderTarget & RenderTarget::clear(const vec4 & color)
	{
		ML_GL.clearColor(color[0], color[1], color[2], color[3]);
		ML_GL.clear(GL::ColorBufferBit | GL::DepthBufferBit);
		return (*this);
	}

	RenderTarget & RenderTarget::setViewport(const vec2i & pos, const vec2i & size)
	{
		ML_GL.viewport(pos[0], pos[1], size[0], size[1]);
		return (*this);
	}
	
	/* * * * * * * * * * * * * * * * * * * * */

	RenderTarget & RenderTarget::draw(const IDrawable * value)
	{
		return (value) ? draw(*value) : (*this);
	}

	RenderTarget & RenderTarget::draw(const IDrawable * value, const RenderBatch & batch)
	{
		return (value) ? draw(*value, batch) : (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	RenderTarget & RenderTarget::draw(const IDrawable & value)
	{
		RenderBatch batch;
		return draw(value, batch);
	}

	RenderTarget & RenderTarget::draw(const IDrawable & value, const RenderBatch & batch)
	{
		value.draw((*this), batch);
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	RenderTarget & RenderTarget::draw(const VertexList & vertices, const RenderBatch & batch)
	{
		return draw(vertices.contiguous(), batch);
	}

	RenderTarget & RenderTarget::draw(const List<float> & vertices, const RenderBatch & batch)
	{
		return draw(vertices.data(), vertices.size(), batch);
	}

	RenderTarget & RenderTarget::draw(const float * vertices, const size_t vertexCount, const RenderBatch & batch)
	{
		if (batch.vbo && batch.vbo && batch.mat->bind())
		{
			batch.vbo->bind();
			batch.vbo->bufferSubData(vertices, (uint32_t)(vertexCount), 0);
			batch.vbo->unbind();

			return draw((*batch.vao), (*batch.vbo));
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

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

	/* * * * * * * * * * * * * * * * * * * * */
}