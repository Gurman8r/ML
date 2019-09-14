#include <ML/Graphics/Transform.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Transform::Transform()
	{
	}

	Transform::Transform(const Transform & copy)
	{
	}

	Transform::~Transform() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Transform & Transform::setPosition(const vec3 & value)
	{
		m_pos = value;
		return (*this);
	}

	Transform & Transform::setRotation(const vec3 & value)
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