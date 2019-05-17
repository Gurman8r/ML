#include <ML/Physics/Particle.hpp>
#include <ML/Physics/Physics.hpp>
#include <ML/Physics/Rigidbody.hpp>
#include <ML/Physics/BoxCollider.hpp>
#include <ML/Physics/SphereCollider.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */
	
	Particle::Particle()
		: index					(-1)
		, pos					()
		, vel					()
		, acc					()
		, force					()
		, mass					(0.0f)
		, massInv				(0.0f)
		, rotation				()
		, angularVel			()
		, angularAcc			()
		, angularMomentum		()
		, torque				()
		, inertiaTensor			()
		, inertiaTensorInv		()
		, inertiaTensor_world	()
		, inertiaTensorInv_world()
		, centerMass			()
		, centerMass_world		()
	{
	}

	Particle::Particle(const vec3 & pos, const float mass)
		: index					(-1)
		, pos					(pos)
		, vel					()
		, acc					()
		, force					()
		, mass					()
		, massInv				()
		, rotation				()
		, angularVel			()
		, angularAcc			()
		, angularMomentum		()
		, torque				()
		, inertiaTensor			()
		, inertiaTensorInv		()
		, inertiaTensor_world	()
		, inertiaTensorInv_world()
		, centerMass			()
		, centerMass_world		()
	{
		setMass(mass);
	}

	Particle::Particle(const Particle & copy)
		: index					(copy.index)
		, pos					(copy.pos)
		, vel					(copy.vel)
		, acc					(copy.acc)
		, force					(copy.force)
		, mass					(copy.mass)
		, massInv				(copy.massInv)
		, rotation				(copy.rotation)
		, angularVel			(copy.angularVel)
		, angularAcc			(copy.angularAcc)
		, angularMomentum		(copy.angularMomentum)
		, torque				(copy.torque)
		, inertiaTensor			(copy.inertiaTensor)
		, inertiaTensorInv		(copy.inertiaTensorInv)
		, inertiaTensor_world	(copy.inertiaTensor_world)
		, inertiaTensorInv_world(copy.inertiaTensorInv_world)
		, centerMass			(copy.centerMass)
		, centerMass_world		(copy.centerMass_world)
	{
	}

	Particle::~Particle()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Particle::isMoving() const
	{
		return (vel.sqrMagnitude() != 0.0f);
	}

	Particle & Particle::applyForce(const vec3 & value)
	{
		force += value;
		return (*this);
	}

	Particle & Particle::convertForce()
	{
		acc += (force / mass);
		return (*this);
	}

	Particle & Particle::resetForce()
	{
		force = vec3::Zero;
		return (*this);
	}

	Particle & Particle::setMass(float value)
	{
		if (value == 0.0f) { value = FLT_MIN; }
		mass = value;
		massInv = (1.0f / value);
		return (*this);
	}

	Particle & Particle::reset()
	{
		pos			= vec3::Zero;
		vel			= vec3::Zero;
		acc			= vec3::Zero;
		force		= vec3::Zero;
		momentum	= vec3::Zero;

		rotation = glm::quat();
		angularVel = 0.0f;
		angularAcc = 0.0f;
		angularMomentum = 0.0f;
		torque = 0.0f;
		inertiaTensor = inertiaTensorInv = mat3::Identity();
		inertiaTensor_world = inertiaTensorInv_world = mat3::Identity();
		centerMass = 0.0f;
		centerMass_world = 0.0f;

		return (*this);
	}

	bool Particle::isRotating()
	{
		return (angularVel.sqrMagnitude() != 0.0f);
	}

	Particle & Particle::applyForceLocation(const vec3 & force, const vec3 worldLoc)
	{
		vec3 arm = worldLoc - centerMass_world;
		//according to this article:
		//https://gafferongames.com/post/physics_in_3d/
		// the force in its entirety is applied to torque and linear force
		this->force += force;
		torque += vec3::cross((arm), force);
		//Debug::log("Direct: {0}\t Force: {1}", directComp, force);

		//cout 
		//	<< "Arm: " << arm << endl
		//	<< "Direct: " << directComp << endl;

		return (*this);

		/*
		forceAtLocation;
		arm = location - centermass;
		direct = project forceAtLocation onto Arm;
		rotational = forceAtLocation - direct;
		
		*/
	}

	Particle & Particle::convertTorque()
	{
		angularAcc = (glm::mat3)(inertiaTensorInv_world) * (glm::vec3)(torque);
		return (*this);
	}

	Particle & Particle::resetTorque()
	{
		torque = 0.0f;
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Particle & Particle::setInertiaTensor(const Rigidbody * rb)
	{
		//if (const Rigidbody * rb = ML_Physics.getLinkedRigidbody(index))
		if (rb)
		{
			this->index = rb->index();

			glm::mat3 temp = glm::mat3();
			switch (rb->collider()->getHullType())
			{
			case Collider::T_Box:
				if (auto c = dynamic_cast<const BoxCollider *>(rb->collider()))
				{
					float w = c->size[0];
					float h = c->size[1];
					float d = c->size[2];

					float coeff = mass * 5.0f / 3.0f;
					temp[0][0] *= coeff * ((h*h) + (d * d));
					temp[1][1] *= coeff * ((w*w) + (d * d));
					temp[2][2] *= coeff * ((h*h) + (w * w));

					inertiaTensor = temp;
					inertiaTensorInv = glm::inverse(temp);
				}
				break;
			case Collider::T_Sphere:
				if (auto c = dynamic_cast<const SphereCollider *>(rb->collider()))
				{
					float r = c->radius;

					float coeff = mass * r * r * 2.0f / 3.0f;
					temp *= coeff;
					inertiaTensor = temp;
					inertiaTensorInv = glm::inverse(temp);
				}
				break;

			default:
				break;
			}
		}
		return (*this);
	}

	vec3 Particle::rotateForce(Particle * p, vec3 force)
	{
		vec3 wv = force * p->rotation[3];
		vec3 cross1 = vec3::cross(p->rotation, force);
		vec3 r2 = p->rotation.complex() * 2.0f;
		vec3 sum = cross1 + wv;
		vec3 cross2 = vec3::cross(r2, sum);
		force += cross2;
		return force;
	}

	Particle & Particle::integrateEulerExplicit(const float dt)
	{
		pos += (vel * dt);
		vel += (acc * dt);

		vec4 rot = rotation;

		vec3 cr = vec3::cross(angularVel, rot);
		vec3 pr = angularVel * rot[3];
		vec3 ad = cr + pr;
		float dot = -(angularVel.dot(rot));
		vec4 q = ad;
		q[3] = dot;

		//glm::quat q = ((glm::vec3)angularVel * (glm::quat)rotation);
		q *= (dt / 2.0f);
		rotation += (quat)q;
		rotation.normalize();

		angularVel += angularAcc * dt;

		return (*this);
	}

	Particle & Particle::integrateEulerSemiImplicit(const float dt)
	{
		vel += (acc * dt);
		pos += (vel * dt);

		angularVel += angularAcc * dt;
		vec4 rot = rotation;

		vec3 cr = vec3::cross(angularVel, rot);
		vec3 pr = angularVel * rot[3];
		vec3 ad = cr + pr;
		float dot = -(angularVel.dot(rot));
		vec4 q = ad;
		q[3] = dot;

		//glm::quat q = ((glm::vec3)angularVel * (glm::quat)rotation);
		q *= (dt / 2.0f);
		rotation += (quat)q;
		rotation.normalize();
		return (*this);
	}

	Particle & Particle::integrateEulerKinematic(const float dt)
	{
		pos += ((vel * dt) + (acc * (dt * dt * 0.5f)));
		return (*this);
	}

	Particle & Particle::updateInertiaTensor()
	{
		if (auto rb = ML_Physics.getLinkedRigidbody(index))
		{
			if (const Transform * transform = rb->transform())
			{
				inertiaTensor_world = Transform::RebaseMatrix(inertiaTensor, transform->getMat());
				inertiaTensorInv_world = Transform::RebaseMatrix(inertiaTensorInv, transform->getMat());
			}
		}
		return (*this);
	}

	Particle & Particle::updateCenterMass()
	{
		if (auto rb = ML_Physics.getLinkedRigidbody(index))
		{
			if (const Transform * transform = rb->transform())
			{
				centerMass_world = Transform::RebasePoint(centerMass, transform->getMat());
			}
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}