#ifndef _ML_VERTEX_ARRAY_HPP_
#define _ML_VERTEX_ARRAY_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Graphics/GL.hpp>
#include <ML/Core/Trackable.hpp>
#include <ML/Core/Handle.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API VertexArrayObject final : public Trackable, public Handle<uint32_t>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		VertexArrayObject();
		VertexArrayObject(const VertexArrayObject & copy);
		~VertexArrayObject();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		VertexArrayObject & clean();

		VertexArrayObject & create(GL::Mode mode);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const VertexArrayObject & bind() const;

		const VertexArrayObject & unbind() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto mode() const -> const GL::Mode & { return m_mode; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		mutable GL::Mode m_mode;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	using VAO = typename VertexArrayObject;

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_VERTEX_ARRAY_HPP_