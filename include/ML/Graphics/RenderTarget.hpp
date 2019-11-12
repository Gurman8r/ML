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
		
		const RenderTarget & draw(const_ptr_t<Drawable> value) const;
		
		const RenderTarget & draw(const_ptr_t<Drawable> value, RenderBatch & batch) const;

		/* * * * * * * * * * * * * * * * * * * * */

		const RenderTarget & draw(const Drawable & value) const;
		
		const RenderTarget & draw(const Drawable & value, RenderBatch & batch) const;
		
		/* * * * * * * * * * * * * * * * * * * * */

		const RenderTarget & draw(const List<Vertex> & verts, RenderBatch & batch) const;
		
		const RenderTarget & draw(const List<float_t> & verts, RenderBatch & batch) const;
		
		const RenderTarget & draw(const_ptr_t<float_t> verts, size_t count, RenderBatch & batch) const;
		
		const RenderTarget & draw(const_ptr_t<float_t> verts, size_t count, const VAO * vao, const VBO * vbo) const;
		
		/* * * * * * * * * * * * * * * * * * * * */

		const RenderTarget & draw(const VAO & vao, const VBO & vbo, const IBO & ibo) const;
		
		const RenderTarget & draw(const VAO & vao, const VBO & vbo) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDER_TARGET_HPP_