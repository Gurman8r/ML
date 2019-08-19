#ifndef _ML_RENDER_BATCH_HPP_
#define _ML_RENDER_BATCH_HPP_

#include <ML/Graphics/VertexArrayObject.hpp>
#include <ML/Graphics/VertexBufferObject.hpp>
#include <ML/Graphics/Material.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// WORK IN PROGRESS
	struct ML_GRAPHICS_API RenderBatch final
		: public I_Newable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const VAO *	vao;
		const VBO *	vbo;
		Material *	mat;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		RenderBatch()
			: vao(nullptr)
			, vbo(nullptr)
			, mat()
		{
		}

		RenderBatch(const VAO * vao, const VBO * vbo, Material * mat)
			: vao(vao)
			, vbo(vbo)
			, mat(mat)
		{
		}

		RenderBatch(const RenderBatch & copy)
			: vao(copy.vao)
			, vbo(copy.vbo)
			, mat(copy.mat)
		{
		}

		~RenderBatch() {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDER_BATCH_HPP_