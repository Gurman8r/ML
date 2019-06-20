#include <ML/Graphics/Camera.hpp>
#include <ML/Graphics/Color.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Camera::Camera()
		: m_res		(vec2i::Zero)
		, m_target	(vec3::Forward)
		, m_ortho	(mat4::Identity())
		, m_persp	(mat4::Identity())
		, position	(vec3::Zero)
		, fov		(45.f)
		, pNear		(0.001f)
		, pFar		(1000.f)
		, oNear		(-1.0f)
		, oFar		(+1.0f)
		, color		(Color::Black)
	{
	}

	Camera::Camera(float fov, float pNear, float pFar, float oNear, float oFar)
		: m_res		(vec2i::Zero)
		, m_target	(vec3::Forward)
		, m_ortho	(mat4::Identity())
		, m_persp	(mat4::Identity())
		, position	(vec3::Zero)
		, fov		(fov)
		, pNear		(pNear)
		, pFar		(pFar)
		, oNear		(oNear)
		, oFar		(oFar)
		, color		(Color::Black)
	{
	}

	Camera::Camera(const Camera & copy)
		: m_res		(copy.m_res)
		, m_target	(copy.m_target)
		, m_ortho	(copy.m_ortho)
		, m_persp	(copy.m_persp)
		, position	(copy.position)
		, fov		(copy.fov)
		, pNear		(copy.pNear)
		, pFar		(copy.pFar)
		, oNear		(copy.oNear)
		, oFar		(copy.oFar)
		, color		(copy.color)
	{
	}

	Camera::~Camera()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Camera & Camera::updateRes(const vec2i & value)
	{
		if ((value != vec2i::Zero) && (m_res != value))
		{
			m_res = value;

			// Orthographic
			m_ortho = Transform::Orthographic(
				{ vec2f::Zero, (vec2f)value },
				this->oNear, 
				this->oFar
			);

			// Perspective
			m_persp = Transform::Perspective(
				this->fov,
				(float)value[0] / (float)value[1],
				this->pNear,
				this->pFar
			);
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}