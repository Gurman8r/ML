#include <ML/Graphics/Sprite.hpp>
#include <ML/Graphics/RenderTarget.hpp>
#include <ML/Graphics/Geometry.hpp>
#include <ML/Graphics/Material.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Sprite::Sprite()
		: m_color(Color::white)
		, m_texture(nullptr)
		, m_transform()
	{
	}

	Sprite::Sprite(const Sprite & copy)
		: m_color(copy.m_color)
		, m_texture(copy.m_texture)
		, m_transform(copy.m_transform)
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
		m_transform.setOrigin(value);
		return (*this);
	}

	Sprite & Sprite::setPosition(const vec2 & value)
	{
		m_transform.setPosition(value);
		return (*this);
	}

	Sprite & Sprite::setRotation(const float_t value)
	{
		m_transform.setRotation(value);
		return (*this);
	}

	Sprite & Sprite::setScale(const vec2 & value)
	{
		m_transform.setScale(value);
		return (*this);
	}

	Sprite & Sprite::setTexture(const Texture * value)
	{
		m_texture = value;
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Sprite::draw(RenderTarget & target, RenderBatch batch) const
	{
		if (m_texture)
		{
			if (uni_col4 * u = batch.mat->get<uni_col4>(ML_FRAG_MAIN_COL))
			{
				u->data = m_color;
			}

			if (uni_sampler * u = batch.mat->get<uni_sampler>(ML_FRAG_MAIN_TEX))
			{
				u->data = m_texture;
			}

			const vec2 size = scale() * (vec2)m_texture->size();
			const vec2 dest = position() - (size * origin());

			target.draw(
				geo::rect_quad::spriteQuad({ dest, size }).data(),
				geo::rect_quad::contiguous_t::Size,
				batch);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}