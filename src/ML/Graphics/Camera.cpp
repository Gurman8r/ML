#include <ML/Graphics/Camera.hpp>
#include <ML/Graphics/GLM.hpp>

namespace ml
{
	static inline mat4 LOOK_AT(const vec3 & eye, const vec3 & center, const vec3 & up)
	{
		const vec3 f = alg::normalize(center - eye);
		const vec3 s = alg::normalize(alg::cross(f, up));
		const vec3 u = alg::cross(s, f);
		mat4 m { 1 };
		m[0] = s[0];
		m[1] = u[0];
		m[2] = -f[0];
		m[4] = s[1];
		m[5] = u[1];
		m[6] = -f[1];
		m[8] = s[2];
		m[9] = u[2];
		m[10] = -f[2];
		m[12] = -alg::dot(s, eye);
		m[13] = -alg::dot(u, eye);
		m[14] = alg::dot(f, eye);
		return m;
	}

	static inline mat4 PERSP(float_t fov, float_t aspect, float_t zNear, float_t zFar)
	{
		mat4 m { NULL };
		m[0 * 4 + 0] = 1.0f / (aspect * std::tan(fov / 2.0f));
		m[1 * 4 + 1] = 1.0f / std::tan(fov / 2.0f);
		m[2 * 4 + 3] = -1.0f;
		m[2 * 4 + 2] = -(zFar + zNear) / (zFar - zNear);
		m[3 * 4 + 2] = -(2.0f * zFar * zNear) / (zFar - zNear);
		return m;
	}
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Camera::Camera()
	{
	}

	Camera::Camera(const Camera & copy)
	{
	}
	
	Camera::~Camera() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}