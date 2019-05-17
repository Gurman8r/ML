#ifndef _ML_BOX_COLLIDER_HPP_
#define _ML_BOX_COLLIDER_HPP_

#include <ML/Physics/Collider.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_PHYSICS_API BoxCollider final
		: public Collider
	{
	public:
		BoxCollider();
		BoxCollider(const vec3 & size);
		BoxCollider(const BoxCollider & copy);
		~BoxCollider();

	public:
		bool checkCollision(const Collider & other) const override;

	public:
		vec3 size;
		vec3 center_world;

	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_BOX_COLLIDER_HPP_