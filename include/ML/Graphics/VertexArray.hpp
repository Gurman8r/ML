#ifndef _ML_VERTEX_ARRAY_HPP_
#define _ML_VERTEX_ARRAY_HPP_

#include <ML/Graphics/GL.hpp>
#include <ML/Core/INewable.hpp>
#include <ML/Core/List.hpp>
#include <ML/Core/IHandle.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API VertexArray final
		: public INewable
		, public IHandle<uint32_t>
	{
	public:
		VertexArray();
		VertexArray(const VertexArray & copy);
		~VertexArray();

	public:
		VertexArray & clean();
		VertexArray & create(GL::Mode mode);

	public:
		const VertexArray & bind() const;
		const VertexArray & unbind() const;

	public:
		inline const GL::Mode & mode()	const { return m_mode; }

	private:
		mutable GL::Mode m_mode;
	};

	/* * * * * * * * * * * * * * * * * * * * */

	using VAO = typename VertexArray;

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_VERTEX_ARRAY_HPP_