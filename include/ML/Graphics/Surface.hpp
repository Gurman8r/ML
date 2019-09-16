#ifndef _ML_SURFACE_HPP_
#define _ML_SURFACE_HPP_

#include <ML/Graphics/FrameBufferObject.hpp>
#include <ML/Graphics/RenderBufferObject.hpp>
#include <ML/Graphics/Material.hpp>
#include <ML/Graphics/Model.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Surface is used to render a texture
	struct ML_GRAPHICS_API Surface final
		: public I_Newable
		, public I_Disposable
		, public I_Drawable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Surface();
		Surface(const Model * model, const Material * material);
		Surface(const Surface & copy);
		~Surface();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;
		bool create();
		bool update(const vec2i & size);
		void draw(RenderTarget & target, RenderBatch batch) const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		const Surface & bind() const;
		const Surface & unbind() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Surface & setColorID(GL::ColorID value);
		Surface & setFrameID(GL::FrameID value);
		Surface & setModel(const Model * value);
		Surface & setMaterial(const Material * value);
		Surface & setSize(const vec2i & value);
		Surface & setStorage(GL::Format value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto colorID()	const -> GL::ColorID		{ return m_colorID; }
		inline auto frameID()	const -> GL::FrameID		{ return m_frameID; }
		inline auto fbo()		const -> const FBO &		{ return m_fbo; }
		inline auto height()	const -> int32_t			{ return m_size[1]; }
		inline auto material()	const -> const Material *	{ return m_material; }
		inline auto model()		const -> const Model *		{ return m_model; }
		inline auto rbo()		const -> const RBO &		{ return m_rbo; }
		inline auto size()		const -> const vec2i &		{ return m_size; }
		inline auto storage()	const -> GL::Format			{ return m_storage; }
		inline auto texture()	const -> const Texture &	{ return m_texture; }
		inline auto width()		const -> int32_t			{ return m_size[0]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline operator bool() const { return model() && material(); }

		inline void * get_handle() const { return texture().get_handle(); }

		template <
			class ... Args
		> inline bool setUniform(Args && ... args) const
		{
			return material() && material()->setUniform(std::forward<Args>(args)...);
		}

		template <
			class Fun, class ... Args
		> inline Surface & render_to(Fun && fun, Args && ... args)
		{
			if (*this)
			{
				this->bind();
				fun(std::forward<Args>(args)...);
				this->unbind();
			}
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		GL::ColorID		m_colorID;	// 
		GL::FrameID		m_frameID;	// 
		FBO				m_fbo;		// 
		const Material *m_material;	// 
		const Model *	m_model;	// 
		RBO				m_rbo;		// 
		vec2i			m_size;		// 
		GL::Format		m_storage;	// 
		Texture			m_texture;	// 

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SURFACE_HPP_