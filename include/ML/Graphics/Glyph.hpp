#ifndef _ML_GLYPH_HPP_
#define _ML_GLYPH_HPP_

#include <ML/Graphics/Geometry.hpp>
#include <ML/Graphics/Texture.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API Glyph final : public Trackable
	{
		enum { Size = 6U };

		Texture		texture { GL::Texture2D, GL::RGBA, GL::Red, true, true };
		FloatRect	bounds	{ 0.0f, 0.0f, 0.0f, 0.0f };
		uint32_t	advance { 0 };

		Glyph() = default;

		inline auto bearing()	const -> vec2		{ return bounds.position(); }
		inline auto size()		const -> vec2		{ return bounds.size(); }
		inline auto x()			const -> float_t	{ return bearing()[0]; }
		inline auto y()			const -> float_t	{ return bearing()[1]; }
		inline auto width()		const -> float_t	{ return size()[0]; }
		inline auto height()	const -> float_t	{ return size()[1]; }
		inline auto offset()	const -> vec2		{ return { x(), -y() }; }
		inline auto step()		const -> float_t	{ return (float_t)(advance >> Size); }
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_GLYPH_HPP_