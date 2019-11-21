#ifndef _ML_VERTEX_BUFFER_HPP_
#define _ML_VERTEX_BUFFER_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Graphics/GL.hpp>
#include <ML/Core/MemoryTracker.hpp>
#include <ML/Core/Handle.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API VertexBufferObject final : public Trackable, public Handle<uint32_t>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		VertexBufferObject();
		VertexBufferObject(VertexBufferObject const & copy);
		~VertexBufferObject();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		VertexBufferObject & clean();

		VertexBufferObject & create(GL::Usage usage);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		VertexBufferObject const & bind() const;
		
		VertexBufferObject const & unbind() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		VertexBufferObject const & bufferData(void * data, uint32_t size) const;
		
		VertexBufferObject const & bufferData(ArrayList<float_t> const & data) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		VertexBufferObject const & bufferSubData(void * data, uint32_t size, uint32_t offset) const;
		
		VertexBufferObject const & bufferSubData(ArrayList<float_t> const & data, uint32_t offset) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto	data()	const -> void * { return m_data; }
		
		inline auto	usage() const -> GL::Usage { return m_usage; }
		
		inline auto	size()	const -> uint32_t { return m_size; }
		
		inline auto	count() const -> int32_t { return m_count; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		GL::Usage			m_usage;
		mutable void *		m_data;
		mutable uint32_t	m_size;		// total length of contiguous data
		mutable int32_t		m_count;	// number of vertices

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	ML_USING VBO = typename VertexBufferObject;

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_VERTEX_BUFFER_HPP_