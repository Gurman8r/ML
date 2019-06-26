#ifndef _ML_SPRITE_HPP_
#define _ML_SPRITE_HPP_

#include <ML/Graphics/I_Drawable.hpp>
#include <ML/Graphics/Texture.hpp>
#include <ML/Graphics/RectTransform.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API Sprite final
		: public I_Newable
		, public I_Drawable
		, public I_Readable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Sprite();
		Sprite(const Sprite & copy);
		~Sprite();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool loadFromFile(const String & filename) override;
		bool loadFromMemory(const Texture * value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Sprite & setColor	(const vec4 &	value);
		Sprite & setOrigin	(const vec2 &	value);
		Sprite & setPosition(const vec2 &	value);
		Sprite & setRotation(const float	value);
		Sprite & setScale	(const vec2 &	value);
		Sprite & setTexture	(const Texture *value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void draw(RenderTarget & target, RenderBatch batch) const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto color()		const -> const vec4 &	{ return m_color; }
		inline auto origin()	const -> const vec2 &	{ return m_transform.origin(); }
		inline auto position()	const -> const vec2 &	{ return m_transform.position(); }
		inline auto rotation()	const -> float			{ return m_transform.rotation(); }
		inline auto scale()		const -> const vec2 &	{ return m_transform.scale(); }
		inline auto texture()	const -> const Texture *{ return m_texture; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		vec4			m_color;
		const Texture * m_texture;
		RectTransform	m_transform;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SPRITE_HPP_