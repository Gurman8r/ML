#ifndef _ML_RENDER_BUFFER_HPP_
#define _ML_RENDER_BUFFER_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Graphics/GL.hpp>
#include <ML/Core/MemoryTracker.hpp>
#include <ML/Core/Handle.hpp>
#include <ML/Core/Matrix.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API RenderBufferObject final : public Trackable, public Handle<uint32_t>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		RenderBufferObject();
		RenderBufferObject(RenderBufferObject const & copy);
		~RenderBufferObject();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		RenderBufferObject & clean();

		RenderBufferObject & create(vec2i const & size);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		RenderBufferObject const & bind() const;

		RenderBufferObject const & unbind() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		RenderBufferObject const & bufferStorage(GL::Format internalFormat) const;

		RenderBufferObject const & setFramebuffer(GL::FrameID attachment) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto size()		const -> vec2i const &		{ return m_size; }
		inline auto width()		const -> int32_t const &	{ return m_size[0]; }
		inline auto height()	const -> int32_t const &	{ return m_size[1]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		mutable vec2i m_size;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	ML_USING RBO = typename RenderBufferObject;

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDER_BUFFER_HPP_