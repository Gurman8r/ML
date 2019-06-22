#ifndef _ML_RENDERER_HPP_
#define _ML_RENDERER_HPP_

#include <ML/Graphics/I_Drawable.hpp>
#include <ML/Graphics/RenderStates.hpp>
#include <ML/Graphics/Material.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API Renderer final
		: public I_Newable
		, public I_Drawable
	{
	public:
		Renderer();
		Renderer(const I_Drawable * drawable, const Material * material);
		Renderer(const Renderer & copy);
		virtual ~Renderer();

	public:
		inline const I_Drawable		* drawable() const	{ return m_drawable; }
		inline const Material		* material() const	{ return m_material; }
		inline const RenderStates	& states()	 const	{ return m_states; }

	public:
		inline const I_Drawable *	& drawable()		{ return m_drawable; }
		inline const Material *		& material()		{ return m_material; }
		inline RenderStates			& states()			{ return m_states; }

	public:
		void draw(RenderTarget & target, RenderBatch batch) const override;

	private:
		const I_Drawable *	m_drawable;
		const Material *	m_material;
		RenderStates		m_states;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDERER_HPP_