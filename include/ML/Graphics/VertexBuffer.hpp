#ifndef _ML_VERTEX_BUFFER_HPP_
#define _ML_VERTEX_BUFFER_HPP_

#include <ML/Graphics/GL.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/List.hpp>
#include <ML/Core/I_Handle.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API VertexBuffer final
		: public I_Newable
		, public I_Handle<uint32_t>
	{
	public:
		VertexBuffer();
		VertexBuffer(const VertexBuffer & copy);
		~VertexBuffer();

	public:
		VertexBuffer & clean();
		VertexBuffer & create(GL::Usage usage);

	public:
		const VertexBuffer & bind() const;
		const VertexBuffer & unbind() const;

		const VertexBuffer & bufferData(const void * data, uint32_t size) const;
		const VertexBuffer & bufferData(const List<float> & data) const;

		const VertexBuffer & bufferSubData(const void * data, uint32_t size, uint32_t offset) const;
		const VertexBuffer & bufferSubData(const List<float> & data, uint32_t offset) const;

	public:
		inline const void *			data()	const { return m_data;}
		inline const GL::Usage &	usage() const { return m_usage; }
		inline const uint32_t &		size()	const { return m_size; }
		inline const int32_t &		count() const { return m_count; }

	private:
		mutable const void *m_data;
		mutable GL::Usage	m_usage;
		mutable uint32_t	m_size;		// total length of contiguous data
		mutable int32_t		m_count;	// number of vertices
	};

	/* * * * * * * * * * * * * * * * * * * * */

	using VBO = typename VertexBuffer;

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_VERTEX_BUFFER_HPP_