#include <ML/Engine/Rigidbody.hpp>
#include <ML/Engine/PhysicsWorld.hpp>
#include <ML/Engine/Collider.hpp>
#include <ML/Engine/Particle.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Rigidbody::Rigidbody()
		: m_index		(-1)
		, m_transform	(nullptr)
		, m_collider	(nullptr)
		, m_particle	(nullptr)
	{
	}

	Rigidbody::Rigidbody(int32_t index, Transform * transform, Collider * collider, Particle * particle)
		: m_index		(index)
		, m_transform	(transform)
		, m_collider	(collider)
		, m_particle	(particle)
	{
		if (m_particle)
		{
			m_particle->setInertiaTensor(this);
		}
	}

	Rigidbody::Rigidbody(const Rigidbody & copy)
		: m_index		(copy.m_index)
		, m_transform	(copy.m_transform)
		, m_collider	(copy.m_collider)
		, m_particle	(copy.m_particle)
	{
	}

	Rigidbody::~Rigidbody()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */
}