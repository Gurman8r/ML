/* * * * * * * * * * * * * * * * * * * * */

#include "DemoPhysics.hpp"

#include <ML/Core/Debug.hpp>
#include <ML/Engine/Engine.hpp>
#include <ML/Engine/Resources.hpp>
#include <ML/Physics/Physics.hpp>
#include <ML/Physics/Collider.hpp>
#include <ML/Physics/Particle.hpp>
#include <ML/Physics/Rigidbody.hpp>
#include <ML/Physics/SphereCollider.hpp>
#include <ML/Physics/BoxCollider.hpp>

/* * * * * * * * * * * * * * * * * * * * */

namespace DEMO
{
	// Init
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void DemoPhysics::init()
	{
		// While the window is alive and open
		while (ML_Engine.isRunning())
		{
			ML_Physics.updateAll(DemoPhysics::update);
		}
	}


	// Update
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void DemoPhysics::update(const int32_t i, ml::PhysicsState & state)
	{
		const float totalTime = ML_Engine.mainTimer().elapsed().delta();
		const float deltaTime = ML_Engine.loopTimer().elapsed().delta<ml::Milliseconds, ml::Ratio<1, 10000>>();
		const float sinTime = std::sinf(totalTime);
		const float cosTime = std::cosf(totalTime);

		// Get copy state's data
		ml::vec3 pos;
		ml::quat rot;
		ml::mat4 mat;
		ml::mat4 inv;
		if (state.get<state.T_Pos>(i, pos) &&
			state.get<state.T_Rot>(i, rot) &&
			state.get<state.T_Mat>(i, mat) &&
			state.get<state.T_Inv>(i, inv))
		{
			// Modify copy state's data
			switch (i)
			{
			case RB_BORG:
			{	// Borg
				/* * * * * * * * * * * * * * * * * * * * */
				pos = { pos[0], +cosTime, pos[2] };
				rot = ml::quat::angleAxis(totalTime, ml::vec3::One);
			}
			break;

			case RB_CUBE:
			{	// Cube
				/* * * * * * * * * * * * * * * * * * * * */
				pos = { pos[0], -sinTime, pos[2] };
				rot = ml::quat::angleAxis(totalTime, ml::vec3::One);
			}
			break;

			case RB_NAVBALL:
			{	// Navball
				/* * * * * * * * * * * * * * * * * * * * */
				pos = { pos[0], -cosTime, pos[2] };
				rot = ml::quat::angleAxis(totalTime, ml::vec3::Forward);
			}
			break;

			case RB_MOON:
			{	// Moon
				/* * * * * * * * * * * * * * * * * * * * */
				pos = { pos[0], +sinTime, pos[2] };
				rot = ml::quat::angleAxis(totalTime, ml::vec3::Up);
			}
			break;

			case RB_EARTH:
			{	// Earth
				/* * * * * * * * * * * * * * * * * * * * */
				rot = ml::quat::angleAxis(totalTime, ml::vec3::Up);
			}
			break;

			case RB_GROUND:
			{	// Ground
				/* * * * * * * * * * * * * * * * * * * * */
			}
			break;
			}

			// Apply changes to copy state
			if (!state.set<state.T_Pos>(i, pos) ||
				!state.set<state.T_Rot>(i, rot) ||
				!state.set<state.T_Mat>(i, mat) ||
				!state.set<state.T_Inv>(i, inv))
			{
				// something went wrong...
			}
		}
	}


	// Sync
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void DemoPhysics::sync(const ml::PhysicsState & state)
	{
		for (auto & pair : ML_Resources.entities)
		{
			if (ml::Rigidbody * rb = pair.second->get<ml::Rigidbody>())
			{
				ml::vec3 scl = rb->transform()->getScl();
				ml::vec3 pos;
				ml::quat rot;
				ml::mat4 mat;
				ml::mat4 inv;

				if (state.get<state.T_Pos>(rb->index(), pos) &&
					state.get<state.T_Rot>(rb->index(), rot) &&
					state.get<state.T_Mat>(rb->index(), mat) &&
					state.get<state.T_Inv>(rb->index(), inv))
				{
					(*rb->transform())
						.update(ml::mat4::Identity())
						.translate(pos)
						.rotate(rot)
						.scale(scl)
						;
				}
			}
		}
	}

}