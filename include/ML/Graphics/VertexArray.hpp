#ifndef _ML_VERTEX_ARRAY_HPP_
#define _ML_VERTEX_ARRAY_HPP_

#include <ML/Graphics/GL.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/List.hpp>
#include <ML/Core/I_Handle.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API VertexArray final
		: public I_Newable
		, public I_Handle<uint32_t>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		VertexArray();
		VertexArray(const VertexArray & copy);
		~VertexArray();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		VertexArray & clean();
		VertexArray & create(GL::Mode mode);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const VertexArray & bind() const;
		const VertexArray & unbind() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto mode() const -> const GL::Mode & { return m_mode; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		mutable GL::Mode m_mode;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	using VAO = typename VertexArray;

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_VERTEX_ARRAY_HPP_