#ifndef _ML_TEXT_HPP_
#define _ML_TEXT_HPP_

#include <ML/Graphics/I_Drawable.hpp>
#include <ML/Graphics/Font.hpp>
#include <ML/Graphics/Vertices.hpp>
#include <ML/Graphics/RenderTarget.hpp>
#include <ML/Graphics/VertexArrayObject.hpp>
#include <ML/Graphics/VertexBufferObject.hpp>
#include <ML/Graphics/Geometry.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API Text final
		: public I_Newable
		, public I_Drawable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Text();
		Text(const Text & copy);
		~Text();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Text & setPosition(const vec2 & value);
		Text & setScale(const vec2 & value);
		Text & setColor(const vec4 & value);
		Text & setFont(const Font * value);
		Text & setFontSize(uint32_t value);
		Text & setString(const String & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto getFont()		const -> const Font *		{ return m_font; }
		inline auto getFontSize()	const -> const uint32_t		{ return m_fontSize; }
		inline auto getPosition()	const -> const vec2 &		{ return m_position; }
		inline auto getScale()		const -> const vec2 &		{ return m_scale; }
		inline auto getString()		const -> const String &		{ return m_string; }
		inline auto getColor()		const -> const vec4 &		{ return m_color; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void update() const;
		void draw(RenderTarget & target, RenderBatch batch) const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		const Font *m_font;
		uint32_t	m_fontSize;
		vec2		m_position;
		vec2		m_scale;
		String		m_string;
		vec4		m_color;

		mutable bool m_changed;
		mutable List<const Texture *> m_textures;
		mutable List<geo::rect_quad::contiguous_t> m_vertices;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}
#endif // !_ML_TEXT_HPP_