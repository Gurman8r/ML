#ifndef _ML_RENDERER_HPP_
#define _ML_RENDERER_HPP_

#include <ML/Graphics/Model.hpp>
#include <ML/Graphics/Material.hpp>
#include <ML/Graphics/RenderStates.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API Renderer final : public Trackable, public Drawable, public NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Renderer();
		Renderer(Model const * model, Material const * material, Shader const * shader);
		Renderer(Model const * model, Material const * material, Shader const * shader, RenderStates const & states);
		~Renderer();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Renderer & setEnabled(bool value);
		Renderer & setMaterial(Material const * value);
		Renderer & setModel(Model const * value);
		Renderer & setShader(Shader const * value);
		Renderer & setStates(RenderStates const & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto enabled()	const	-> bool					{ return m_enabled; }
		inline auto material()			-> Material *			{ return std::remove_cv_t<Material *>(m_material); }
		inline auto material()	const	-> Material const *		{ return m_material; }
		inline auto model()				-> Model *				{ return std::remove_cv_t<Model *>(m_model); }
		inline auto model()		const	-> Model const *		{ return m_model; }
		inline auto shader()			-> Shader *				{ return std::remove_cv_t<Shader *>(m_shader); }
		inline auto shader()	const	-> Shader const *		{ return m_shader; }
		inline auto states()			-> RenderStates &		{ return m_states; }
		inline auto states()	const	-> RenderStates const & { return m_states; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void draw(RenderTarget const & target, RenderBatch & batch) const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		bool				m_enabled;
		Model const *		m_model;
		Material const *	m_material;
		Shader const *		m_shader;
		RenderStates		m_states;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDERER_HPP_