#ifndef _ML_SURFACE_HPP_
#define _ML_SURFACE_HPP_

#include <ML/Graphics/FrameBuffer.hpp>
#include <ML/Graphics/RenderBuffer.hpp>
#include <ML/Graphics/Shader.hpp>
#include <ML/Graphics/Model.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Surface is Framebuffer + Renderbuffer
	struct ML_GRAPHICS_API Surface final
		: public I_Newable
		, public I_Disposable
		, public I_Drawable
		, public I_Readable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Surface();
		Surface(const Surface & copy);
		~Surface();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;
		bool loadFromFile(const String & filename) override;

		bool create(const vec2i & size, uint32_t attachment);
		bool resize(const vec2i & size);

		void bind() const;
		void unbind() const;

		bool setModel(const Model * value);
		bool setShader(const Shader * value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void draw(RenderTarget & target, RenderBatch batch) const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto model()		const	->  const Model *	{ return m_model;   }
		inline auto shader()	const	->  const Shader *	{ return m_shader;  }
		inline auto shader()			->  const Shader *&	{ return m_shader;  }
		inline auto size()		const	->  const vec2i	&	{ return m_size;	}
		inline auto texture()	const	->  const Texture &	{ return m_texture; }
		inline auto texture()			-> 	Texture &		{ return m_texture; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		const Model *	m_model;
		const Shader *	m_shader;
		vec2i			m_size;
		Texture			m_texture;
		uint32_t		m_attachment;
		FBO				m_fbo;
		RBO				m_rbo;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SURFACE_HPP_