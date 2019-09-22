#ifndef _ML_RENDERER_HPP_
#define _ML_RENDERER_HPP_

#include <ML/Graphics/I_Drawable.hpp>
#include <ML/Graphics/RenderStates.hpp>
#include <ML/Graphics/Material.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Renderer is a drawable that draws another drawable
	struct ML_GRAPHICS_API Renderer final
		: public I_Newable
		, public I_Drawable
		, public I_NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Renderer();
		Renderer(const I_Drawable * drawable, const Material * material);
		Renderer(const I_Drawable * drawable, const Material * material, const RenderStates & states);
		~Renderer();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Renderer & setDrawable(const I_Drawable * value);
		Renderer & setEnabled(bool value);
		Renderer & setMaterial(const Material * value);
		Renderer & setStates(const RenderStates & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto enabled()	const	-> const bool &			{ return m_enabled; }
		inline auto drawable()	const	-> const I_Drawable *	{ return m_drawable; }
		inline auto material()	const	-> const Material *		{ return m_material; }
		inline auto states()			-> RenderStates	&		{ return m_states; }
		inline auto states()	const	-> const RenderStates & { return m_states; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void draw(RenderTarget & target, RenderBatch batch) const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		const I_Drawable *	m_drawable;
		bool				m_enabled;
		const Material *	m_material;
		RenderStates		m_states;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDERER_HPP_