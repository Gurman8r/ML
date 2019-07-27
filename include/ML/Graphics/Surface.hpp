#ifndef _ML_SURFACE_HPP_
#define _ML_SURFACE_HPP_

#include <ML/Graphics/FrameBuffer.hpp>
#include <ML/Graphics/RenderBuffer.hpp>
#include <ML/Graphics/Shader.hpp>
#include <ML/Graphics/Model.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Surface is used to render objects to a 2D Texture
	struct ML_GRAPHICS_API Surface final
		: public I_Newable
		, public I_Disposable
		, public I_Drawable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Surface();
		Surface(const Model * model, const Shader * shader);
		Surface(const Surface & copy);
		~Surface();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;

		bool create(const vec2i & size, uint32_t attachment);
		bool update(const vec2i & size);
		
		const Surface & bind() const;
		const Surface & unbind() const;

		void draw(RenderTarget & target, RenderBatch batch) const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline operator bool() const
		{
			return model() && shader();
		}

		inline void * get_handle() const
		{
			return texture().get_handle();
		}

		template <
			class ... Args
		> inline bool setUniform(Args && ... args) const
		{
			return shader() && shader()->setUniform(std::forward<Args>(args)...);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto model()				-> const Model *&	{ return m_model; }
		inline auto model()		const	-> const Model *	{ return m_model; }
		inline auto shader()			-> const Shader *&	{ return m_shader; }
		inline auto shader()	const	-> const Shader *	{ return m_shader; }
		inline auto fbo()		const	-> const FBO &		{ return m_fbo; }
		inline auto rbo()		const	-> const RBO &		{ return m_rbo; }
		inline auto size()		const	-> const vec2i	&	{ return m_size; }
		inline auto texture()	const	-> const Texture &	{ return m_texture; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		const Model *	m_model;
		const Shader *	m_shader;
		
		Texture		m_texture;
		vec2i		m_size;
		uint32_t	m_attachment;
		FBO			m_fbo;
		RBO			m_rbo;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SURFACE_HPP_