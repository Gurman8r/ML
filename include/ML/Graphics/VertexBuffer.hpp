#ifndef _ML_VERTEX_BUFFER_HPP_
#define _ML_VERTEX_BUFFER_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Graphics/GL.hpp>
#include <ML/Core/Newable.hpp>
#include <ML/Core/Handle.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API VertexBuffer final : public Newable, public Handle<uint32_t>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		VertexBuffer();
		VertexBuffer(const VertexBuffer & copy);
		~VertexBuffer();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		VertexBuffer & clean();

		VertexBuffer & create(GL::Usage usage);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const VertexBuffer & bind() const;
		
		const VertexBuffer & unbind() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const VertexBuffer & bufferData(voidptr_t data, uint32_t size) const;
		
		const VertexBuffer & bufferData(const List<float_t> & data) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const VertexBuffer & bufferSubData(voidptr_t data, uint32_t size, uint32_t offset) const;
		
		const VertexBuffer & bufferSubData(const List<float_t> & data, uint32_t offset) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto	data()	const -> voidptr_t			{ return m_data; }
		inline auto	usage() const -> const GL::Usage &	{ return m_usage; }
		inline auto	size()	const -> const uint32_t &	{ return m_size; }
		inline auto	count() const -> const int32_t &	{ return m_count; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		mutable voidptr_t	m_data;
		mutable GL::Usage	m_usage;
		mutable uint32_t	m_size;		// total length of contiguous data
		mutable int32_t		m_count;	// number of vertices

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	using VBO = typename VertexBuffer;

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_VERTEX_BUFFER_HPP_