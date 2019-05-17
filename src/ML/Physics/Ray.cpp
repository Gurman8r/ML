#include <ML/Physics/Ray.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Ray::Ray()
		: m_origin()
		, m_direction()
	{
	}

	Ray::Ray(const vec3 & origin, const vec3 & direction)
		: m_origin(origin)
		, m_direction(direction)
	{
	}

	Ray::Ray(const Ray & copy)
		: m_origin(copy.m_origin)
		, m_direction(copy.m_direction)
	{
	}

	Ray::~Ray()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */
}