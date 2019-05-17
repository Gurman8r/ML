#include <ML/Graphics/Camera.hpp>
#include <ML/Graphics/Color.hpp>

#define ML_CAM_DEFAULT_FOV		45.0f
#define ML_CAM_DEFAULT_PNEAR	0.1f
#define ML_CAM_DEFAULT_PFAR		1000.0f
#define ML_CAM_DEFAULT_ONEAR	-1.0f
#define ML_CAM_DEFAULT_OFAR		+1.0f

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Camera::Camera()
		: m_res		(vec2i::Zero)
		, m_target	(vec3::Forward)
		, m_ortho	(mat4::Identity())
		, m_persp	(mat4::Identity())
		, position	(vec3::Zero)
		, fov		(ML_CAM_DEFAULT_FOV)
		, pNear		(ML_CAM_DEFAULT_PNEAR)
		, pFar		(ML_CAM_DEFAULT_PFAR)
		, oNear		(ML_CAM_DEFAULT_ONEAR)
		, oFar		(ML_CAM_DEFAULT_OFAR)
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