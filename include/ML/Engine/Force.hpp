#ifndef _ML_FORCE_HPP_
#define _ML_FORCE_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/Transform.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API Force
	{
	public:
		static const vec3 gravity(const vec3 & up, const float mass);
		static const vec3 normal(const vec3 & grav, const vec3 & norm);
		static const vec3 sliding(const vec3 & grav, const vec3 & norm);
		static const vec3 frictionStatic(const vec3 & norm, const vec3 & opp, const vec3 & coeff);
		static const vec3 frictionKinematic(const vec3 & norm, const vec3 & vel, const float coeff);
		static const vec3 drag(const vec3 & vel, const vec3 & fluidVel, const float fluidDensity, const float area, const float coeff);
		static const vec3 spring(const vec3 & pos, const vec3 & anchor, const float rest, const float coeff);
		static const vec3 dampingLinear(const vec3 & vel, const float coeff);
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FORCE_HPP_