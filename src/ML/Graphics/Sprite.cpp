#include <ML/Graphics/Sprite.hpp>
#include <ML/Graphics/RenderTarget.hpp>
#include <ML/Graphics/Geometry.hpp>
#include <ML/Graphics/Material.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Sprite::Sprite()
		: m_color	{ Colors::white.rgba() }
		, m_texture	{ nullptr }
		, m_tf		{}
	{
	}

	Sprite::Sprite(String const & filename)
		: Sprite{}
	{
		loadFromFile(filename);
	}

	Sprite::Sprite(Sprite const & copy)
		: m_color	{ copy.m_color }
		, m_texture	{ copy.m_texture }
		, m_tf		{ copy.m_tf }
	{
	}

	Sprite::~Sprite() {}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Sprite::loadFromFile(String const & filename)
	{
		return true;
	}

	bool Sprite::loadFromMemory(Texture const * value)
	{
		if (value)
		{
			setTexture(value);
			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Sprite & Sprite::setColor(vec4 const & value)
	{
		m_color = value;
		return (*this);
	}
	
	Sprite & Sprite::setOrigin(vec2 const & value)
	{
		m_tf.setOrigin(value);
		return (*this);
	}

	Sprite & Sprite::setPosition(vec2 const & value)
	{
		m_tf.setPosition(value);
		return (*this);
	}

	Sprite & Sprite::setRotation(const float_t value)
	{
		m_tf.setRotation(value);
		return (*this);
	}

	Sprite & Sprite::setScale(vec2 const & value)
	{
		m_tf.setScale(value);
		return (*this);
	}

	Sprite & Sprite::setTexture(Texture const * value)
	{
		m_texture = value;
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Sprite::draw(RenderTarget const & target, RenderBatch & batch) const
	{
		if (m_texture && batch.mat)
		{
			batch.mat->set<uni_color>(ML_IMPL_UNI_MAIN_COLOR, m_color);
			
			batch.mat->set<uni_sampler>(ML_IMPL_UNI_MAIN_TEXTURE, m_texture);

			target.draw(
				geo::spriteQuad({ position() - (size() * origin()), size() }).data(),
				geo::rect_quad::Size,
				batch
			);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}