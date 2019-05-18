#ifndef _DEMO_PHYSICS_HPP_
#define _DEMO_PHYSICS_HPP_

#include <ML/Physics/Physics.hpp>

namespace DEMO
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum Rigidbody_ID : int32_t
	{
		RB_BORG,
		RB_CUBE,
		RB_NAVBALL,
		RB_MOON,
		RB_EARTH,
		RB_GROUND,

		MAX_DEMO_RIGIDBODY
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct DemoPhysics final
	{
		static void init();
		
		static void update(const int32_t i, ml::PhysicsState & state);
		
		static void sync(const ml::PhysicsState & state);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_DEMO_PHYSICS_HPP_