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
		explicit Sprite(String const & filename);
		Sprite(Sprite const & copy);
		~Sprite();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool loadFromFile(String const & filename);
		bool loadFromMemory(Texture const * value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Sprite & setColor	(vec4 const &	value);
		Sprite & setOrigin	(vec2 const &	value);
		Sprite & setPosition(vec2 const &	value);
		Sprite & setRotation(const float_t	value);
		Sprite & setScale	(vec2 const &	value);
		Sprite & setTexture	(Texture const *value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void draw(RenderTarget const & target, RenderBatch & batch) const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto color()		const -> vec4 const &			{ return m_color; }
		inline auto origin()	const -> vec2 const &			{ return m_tf.origin(); }
		inline auto position()	const -> vec2 const &			{ return m_tf.position(); }
		inline auto rotation()	const -> float_t				{ return m_tf.rotation(); }
		inline auto scale()		const -> vec2 const &			{ return m_tf.scale(); }
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