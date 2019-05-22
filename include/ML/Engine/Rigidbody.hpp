#ifndef _ML_RIGIDBODY_HPP_
#define _ML_RIGIDBODY_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/Transform.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class Collider;
	class Particle;

	/* * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API Rigidbody
		: public ITrackable
	{
	public:
		Rigidbody();
		Rigidbody(int32_t index, Transform * transform, Collider * collider, Particle * particle);
		Rigidbody(const Rigidbody & copy);
		~Rigidbody();

	public:
		inline const int32_t	index()		const	{ return m_index;		}
		
		inline const Collider * collider()	const	{ return m_collider;	}
		inline Collider *		collider()			{ return m_collider;	}
		
		inline const Particle * particle()	const	{ return m_particle;	}
		inline Particle *		particle()			{ return m_particle;	}
		
		inline const Transform *transform() const	{ return m_transform;	}
		inline Transform *		transform()			{ return m_transform;	}

		bool enabled = true;

	private:
		int32_t			m_index;
		Transform *		m_transform;
		Collider *		m_collider;
		Particle *		m_particle;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RIGIDBODY_HPP_