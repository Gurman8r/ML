#ifndef _ML_RENDER_BUFFER_HPP_
#define _ML_RENDER_BUFFER_HPP_

#include <ML/Graphics/GL.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/List.hpp>
#include <ML/Core/I_Handle.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API RenderBuffer final
		: public I_Newable
		, public I_Handle<uint32_t>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		RenderBuffer();
		RenderBuffer(const RenderBuffer & copy);
		~RenderBuffer();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		RenderBuffer & clean();
		RenderBuffer & create(int32_t width, int32_t height);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const RenderBuffer & bind() const;
		const RenderBuffer & unbind() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const RenderBuffer & bufferStorage(GL::Format internalFormat) const;
		const RenderBuffer & setFramebuffer(GL::FrameAttachment attachment) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto width()  const -> int32_t { return m_width; }
		inline auto height() const -> int32_t { return m_height; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		mutable int32_t m_width;
		mutable int32_t m_height;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	using RBO = typename RenderBuffer;

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDER_BUFFER_HPP_