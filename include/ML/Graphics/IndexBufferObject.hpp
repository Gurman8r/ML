#ifndef _ML_INDEX_BUFFER_HPP_
#define _ML_INDEX_BUFFER_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Graphics/GL.hpp>
#include <ML/Graphics/Vertex.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/I_Handle.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API IndexBufferObject final
		: public I_Newable
		, public I_Handle<uint32_t>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		IndexBufferObject();
		IndexBufferObject(const IndexBufferObject & copy);
		~IndexBufferObject();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		IndexBufferObject & clean();
		IndexBufferObject & create(GL::Usage usage, GL::Type type);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const IndexBufferObject & bind() const;
		const IndexBufferObject & unbind() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const IndexBufferObject & bufferData(const uint32_t * data, uint32_t count) const;
		const IndexBufferObject & bufferData(const List<uint32_t> & data) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto data()	const -> const uint32_t *	{ return m_data; }
		inline auto count() const -> uint32_t			{ return m_count; }
		inline auto usage() const -> GL::Usage			{ return m_usage; }
		inline auto type()	const -> GL::Type			{ return m_type; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	private:
		mutable const uint32_t *	m_data;
		mutable uint32_t			m_count;
		mutable GL::Usage			m_usage;
		mutable GL::Type			m_type;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	using IBO = typename IndexBufferObject;

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_INDEX_BUFFER_HPP_