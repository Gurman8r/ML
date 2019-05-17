/* * * * * * * * * * * * * * * * * * * * */

#include "DemoPhysics.hpp"

#include <ML/Core/Debug.hpp>
#include <ML/Core/Time.hpp>
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
	void demo_physics::init()
	{
		// While the window is alive and open
		while (ML_Engine.isRunning())
		{
			ML_Physics.updateAll(demo_physics::update);
		}
	}


	// Update
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void demo_physics::update(const int32_t i, ml::PhysicsState & state)
	{
		static ml::SphereCollider * c_earth;
		static ml::BoxCollider * c_ground;

		static float deltaT, totalT;
		totalT = ML_Time.elapsed().delta();
		deltaT = ML_Engine.elapsed().delta<ml::Milliseconds, ml::Ratio<1, 10000>>();

		// Get the RB
		ml::Rigidbody	* rb = ML_Physics.getLinkedRigidbody(i);
		ml::Collider	* c = rb->collider();
		ml::Particle	* p = rb->particle();
		ml::Transform	* t = rb->transform();

		// Get copy state's data
		ml::vec3 pos;
		ml::quat rot;
		ml::mat4 mat;
		ml::mat4 inv;
		if (state.get<state.T_Pos>(i, pos) &&
			state.get<state.T_Rot>(i, rot) &&
			state.get<state.T_Mat>(i, mat) &&
			state.get<state.T_Inv>(i, inv) &&
			rb->enabled)
		{
			// Modify copy state's data
			switch (i)
			{
			// Borg
			/* * * * * * * * * * * * * * * * * * * * */
			case RB_BORG:
			{
				(*p).applyForce(ml::vec3::Zero);
				pos = { pos[0], +ML_Time.cos(), pos[2] };
				rot = ml::quat::angleAxis(totalT, ml::vec3::One);
				//(*p).applyForceLocation(ml::vec3::Left, p->centerMass_world + ml::vec3{ 0.2f, 0.1f, 0.0f })
				//	.integrateEulerExplicit(deltaT)
				//	.convertTorque()
				//	.updateCenterMass()
				//	.updateInertiaTensor()
				//	.resetTorque();
				//
				//rot = p->rotation;
				//pos = p->pos;
			}
			break;

			// Cube
			/* * * * * * * * * * * * * * * * * * * * */
			case RB_CUBE: 
			{
				(*p).applyForce(ml::vec3::Zero);
				pos = { pos[0], -ML_Time.sin(), pos[2] };
				rot = ml::quat::angleAxis(totalT, ml::vec3::One);
				
			}
			break;

			// Navball
			/* * * * * * * * * * * * * * * * * * * * */
			case RB_NAVBALL:
			{
				(*p).applyForce(ml::vec3::Zero);
				pos = { pos[0], -ML_Time.cos(), pos[2] };
				rot = ml::quat::angleAxis(totalT, ml::vec3::Forward);
			}
			break;

			// Moon
			/* * * * * * * * * * * * * * * * * * * * */
			case RB_MOON:
			{
				(*p).applyForce(ml::vec3::Zero);
				pos = { pos[0], +ML_Time.sin(), pos[2] };
				rot = ml::quat::angleAxis(totalT, ml::vec3::Up);
			}
			break;

			// Earth
			/* * * * * * * * * * * * * * * * * * * * */
			case RB_EARTH:
			{
				c_earth = !c_earth ? (dynamic_cast<ml::SphereCollider *>(c)) : c_earth;

				//(*p).applyForceLocation(ml::vec3::Left * 0.003f, p->centerMass_world + ml::vec3::Right)
				//	.integrateEulerSemiImplicit(deltaT)
				//	.convertForce()
				//	.convertTorque()
				//	.updateCenterMass()
				//	.updateInertiaTensor()
				//	.resetForce()
				//	.resetTorque()
				//	;

				(*p).applyForce(ml::Force::gravity(ml::vec3::Up, p->mass))
					.applyForceLocation(ml::vec3::Left, p->centerMass_world + ml::vec3(0.0f, 1.0f, 0.0f));
				
				if (c_earth && c_ground)
				{
					if (c_earth->checkCollision(*c_ground))
					{
						
						p->pos += c_earth->collPush;
						p->vel -= c_earth->collNorm * p->vel.dot(c_earth->collDelta);
						(*p).applyForce(ml::Force::normal(p->force, ml::vec3::Up));
						//friction
						ml::vec3 fric = ml::Force::frictionStatic(ml::vec3::Up, ml::vec3::Left, 0.48f);
						if (p->isMoving())
						{
							(*p).applyForceLocation(fric, c_earth->contactPoint);
						}
						
					}
				}

				(*p).integrateEulerSemiImplicit(deltaT)
					.convertForce()
					.convertTorque()
					.updateCenterMass()
					.updateInertiaTensor()
					.resetForce()
					.resetTorque()
					;

				//rot = ml::quat::angleAxis(totalT, ml::vec3::Up);
				rot = p->rotation;
				pos = p->pos;

				if (auto sphere = dynamic_cast<ml::SphereCollider *>(c))
				{
					sphere->center_world = pos;
				}
			}
			break;

			// Ground
			/* * * * * * * * * * * * * * * * * * * * */
			case RB_GROUND:
			{
				c_ground = !c_ground ? (dynamic_cast<ml::BoxCollider *>(c)) : c_ground;
				pos = p->pos;
			}
			break;
			}

			// Apply changes to copy state
			if (!state.set<state.T_Pos>(i, pos) ||
				!state.set<state.T_Rot>(i, rot) ||
				!state.set<state.T_Mat>(i, mat) ||
				!state.set<state.T_Inv>(i, inv))
			{
			}
		}
	}


	// Sync
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void demo_physics::sync(const ml::PhysicsState & state)
	{
		for (auto & pair : ML_Res.entities)
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
						.update		(ml::mat4::Identity())
						.translate	(pos)
						.rotate		(rot)
						.scale		(scl)
						;
				}
			}
		}
	}

}