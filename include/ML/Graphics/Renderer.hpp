#ifndef _ML_RENDERER_HPP_
#define _ML_RENDERER_HPP_

#include <ML/Graphics/Model.hpp>
#include <ML/Graphics/Material.hpp>
#include <ML/Graphics/RenderStates.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API Renderer final : public Newable, public Drawable, public NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Renderer();
		Renderer(const_ptr_t<Model> model, const_ptr_t<Material> material, const_ptr_t<Shader> shader);
		Renderer(const_ptr_t<Model> model, const_ptr_t<Material> material, const_ptr_t<Shader> shader, const RenderStates & states);
		~Renderer();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Renderer & setEnabled(bool value);
		Renderer & setMaterial(const_ptr_t<Material> value);
		Renderer & setModel(const_ptr_t<Model> value);
		Renderer & setShader(const_ptr_t<Shader> value);
		Renderer & setStates(const RenderStates & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto enabled()	const	-> const bool &			{ return m_enabled; }
		inline auto material()			-> ptr_t<Material>		{ return std::remove_cv_t<ptr_t<Material>>(m_material); }
		inline auto material()	const	-> const_ptr_t<Material>{ return m_material; }
		inline auto model()				-> ptr_t<Model>			{ return std::remove_cv_t<ptr_t<Model>>(m_model); }
		inline auto model()		const	-> const_ptr_t<Model>	{ return m_model; }
		inline auto shader()			-> ptr_t<Shader>		{ return std::remove_cv_t<ptr_t<Shader>>(m_shader); }
		inline auto shader()	const	-> const_ptr_t<Shader>	{ return m_shader; }
		inline auto states()			-> RenderStates &		{ return m_states; }
		inline auto states()	const	-> const RenderStates & { return m_states; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void draw(const RenderTarget & target, RenderBatch & batch) const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		bool					m_enabled;
		const_ptr_t<Model>		m_model;
		const_ptr_t<Material>	m_material;
		const_ptr_t<Shader>		m_shader;
		RenderStates			m_states;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDERER_HPP_