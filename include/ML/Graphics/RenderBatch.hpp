#ifndef _ML_RENDER_BATCH_HPP_
#define _ML_RENDER_BATCH_HPP_

#include <ML/Graphics/VertexArrayObject.hpp>
#include <ML/Graphics/VertexBufferObject.hpp>
#include <ML/Graphics/Material.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Used to render multiple objects with a shared state.
	// Currently only utilized by Sprite and Text.
	struct ML_GRAPHICS_API RenderBatch final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		VAO const *	vao	{ nullptr };
		VBO const *	vbo	{ nullptr };
		Material *	mat	{ nullptr };

		constexpr RenderBatch(VAO const * vao, VBO const * vbo, Material * mat) noexcept
			: vao { vao }
			, vbo { vbo }
			, mat { mat }
		{
		}

		constexpr RenderBatch(RenderBatch const & copy) noexcept
			: RenderBatch { copy.vao, copy.vbo, copy.mat }
		{
		}

		constexpr RenderBatch(RenderBatch && copy) noexcept
			: RenderBatch { copy.vao, copy.vbo, copy.mat }
		{
		}

		constexpr RenderBatch() noexcept
			: RenderBatch { nullptr, nullptr, nullptr }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDER_BATCH_HPP_