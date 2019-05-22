#include <ML/Engine/SphereCollider.hpp>
#include <ML/Engine/BoxCollider.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	SphereCollider::SphereCollider()
		: Collider(Collider::T_Sphere)
		, radius(0.0f)
	{
	}

	SphereCollider::SphereCollider(const float radius)
		: Collider(Collider::T_Sphere)
		, radius(radius)
	{
	}

	SphereCollider::SphereCollider(const SphereCollider & copy)
		: Collider(Collider::T_Sphere)
		, radius(copy.radius)
	{
	}

	SphereCollider::~SphereCollider()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool SphereCollider::checkCollision(const Collider & other) const
	{
		switch (other.getHullType())
		{
		case Collider::T_Box:
			if (auto temp = dynamic_cast<const BoxCollider *>(&other))
			{
				auto clamp = [](auto value, auto min, auto max)
				{
					return std::max(min, std::min(value, max));
				};

				vec3 point(
					clamp(center_world[0], temp->center_world[0] - temp->size[0], temp->center_world[0] + temp->size[0]),
					clamp(center_world[1], temp->center_world[1] - temp->size[1], temp->center_world[1] + temp->size[1]),
					clamp(center_world[2], temp->center_world[2] - temp->size[2], temp->center_world[2] + temp->size[2])
				);
				//float dist = vec3::Distance(point, center_world);
				vec3 delta = center_world - point;
				float dist = delta.magnitude();
				vec3 Norm = delta / dist;
				if (dist < radius && dist != 0)
				{
					//Debug::log("HIT");
					collideFlag = true;
					contactPoint = point;
					collPush = Norm * (radius - dist);
					collNorm = Norm;
					collDelta = delta;
					return true;
				}
				//Debug::log("dist: {0}", dist);
				//Debug::log("other center: {0}\n", temp->center_world);

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