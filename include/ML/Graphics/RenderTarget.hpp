#ifndef _ML_RENDER_TARGET_HPP_
#define _ML_RENDER_TARGET_HPP_

#include <ML/Graphics/I_Drawable.hpp>
#include <ML/Graphics/VertexArray.hpp>
#include <ML/Graphics/VertexBuffer.hpp>
#include <ML/Graphics/IndexBuffer.hpp>
#include <ML/Graphics/FrameBuffer.hpp>
#include <ML/Graphics/RenderBuffer.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API RenderTarget
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		virtual ~RenderTarget() {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		RenderTarget & clear();
		RenderTarget & clear(const vec4 & color);
		RenderTarget & setViewport(const vec2i & pos, const vec2i & size);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		RenderTarget & draw(const I_Drawable * value);
		RenderTarget & draw(const I_Drawable * value, const RenderBatch & batch);

		/* * * * * * * * * * * * * * * * * * * * */

		RenderTarget & draw(const I_Drawable & value);
		RenderTarget & draw(const I_Drawable & value, const RenderBatch & batch);
		
		/* * * * * * * * * * * * * * * * * * * * */

		RenderTarget & draw(const Vertices & verts, const RenderBatch & batch);
		RenderTarget & draw(const List<float> & verts, const RenderBatch & batch);
		RenderTarget & draw(const float * verts, size_t count, const RenderBatch & batch);
		
		/* * * * * * * * * * * * * * * * * * * * */

		RenderTarget & draw(const VAO & vao, const VBO & vbo, const IBO & ibo);
		RenderTarget & draw(const VAO & vao, const VBO & vbo);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDER_TARGET_HPP_