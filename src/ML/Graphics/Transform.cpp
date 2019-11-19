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

	Transform::Transform(Transform const & copy)
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

	Transform & Transform::setPosition(vec3 const & value)
	{
		m_pos = value;
		return (*this);
	}

	Transform & Transform::setRotation(vec4 const & value)
	{
		m_rot = value;
		return (*this);
	}

	Transform & Transform::setScale(vec3 const & value)
	{
		m_scl = value;
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}