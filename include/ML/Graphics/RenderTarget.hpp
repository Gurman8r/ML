#ifndef _ML_RENDER_TARGET_HPP_
#define _ML_RENDER_TARGET_HPP_

#include <ML/Graphics/Drawable.hpp>
#include <ML/Graphics/VertexArrayObject.hpp>
#include <ML/Graphics/VertexBufferObject.hpp>
#include <ML/Graphics/IndexBufferObject.hpp>
#include <ML/Graphics/FrameBufferObject.hpp>
#include <ML/Graphics/RenderBufferObject.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API RenderTarget
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		virtual ~RenderTarget() {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		RenderTarget const & draw(Drawable const * value) const;
		
		RenderTarget const & draw(Drawable const * value, RenderBatch & batch) const;

		/* * * * * * * * * * * * * * * * * * * * */

		RenderTarget const & draw(Drawable const & value) const;
		
		RenderTarget const & draw(Drawable const & value, RenderBatch & batch) const;
		
		/* * * * * * * * * * * * * * * * * * * * */

		RenderTarget const & draw(const List<Vertex> & verts, RenderBatch & batch) const;
		
		RenderTarget const & draw(const List<float_t> & verts, RenderBatch & batch) const;
		
		RenderTarget const & draw(float_t const * verts, size_t count, RenderBatch & batch) const;
		
		RenderTarget const & draw(float_t const * verts, size_t count, VAO const * vao, VBO const * vbo) const;
		
		/* * * * * * * * * * * * * * * * * * * * */

		RenderTarget const & draw(VAO const & vao, VBO const & vbo, IBO const & ibo) const;
		
		RenderTarget const & draw(VAO const & vao, VBO const & vbo) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDER_TARGET_HPP_