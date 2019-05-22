#ifndef _ML_SPHERE_COLLIDER_HPP_
#define _ML_SPHERE_COLLIDER_HPP_

#include <ML/Engine/Collider.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API SphereCollider final
		: public Collider
	{
	public:
		SphereCollider();
		SphereCollider(const float radius);
		SphereCollider(const SphereCollider & copy);
		~SphereCollider();

	public:
		bool checkCollision(const Collider & other) const override;

	public:
		float radius;
		vec3 center_world;
		mutable bool collideFlag = false;
		mutable vec3 contactPoint, collPush, collNorm, collDelta;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SPHERE_COLLIDER_HPP_