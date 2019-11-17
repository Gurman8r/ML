#ifndef _ML_SURFACE_HPP_
#define _ML_SURFACE_HPP_

#include <ML/Graphics/FrameBuffer.hpp>
#include <ML/Graphics/RenderBuffer.hpp>
#include <ML/Graphics/Material.hpp>
#include <ML/Graphics/Model.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Surface is used to render a texture
	struct ML_GRAPHICS_API Surface final
		: public Trackable
		, public Disposable
		, public Drawable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Surface();
		Surface(const_ptr_t<Model> model, const_ptr_t<Material> material, const_ptr_t<Shader> shader);
		Surface(const Surface & copy);
		~Surface();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;
		bool create();
		bool update(const vec2i & size);
		void draw(const RenderTarget & target, RenderBatch & batch) const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		const Surface & bind() const;
		const Surface & unbind() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Surface & setColorID(GL::ColorID value);
		Surface & setFrameID(GL::FrameID value);
		Surface & setModel(const_ptr_t<Model> value);
		Surface & setMaterial(const_ptr_t<Material> value);
		Surface & setShader(const_ptr_t<Shader> value);
		Surface & setSize(const vec2i & value);
		Surface & setStorage(GL::Format value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto colorID()	const -> GL::ColorID			{ return m_colorID; }
		inline auto frameID()	const -> GL::FrameID			{ return m_frameID; }
		inline auto fbo()		const -> const FBO & 			{ return m_fbo; }
		inline auto height()	const -> int32_t				{ return m_size[1]; }
		inline auto material()	const -> const_ptr_t<Material>	{ return m_material; }
		inline auto model()		const -> const_ptr_t<Model>		{ return m_model; }
		inline auto rbo()		const -> const RBO &			{ return m_rbo; }
		inline auto shader()	const -> const_ptr_t<Shader>	{ return m_shader; }
		inline auto size()		const -> const vec2i  &			{ return m_size; }
		inline auto storage()	const -> GL::Format				{ return m_storage; }
		inline auto texture()	const -> const Texture &		{ return m_texture; }
		inline auto width()		const -> int32_t				{ return m_size[0]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline operator bool() const { return model() && material() && shader(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		GL::ColorID				m_colorID;	// 
		GL::FrameID				m_frameID;	// 
		FBO						m_fbo;		// 
		const_ptr_t<Material>	m_material;	// 
		const_ptr_t<Shader>		m_shader;	//
		const_ptr_t<Model>		m_model;	// 
		RBO						m_rbo;		// 
		vec2i					m_size;		// 
		GL::Format				m_storage;	// 
		Texture					m_texture;	// 

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SURFACE_HPP_