#ifndef _ML_RENDERER_HPP_
#define _ML_RENDERER_HPP_

#include <ML/Graphics/Model.hpp>
#include <ML/Graphics/Material.hpp>
#include <ML/Graphics/RenderStates.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API Renderer final
		: public Newable
		, public Drawable
		, public NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Renderer();
		Renderer(const Model * model, const Material * material);
		Renderer(const Model * model, const Material * material, const RenderStates & states);
		~Renderer();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Renderer & setEnabled(bool value);
		Renderer & setMaterial(const Material * value);
		Renderer & setModel(const Model * value);
		Renderer & setShader(const Shader * value);
		Renderer & setStates(const RenderStates & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto enabled()	const	-> const bool &			{ return m_enabled; }
		inline auto material()			-> Material *			{ return std::remove_cv_t<Material *>(m_material); }
		inline auto material()	const	-> const Material *		{ return m_material; }
		inline auto model()				-> Model *				{ return std::remove_cv_t<Model *>(m_model); }
		inline auto model()		const	-> const Model *		{ return m_model; }
		inline auto shader()			-> Shader *				{ return (m_material && m_material->shader()) ? std::remove_cv_t<Shader *>(m_material->shader()) : nullptr; }
		inline auto shader()	const	-> const Shader *		{ return (m_material && m_material->shader()) ? m_material->shader() : nullptr; }
		inline auto states()			-> RenderStates	&		{ return m_states; }
		inline auto states()	const	-> const RenderStates & { return m_states; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void draw(const RenderTarget & target, RenderBatch & batch) const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline operator bool() const { return enabled(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		bool				m_enabled;
		const Model *		m_model;
		const Material *	m_material;
		RenderStates		m_states;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDERER_HPP_