#ifndef _ML_SPRITE_HPP_
#define _ML_SPRITE_HPP_

#include <ML/Graphics/I_Drawable.hpp>
#include <ML/Graphics/Texture.hpp>
#include <ML/Graphics/RectTransform.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API Sprite final
		: public I_Newable
		, public I_Disposable
		, public I_Drawable
		, public I_Readable
	{
	public:
		Sprite();
		Sprite(const Sprite & copy);
		~Sprite();

	public:
		bool dispose() override;
		bool loadFromFile(const String & filename) override;
		bool loadFromMemory(const Texture * value);

	public:
		Sprite & setColor	(const vec4 &	value);
		Sprite & setOrigin	(const vec2 &	value);
		Sprite & setPosition(const vec2 &	value);
		Sprite & setRotation(const float	value);
		Sprite & setScale	(const vec2 &	value);
		Sprite & setTexture	(const Texture *value);

	public:
		void draw(RenderTarget & target, RenderBatch batch) const override;

	public:
		inline const vec4 &		color()		const { return m_color;					}
		inline const vec2 &		origin()	const { return m_transform.origin();	}
		inline const vec2 &		position()	const { return m_transform.position();	}
		inline const float		rotation()	const { return m_transform.rotation();	}
		inline const vec2 &		scale()		const { return m_transform.scale();		}
		inline const Texture *	texture()	const { return m_texture;				}

	private:
		vec4			m_color;
		const Texture * m_texture;
		RectTransform	m_transform;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SPRITE_HPP_