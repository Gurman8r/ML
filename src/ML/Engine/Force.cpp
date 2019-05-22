#include <ML/Engine/Force.hpp>
#include <ML/Engine/PhysicsWorld.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	const vec3 Force::gravity(const vec3 & up, const float mass)
	{
		return (up * PhysicsWorld::Gravity[1] * mass);
	}

	const vec3 Force::normal(const vec3 & grav, const vec3 & norm)
	{
		return glm::proj((glm::vec3)grav, (glm::vec3)norm);
	}

	const vec3 Force::sliding(const vec3 & grav, const vec3 & norm)
	{
		return (grav + norm);
	}

	const vec3 Force::frictionStatic(const vec3 & norm, const vec3 & opp, const vec3 & coeff)
	{
		const vec3 max = (norm * coeff);
		
		return ((opp.sqrMagnitude() < max.sqrMagnitude())
			? -(opp)
			: -(opp.normalized() * max.magnitude()));
	}

	const vec3 Force::frictionKinematic(const vec3 & norm, const vec3 & vel, const float coeff)
	{
		const float len = (norm * coeff).magnitude();

		const vec3 n = -(vel.normalized());

		return (n * len);
	}

	const vec3 Force::drag(const vec3 & vel, const vec3 & fluidVel, const float fluidDensity, const float area, const float coeff)
	{
		const vec3 u = (fluidVel - vel);

		return ((u * u) * fluidDensity * area * coeff * 2.0f);
	}

	const vec3 Force::spring(const vec3 & pos, const vec3 & anchor, const float rest, const float coeff)
	{
		const vec3 diff = (pos - anchor);

		const float t = -(coeff) * (diff.magnitude() * rest);

		return (diff * t);
	}

	const vec3 Force::dampingLinear(const vec3 & vel, const float coeff)
	{
		return (vel * -(coeff));
	}

	/* * * * * * * * * * * * * * * * * * * * */
}