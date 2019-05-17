#ifndef _ML_COLLIDER_HPP_
#define _ML_COLLIDER_HPP_

#include <ML/Physics/Export.hpp>
#include <ML/Core/Transform.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_PHYSICS_API Collider
		: public ITrackable
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * */
		enum Type : int32_t
		{
			T_None,
			T_Point,
			T_Plane,
			T_Disc,
			T_Box,
			T_Sphere,
			T_Cylinder,
			T_Mesh,

			MAX_COLLIDER_TYPE
		};

		enum Flag : int32_t
		{
			F_None,
			F_3D,
			F_AxisAligned,

			MAX_COLLIDER_FLAG
		};

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		Collider(const int32_t hulltype)
			: m_hullType(hulltype)
		{
		}

		virtual ~Collider() {}


	public:
		/* * * * * * * * * * * * * * * * * * * * */
		virtual bool checkCollision(const Collider & other) const = 0;


	public:
		/* * * * * * * * * * * * * * * * * * * * */
		inline const int32_t & getHullType() const { return m_hullType; }

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		inline operator bool() const 
		{
			return (getHullType() > T_None) && (getHullType() < MAX_COLLIDER_TYPE);
		}

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		const int32_t m_hullType;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_COLLIDER_HPP_