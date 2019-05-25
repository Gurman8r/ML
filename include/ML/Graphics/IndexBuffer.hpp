#ifndef _ML_INDEX_BUFFER_HPP_
#define _ML_INDEX_BUFFER_HPP_

#include <ML/Graphics/GL.hpp>
#include <ML/Core/IObject.hpp>
#include <ML/Graphics/VertexList.hpp>
#include <ML/Core/IHandle.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API IndexBuffer final
		: public IObject
		, public IHandle<uint32_t>
	{
	public:
		IndexBuffer();
		IndexBuffer(const IndexBuffer & copy);
		~IndexBuffer();

	public:
		IndexBuffer & clean();
		IndexBuffer & create(GL::Usage usage, GL::Type type);

	public:
		const IndexBuffer & bind() const;
		const IndexBuffer & unbind() const;
		const IndexBuffer & bufferData(const uint32_t * data, uint32_t count) const;
		const IndexBuffer & bufferData(const List<uint32_t> & data) const;

	public:
		inline const uint32_t *		data()	const { return m_data; }
		inline const uint32_t &		count() const { return m_count; }
		inline const GL::Usage &	usage() const { return m_usage; }
		inline const GL::Type &		type()	const { return m_type; }

	private:
		mutable const uint32_t *m_data;
		mutable uint32_t		m_count;
		mutable GL::Usage		m_usage;
		mutable GL::Type		m_type;
	};

	/* * * * * * * * * * * * * * * * * * * * */

	using IBO = typename IndexBuffer;

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_INDEX_BUFFER_HPP_