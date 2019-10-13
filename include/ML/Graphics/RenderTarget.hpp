#ifndef _ML_RENDER_TARGET_HPP_
#define _ML_RENDER_TARGET_HPP_

#include <ML/Graphics/Drawable.hpp>
#include <ML/Graphics/VertexArray.hpp>
#include <ML/Graphics/VertexBuffer.hpp>
#include <ML/Graphics/IndexBuffer.hpp>
#include <ML/Graphics/FrameBuffer.hpp>
#include <ML/Graphics/RenderBuffer.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API RenderTarget
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		virtual ~RenderTarget() {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		RenderTarget & draw(const Drawable * value);
		RenderTarget & draw(const Drawable * value, const RenderBatch & batch);

		/* * * * * * * * * * * * * * * * * * * * */

		RenderTarget & draw(const Drawable & value);
		RenderTarget & draw(const Drawable & value, const RenderBatch & batch);
		
		/* * * * * * * * * * * * * * * * * * * * */

		RenderTarget & draw(const List<Vertex> & verts, const RenderBatch & batch);
		RenderTarget & draw(const List<float_t> & verts, const RenderBatch & batch);
		RenderTarget & draw(const float_t * verts, size_t count, const RenderBatch & batch);
		RenderTarget & draw(const float_t * verts, size_t count, const VAO * vao, const VBO * vbo);
		
		/* * * * * * * * * * * * * * * * * * * * */

		RenderTarget & draw(const VAO & vao, const VBO & vbo, const IBO & ibo);
		RenderTarget & draw(const VAO & vao, const VBO & vbo);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDER_TARGET_HPP_