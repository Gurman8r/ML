#ifndef _ML_INDEX_BUFFER_HPP_
#define _ML_INDEX_BUFFER_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Graphics/GL.hpp>
#include <ML/Graphics/Vertex.hpp>
#include <ML/Core/MemoryTracker.hpp>
#include <ML/Core/Handle.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API IndexBufferObject final : public Trackable, public Handle<uint32_t>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		IndexBufferObject();
		IndexBufferObject(IndexBufferObject const & copy);
		~IndexBufferObject();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		IndexBufferObject & clean();

		IndexBufferObject & create(GL::Usage usage, GL::Type type);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		IndexBufferObject const & bind() const;

		IndexBufferObject const & unbind() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		IndexBufferObject const & bufferData(uint32_t const * data, uint32_t count) const;

		IndexBufferObject const & bufferData(ArrayList<uint32_t> const & data) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto data()	const -> uint32_t const *	{ return m_data; }
		inline auto count() const -> uint32_t			{ return m_count; }
		inline auto usage() const -> GL::Usage			{ return m_usage; }
		inline auto type()	const -> GL::Type			{ return m_type; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	private:
		mutable uint32_t const *	m_data;
		mutable uint32_t			m_count;
		mutable GL::Usage			m_usage;
		mutable GL::Type			m_type;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	ML_USING IBO = typename IndexBufferObject;

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_INDEX_BUFFER_HPP_