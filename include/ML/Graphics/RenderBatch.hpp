#ifndef _ML_RENDER_BATCH_HPP_
#define _ML_RENDER_BATCH_HPP_

#include <ML/Graphics/VertexArray.hpp>
#include <ML/Graphics/VertexBuffer.hpp>
#include <ML/Graphics/Material.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// WIP - used for rendering Text and Sprites
	struct ML_GRAPHICS_API RenderBatch final : public NonNewable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const VAO *	vao	{ nullptr };
		const VBO *	vbo	{ nullptr };
		Material *	mat	{ nullptr };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDER_BATCH_HPP_