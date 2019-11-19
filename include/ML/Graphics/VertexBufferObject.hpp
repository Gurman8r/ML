#ifndef _ML_VERTEX_BUFFER_HPP_
#define _ML_VERTEX_BUFFER_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Graphics/GL.hpp>
#include <ML/Core/Trackable.hpp>
#include <ML/Core/Handle.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API VertexBufferObject final : public Trackable, public Handle<uint32_t>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		VertexBufferObject();
		VertexBufferObject(const VertexBufferObject & copy);
		~VertexBufferObject();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		VertexBufferObject & clean();

		VertexBufferObject & create(GL::Usage usage);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const VertexBufferObject & bind() const;
		
		const VertexBufferObject & unbind() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const VertexBufferObject & bufferData(void * data, uint32_t size) const;
		
		const VertexBufferObject & bufferData(const List<float_t> & data) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const VertexBufferObject & bufferSubData(void * data, uint32_t size, uint32_t offset) const;
		
		const VertexBufferObject & bufferSubData(const List<float_t> & data, uint32_t offset) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto	data()	const -> void *			{ return m_data; }
		inline auto	usage() const -> const GL::Usage &	{ return m_usage; }
		inline auto	size()	const -> const uint32_t &	{ return m_size; }
		inline auto	count() const -> const int32_t &	{ return m_count; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		mutable void *	m_data;
		mutable GL::Usage	m_usage;
		mutable uint32_t	m_size;		// total length of contiguous data
		mutable int32_t		m_count;	// number of vertices

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	using VBO = typename VertexBufferObject;

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_VERTEX_BUFFER_HPP_