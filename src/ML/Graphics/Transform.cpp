#include <ML/Graphics/Transform.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Transform::Transform()
		: m_enabled { true }
		, m_pos { NULL }
		, m_scl { NULL }
		, m_rot { NULL }
	{
	}

	Transform::Transform(const Transform & copy)
		: Transform {}
	{
	}

	Transform::~Transform() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Transform & Transform::setEnabled(bool value)
	{
		m_enabled = value;
		return (*this);
	}

	Transform & Transform::setPosition(const vec3 & value)
	{
		m_pos = value;
		return (*this);
	}

	Transform & Transform::setRotation(const vec4 & value)
	{
		m_rot = value;
		return (*this);
	}

	Transform & Transform::setScale(const vec3 & value)
	{
		m_scl = value;
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}