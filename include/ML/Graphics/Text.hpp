#ifndef _ML_TEXT_HPP_
#define _ML_TEXT_HPP_

#include <ML/Graphics/I_Drawable.hpp>
#include <ML/Graphics/Font.hpp>
#include <ML/Graphics/VertexList.hpp>
#include <ML/Graphics/RenderTarget.hpp>
#include <ML/Graphics/VertexArray.hpp>
#include <ML/Graphics/VertexBuffer.hpp>
#include <ML/Graphics/Shapes.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API Text final
		: public I_Newable
		, public I_Drawable
	{
	public:
		Text();
		Text(const Text & copy);
		~Text();

	public:
		Text & setPosition(const vec2 & value);
		Text & setScale(const vec2 & value);
		Text & setColor(const vec4 & value);
		Text & setFont(const Font * value);
		Text & setFontSize(uint32_t value);
		Text & setString(const String & value);

	public:
		inline const Font *		getFont()		const { return m_font;		}
		inline const uint32_t	getFontSize()	const { return m_fontSize;	}
		inline const vec2 &		getPosition()	const { return m_position;	}
		inline const vec2 &		getScale()		const { return m_scale;		}
		inline const String &	getString()		const { return m_string;	}
		inline const vec4 &		getColor()		const { return m_color;		}

	public:
		void update() const;
		void draw(RenderTarget & target, RenderBatch batch) const override;

	private:
		const Font *m_font;
		uint32_t	m_fontSize;
		vec2		m_position;
		vec2		m_scale;
		String		m_string;
		vec4		m_color;

		mutable bool m_changed;
		mutable List<const Texture *> m_textures;
		mutable List<Array<float, Shapes::RectQuad::Size>> m_vertices;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}
#endif // !_ML_TEXT_HPP_