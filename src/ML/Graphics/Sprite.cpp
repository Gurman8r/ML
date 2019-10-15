#include <ML/Graphics/Sprite.hpp>
#include <ML/Graphics/RenderTarget.hpp>
#include <ML/Graphics/Geometry.hpp>
#include <ML/Graphics/Material.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Sprite::Sprite()
		: m_color(Colors::white)
		, m_texture(nullptr)
		, m_tf()
	{
	}

	Sprite::Sprite(const String & filename)
		: Sprite()
	{
		loadFromFile(filename);
	}

	Sprite::Sprite(const Sprite & copy)
		: m_color(copy.m_color)
		, m_texture(copy.m_texture)
		, m_tf(copy.m_tf)
	{
	}

	Sprite::~Sprite() {}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Sprite::loadFromFile(const String & filename)
	{
		return true;
	}

	bool Sprite::loadFromMemory(const Texture * value)
	{
		if (value)
		{
			setTexture(value);
			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Sprite & Sprite::setColor(const vec4 & value)
	{
		m_color = value;
		return (*this);
	}
	
	Sprite & Sprite::setOrigin(const vec2 & value)
	{
		m_tf.setOrigin(value);
		return (*this);
	}

	Sprite & Sprite::setPosition(const vec2 & value)
	{
		m_tf.setPosition(value);
		return (*this);
	}

	Sprite & Sprite::setRotation(const float_t value)
	{
		m_tf.setRotation(value);
		return (*this);
	}

	Sprite & Sprite::setScale(const vec2 & value)
	{
		m_tf.setScale(value);
		return (*this);
	}

	Sprite & Sprite::setTexture(const Texture * value)
	{
		m_texture = value;
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Sprite::draw(const RenderTarget & target, RenderBatch batch) const
	{
		if (m_texture && batch.mat)
		{
			if (uni_color * u = batch.mat->get<uni_color>(ML_UNI_MAIN_COL))
			{
				u->data = m_color;
			}

			if (uni_sampler * u = batch.mat->get<uni_sampler>(ML_UNI_MAIN_TEX))
			{
				u->data = m_texture;
			}

			const vec2 scl = scale() * (vec2)m_texture->size();
			const vec2 pos = position() - (scl * origin());

			target.draw(
				geo::rect_quad::spriteQuad({ pos, scl }).data(),
				geo::rect_quad::contiguous_t::Size,
				batch
			);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}