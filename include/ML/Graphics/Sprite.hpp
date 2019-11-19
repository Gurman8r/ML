#ifndef _ML_SPRITE_HPP_
#define _ML_SPRITE_HPP_

#include <ML/Graphics/Drawable.hpp>
#include <ML/Graphics/Texture.hpp>
#include <ML/Graphics/RectTransform.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API Sprite final
		: public Trackable
		, public Drawable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Sprite();
		explicit Sprite(const String & filename);
		Sprite(const Sprite & copy);
		~Sprite();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool loadFromFile(const String & filename);
		bool loadFromMemory(Texture const * value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Sprite & setColor	(const vec4 &	value);
		Sprite & setOrigin	(const vec2 &	value);
		Sprite & setPosition(const vec2 &	value);
		Sprite & setRotation(const float_t	value);
		Sprite & setScale	(const vec2 &	value);
		Sprite & setTexture	(Texture const *value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void draw(const RenderTarget & target, RenderBatch & batch) const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto color()		const -> const vec4 &			{ return m_color; }
		inline auto origin()	const -> const vec2 &			{ return m_tf.origin(); }
		inline auto position()	const -> const vec2 &			{ return m_tf.position(); }
		inline auto rotation()	const -> float_t				{ return m_tf.rotation(); }
		inline auto scale()		const -> const vec2 &			{ return m_tf.scale(); }
		inline auto size()		const -> vec2					{ return scale() * m_texture->size(); }
		inline auto texture()	const -> Texture const *	{ return m_texture; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		vec4 m_color;
		Texture const * m_texture;
		RectTransform m_tf;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SPRITE_HPP_