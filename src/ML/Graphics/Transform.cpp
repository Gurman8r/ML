#include <ML/Graphics/Transform.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Transform::Transform()
		: m_enabled { true }
		, m_pos { 0, 0, 0 }
		, m_scl { 1, 1, 1 }
		, m_rot { 0, 1, 0, 1 }
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