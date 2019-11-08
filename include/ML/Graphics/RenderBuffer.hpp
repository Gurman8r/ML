#ifndef _ML_RENDER_BUFFER_HPP_
#define _ML_RENDER_BUFFER_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Graphics/GL.hpp>
#include <ML/Core/Newable.hpp>
#include <ML/Core/Handle.hpp>
#include <ML/Core/Matrix.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API RenderBuffer final
		: public Newable
		, public Handle<uint32_t>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		RenderBuffer();
		RenderBuffer(const RenderBuffer & copy);
		~RenderBuffer();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		RenderBuffer & clean();

		RenderBuffer & create(const vec2i & size);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const RenderBuffer & bind() const;

		const RenderBuffer & unbind() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const RenderBuffer & bufferStorage(GL::Format internalFormat) const;

		const RenderBuffer & setFramebuffer(GL::FrameID attachment) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto size()		const -> const vec2i &		{ return m_size; }
		inline auto width()		const -> const int32_t &	{ return m_size[0]; }
		inline auto height()	const -> const int32_t &	{ return m_size[1]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		mutable vec2i m_size;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	using RBO = typename RenderBuffer;

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDER_BUFFER_HPP_