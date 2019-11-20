#ifndef _ML_RENDER_TEXTURE_HPP_
#define _ML_RENDER_TEXTURE_HPP_

#include <ML/Graphics/FrameBufferObject.hpp>
#include <ML/Graphics/RenderBufferObject.hpp>
#include <ML/Graphics/Material.hpp>
#include <ML/Graphics/Model.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API RenderTexture final : public Trackable, public Drawable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		RenderTexture();
		RenderTexture(Model const * model, Material const * material, Shader const * shader);
		RenderTexture(RenderTexture const & copy);
		~RenderTexture() { this->dispose(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose();
		bool create();
		bool update(vec2i const & size);
		void draw(RenderTarget const & target, RenderBatch & batch) const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		RenderTexture const & bind() const;
		RenderTexture const & unbind() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		RenderTexture & setColorID(GL::ColorID value);
		RenderTexture & setFrameID(GL::FrameID value);
		RenderTexture & setModel(Model const * value);
		RenderTexture & setMaterial(Material const * value);
		RenderTexture & setShader(Shader const * value);
		RenderTexture & setSize(vec2i const & value);
		RenderTexture & setStorage(GL::Format value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto colorID()	const	-> GL::ColorID		{ return m_colorID; }
		inline auto frameID()	const	-> GL::FrameID		{ return m_frameID; }
		inline auto fbo()		const	-> FBO const & 		{ return m_fbo; }
		inline auto height()	const	-> int32_t			{ return m_size[1]; }
		inline auto material()	const	-> Material const *	{ return m_material; }
		inline auto model()		const	-> Model const *	{ return m_model; }
		inline auto rbo()		const	-> RBO const &		{ return m_rbo; }
		inline auto shader()	const	-> Shader const *	{ return m_shader; }
		inline auto size()		const	-> const vec2i  &	{ return m_size; }
		inline auto storage()	const	-> GL::Format		{ return m_storage; }
		inline auto texture()	const	-> Texture const &	{ return m_texture; }
		inline auto texture()			-> Texture &		{ return m_texture; }
		inline auto width()		const	-> int32_t			{ return m_size[0]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline operator bool() const { return model() && material() && shader(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		GL::ColorID			m_colorID;	// 
		GL::FrameID			m_frameID;	// 
		FBO					m_fbo;		// 
		Material const *	m_material;	// 
		Shader const *		m_shader;	//
		Model const *		m_model;	// 
		RBO					m_rbo;		// 
		vec2i				m_size;		// 
		GL::Format			m_storage;	// 
		Texture				m_texture;	// 

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDER_TEXTURE_HPP_