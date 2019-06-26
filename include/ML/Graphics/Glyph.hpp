#ifndef _ML_GLYPH_HPP_
#define _ML_GLYPH_HPP_

#include <ML/Graphics/Geometry.hpp>
#include <ML/Graphics/Texture.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API Glyph final
		: public I_Newable
	{
		enum { VertexCount = 6U };

		Texture		texture;
		FloatRect	bounds;
		uint32_t	advance;

		Glyph()
			: texture(GL::Texture2D, GL::RGBA, GL::Red, true, true)
			, bounds ({ 0.0f, 0.0f, 0.0f, 0.0f })
			, advance(0)
		{
		}

		inline const vec2		bearing()	const { return bounds.position(); }
		inline const vec2		size()		const { return bounds.size(); }
		inline const float_t	x()			const { return bearing()[0]; }
		inline const float_t	y()			const { return bearing()[1]; }
		inline const float_t	width()		const { return size()[0]; }
		inline const float_t	height()	const { return size()[1]; }
		inline const vec2		offset()	const { return { x(), -y() }; }
		inline const float_t	step()		const { return (float_t)(advance >> VertexCount); }
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_GLYPH_HPP_