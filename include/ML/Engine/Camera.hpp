#ifndef _ML_FPS_CAMERA_HPP_
#define _ML_FPS_CAMERA_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/Rect.hpp>
#include <ML/Core/Quaternion.hpp>
#include <ML/Core/I_Newable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// WIP - UNTESTED
	struct ML_ENGINE_API Camera final
		: public I_Newable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Camera();
		Camera(const vec3 & position, const vec3 & target);
		Camera(const Camera & copy);
		~Camera();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Camera & lookAt(const vec3 & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Camera & setForward(const vec3 & value);
		Camera & setPosition(const vec3 & value);
		Camera & setTarget(const vec3 & value);
		Camera & setUp(const vec3 & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto right()		const -> vec3			{ return alg::normalize(alg::cross(forward(), up()) * vec3 { 1, 0, 1 }); }
		inline auto forward()	const -> vec3			{ return alg::normalize(m_target - m_position); }
		inline auto matrix()	const -> const mat4 &	{ return m_matrix; };
		inline auto position()	const -> const vec3 &	{ return m_position; }
		inline auto target()	const -> const vec3 &	{ return m_target; }
		inline auto up()		const -> const vec3 &	{ return m_up; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		mat4 m_matrix;
		vec3 m_position;
		vec3 m_target;
		vec3 m_up;

		mutable bool m_changed;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FPS_CAMERA_HPP_