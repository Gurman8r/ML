#ifndef _ML_CAMERA_HPP_
#define _ML_CAMERA_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Core/Transform.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API Camera final
		: public I_Newable
	{
	public:
		Camera();
		Camera(float fov, float pNear, float pFar, float oNear, float oFar);
		Camera(const Camera & copy);
		~Camera();

	public:
		Camera & updateRes(const vec2i & resolution);

	public:
		inline const mat4 &	getOrthoMatrix() const	{ return m_ortho; }
		inline const mat4 &	getPerspMatrix() const	{ return m_persp; }

		inline const vec3 & forward(const vec3 & value)
		{
			return (m_target = this->position + value.normalized());
		}

		inline vec3 forward() const 
		{
			return (m_target - this->position).normalized();
		}
		
		inline vec3 right() const 
		{ 
			return (forward().cross(vec3::Up) * vec3(1, 0, 1)).normalized();
		}

	public:
		vec3		position;
		float		fov;
		float		pNear, pFar;
		float		oNear, oFar;
		vec4		color;

	private:
		vec2i		m_res;
		vec3		m_target;
		mat4		m_ortho;
		mat4		m_persp;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CAMERA_HPP_