#ifndef _ML_RENDERER_HPP_
#define _ML_RENDERER_HPP_

#include <ML/Graphics/I_Drawable.hpp>
#include <ML/Graphics/RenderStates.hpp>
#include <ML/Graphics/Material.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

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
		Renderer & setMaterial(const Material * value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto drawable()	const	-> const I_Drawable *	{ return m_drawable; }
		inline auto material()	const	-> const Material *		{ return m_material; }
		inline auto states()			-> RenderStates	&		{ return m_states; }
		inline auto states()	const	-> const RenderStates & { return m_states; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void draw(RenderTarget & target, RenderBatch batch) const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		const I_Drawable *	m_drawable;
		const Material *	m_material;
		RenderStates		m_states;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDERER_HPP_