#include <ML/Engine/Camera.hpp>
#include <ML/Graphics/GLM.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Camera::Camera()
		: m_matrix	({ uninit })
		, m_position({ uninit })
		, m_target	({ uninit })
		, m_up		({ 0.0f, 1.0f, 0.0f })
	{
	}

	Camera::Camera(const vec3 & position, const vec3 & target)
		: m_matrix	({ uninit })
		, m_position(position)
		, m_target	(target)
		, m_up		({ 0.0f, 1.0f, 0.0f })
	{
	}

	Camera::Camera(const Camera & copy)
		: m_matrix	(copy.m_matrix)
		, m_position(copy.m_position)
		, m_target	(copy.m_target)
		, m_up		(copy.m_up)
	{
	}
	
	Camera::~Camera()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Camera & Camera::lookAt(const vec3 & value)
	{
		if (m_changed)
		{
			m_changed = false;
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Camera & Camera::setForward(const vec3 & value)
	{
		return setTarget(m_position + alg::normalize(value));
	}

	Camera & Camera::setPosition(const vec3 & value)
	{
		if (m_position != value)
		{
			m_changed = true;
			m_position = value;
		}
		return (*this);
	}

	Camera & Camera::setTarget(const vec3 & value)
	{
		if (m_target != value)
		{
			m_changed = true;
			m_target = value;
		}
		return (*this);
	}

	Camera & Camera::setUp(const vec3 & value)
	{
		if (m_up != value)
		{
			m_changed = true;
			m_up = value;
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}