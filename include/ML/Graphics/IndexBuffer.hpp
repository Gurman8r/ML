#ifndef _ML_INDEX_BUFFER_HPP_
#define _ML_INDEX_BUFFER_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Graphics/GL.hpp>
#include <ML/Graphics/Vertex.hpp>
#include <ML/Core/Newable.hpp>
#include <ML/Core/Handle.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API IndexBuffer final
		: public Newable
		, public Handle<uint32_t>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		IndexBuffer();
		IndexBuffer(const IndexBuffer & copy);
		~IndexBuffer();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		IndexBuffer & clean();

		IndexBuffer & create(GL::Usage usage, GL::Type type);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const IndexBuffer & bind() const;

		const IndexBuffer & unbind() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const IndexBuffer & bufferData(const_ptr_t<uint32_t> data, uint32_t count) const;

		const IndexBuffer & bufferData(const List<uint32_t> & data) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto data()	const -> const_ptr_t<uint32_t>	{ return m_data; }
		inline auto count() const -> uint32_t			{ return m_count; }
		inline auto usage() const -> GL::Usage			{ return m_usage; }
		inline auto type()	const -> GL::Type			{ return m_type; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	private:
		mutable const_ptr_t<uint32_t>	m_data;
		mutable uint32_t			m_count;
		mutable GL::Usage			m_usage;
		mutable GL::Type			m_type;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	using IBO = typename IndexBuffer;

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_INDEX_BUFFER_HPP_