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
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Renderer();
		Renderer(const I_Drawable * drawable, const Material * material);
		Renderer(const Renderer & copy);
		~Renderer();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto drawable()	const	-> const I_Drawable *	{ return m_drawable; }
		inline auto material()	const	-> const Material *		{ return m_material; }
		inline auto states()	const	-> const RenderStates & { return m_states; }
		inline auto drawable()			-> const I_Drawable *&	{ return m_drawable; }
		inline auto material()			-> const Material *&	{ return m_material; }
		inline auto states()			-> RenderStates	&		{ return m_states; }

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