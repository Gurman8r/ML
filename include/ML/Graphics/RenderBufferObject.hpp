#ifndef _ML_RENDER_BUFFER_HPP_
#define _ML_RENDER_BUFFER_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Graphics/GL.hpp>
#include <ML/Core/List.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/I_Handle.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API RenderBufferObject final
		: public I_Newable
		, public I_Handle<uint32_t>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		RenderBufferObject();
		RenderBufferObject(const RenderBufferObject & copy);
		~RenderBufferObject();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		RenderBufferObject & clean();
		RenderBufferObject & create(int32_t width, int32_t height);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const RenderBufferObject & bind() const;
		const RenderBufferObject & unbind() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const RenderBufferObject & bufferStorage(GL::Format internalFormat) const;
		const RenderBufferObject & setFramebuffer(GL::FrameAttachment attachment) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto width()  const -> int32_t { return m_width; }
		inline auto height() const -> int32_t { return m_height; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		mutable int32_t m_width;
		mutable int32_t m_height;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	using RBO = typename RenderBufferObject;

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDER_BUFFER_HPP_