#ifndef _ML_RENDER_BATCH_HPP_
#define _ML_RENDER_BATCH_HPP_

#include <ML/Graphics/VertexArray.hpp>
#include <ML/Graphics/VertexBuffer.hpp>
#include <ML/Graphics/Material.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Used to render multiple objects with a shared state.
	// Currently only utilized by Sprite and Text.
	struct ML_GRAPHICS_API RenderBatch final : public NonNewable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const VAO * vao	{ nullptr };
		const VBO * vbo	{ nullptr };
		Material * mat	{ nullptr };

		constexpr RenderBatch(const VAO * vao, const VBO * vbo, Material * mat) noexcept
			: vao { vao }
			, vbo { vbo }
			, mat { mat }
		{
		}

		constexpr RenderBatch(const RenderBatch & copy) noexcept
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