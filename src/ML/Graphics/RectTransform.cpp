#include <ML/Graphics/RectTransform.hpp>
#include <ML/Core/Algorithm.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	RectTransform::RectTransform()
		: m_changed	(true)
		, m_matrix	()
		, m_position(0.0f)
		, m_scale	(1.0f)
		, m_rotation(0.0f)
		, m_origin	(0.5f)
	{
	}

	RectTransform::RectTransform(const vec2 & position, const vec2 & scale, const float rotation, const vec2 & origin)
		: m_changed	(true)
		, m_matrix	()
		, m_position(position)
		, m_scale	(scale)
		, m_rotation(rotation)
		, m_origin	(origin)
	{
	}

	RectTransform::RectTransform(const RectTransform & copy)
		: m_changed	(copy.m_changed)
		, m_matrix	(copy.m_matrix)
		, m_position(copy.m_position)
		, m_scale	(copy.m_scale)
		, m_rotation(copy.m_rotation)
		, m_origin	(copy.m_origin)
	{
	}

	RectTransform::~RectTransform()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void RectTransform::update() const
	{
		if (m_changed)
		{	m_changed = false;

			float angle		= maths::to_radians(m_rotation);
			float cosine	= std::cosf(angle);
			float sine		= std::sinf(angle);
			float sxc		= m_scale[0] * cosine;
			float syc		= m_scale[1] * cosine;
			float sxs		= m_scale[0] * sine;
			float sys		= m_scale[1] * sine;
			float tx		= -m_origin[0] * sxc - m_origin[1] * sys + m_position[0];
			float ty		=  m_origin[0] * sxs - m_origin[1] * syc + m_position[1];

			float m00 =  sxc; float m01 = sys; float m02 = tx;
			float m03 = -sxs; float m04 = syc; float m05 = ty;
			float m10 =  0.f; float m11 = 0.f; float m12 = 0.f;

			m_matrix = {
				m00, m01, 0.f, m02,
				m03, m04, 0.f, m05,
				0.f, 0.f, 1.f, 0.f,
				m10, m11, 0.f, m12,
			};
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */

	RectTransform & RectTransform::setOrigin(const vec2 & value)
	{
		if (m_origin != value)
		{
			m_changed = true;
			m_origin = value;
		}
		return (*this);
	}

	RectTransform & RectTransform::setPosition(const vec2 & value)
	{
		if (m_position != value)
		{
			m_changed = true;
			m_position = value;
		}
		return (*this);
	}

	RectTransform & RectTransform::setRotation(const float value)
	{
		if (m_rotation != value)
		{
			m_changed = true;
			m_rotation = value;
		}
		return (*this);
	}

	RectTransform & RectTransform::setScale(const vec2 & value)
	{
		if (m_scale != value)
		{
			m_changed = true;
			m_scale = value;
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}