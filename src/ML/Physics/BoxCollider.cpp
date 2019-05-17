#include <ML/Physics/BoxCollider.hpp>
#include <ML/Physics/SphereCollider.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	BoxCollider::BoxCollider()
		: Collider(Collider::T_Box)
		, size(0.0f)
	{
	}

	BoxCollider::BoxCollider(const vec3 & size)
		: Collider(Collider::T_Box)
		, size(size)
	{
	}

	BoxCollider::BoxCollider(const BoxCollider & copy)
		: Collider(Collider::T_Box)
		, size(copy.size)
	{
	}

	BoxCollider::~BoxCollider()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool BoxCollider::checkCollision(const Collider & other) const
	{
		switch (other.getHullType())
		{
		case Collider::T_Box:
			if (auto temp = static_cast<const BoxCollider &>(other))
			{
				return false;
			}

		case Collider::T_Sphere:
			if (auto temp = static_cast<const SphereCollider &>(other))
			{
				return false;
			}

		default:
			return false;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}