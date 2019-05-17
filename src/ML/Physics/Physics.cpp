#include <ML/Physics/Physics.hpp>
#include <ML/Physics/Rigidbody.hpp>
#include <ML/Physics/Collider.hpp>
#include <ML/Physics/Particle.hpp>
#include <ML/Core/Debug.hpp>

#define ML_GRAVITY -9.80665f

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	const vec3 Physics::Gravity(0.0f, ML_GRAVITY, 0.0f);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	Physics::Physics() 
		: m_updating(false)
		, m_state	()
		, m_mutex	()
		, m_thread	()
		, m_timer	()
		, m_elapsed	()
		, m_rb		()
	{
	}
	
	Physics::~Physics() { dispose(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Physics::dispose()
	{
		return m_thread.dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Physics::createLinkToRigidbody(Rigidbody * value)
	{
		int32_t i;
		if (value && ((i = value->index()) >= 0))
		{
			if (const Transform * transform = value->transform())
			{
				if (m_state.set<PhysicsState::T_Pos>(i, transform->getPos()) &&
					m_state.set<PhysicsState::T_Rot>(i, transform->getRot()) &&
					m_state.set<PhysicsState::T_Mat>(i, transform->getMat()) &&
					m_state.set<PhysicsState::T_Inv>(i, transform->getInv()))
				{
					m_rb.push_back(value);

					return ((m_state.m_size = (int32_t)m_rb.size()) > 0);
				}
				else
				{
					return Debug::logError("Physics | Failed setting Rigidbody data");
				}
			}
			else
			{
				return Debug::logError("Physics | Failed getting Rigidbody Transform");
			}
		}
		else
		{
			return Debug::logError("Physics | Invalid Rigidbody index");
		}
	}

	Rigidbody * Physics::getLinkedRigidbody(const int32_t index) const
	{
		return (((static_cast<size_t>(index) < m_rb.size()))
			? (m_rb[static_cast<size_t>(index)])
			: (NULL)
		);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Physics::beginUpdate(PhysicsState & value)
	{
		if (!m_updating)
		{
			m_updating = true;

			value.deepCopy(m_state);
			
			m_timer.start();

			m_mutex.lock();
			
			return true;
		}
		else
		{
			return Debug::logError("Physics | Unable to call Physics::beginUpdate");
		}
	}

	bool Physics::endUpdate(const PhysicsState & value)
	{
		if (m_updating)
		{
			m_updating = false;
			
			m_mutex.unlock();

			m_elapsed = m_timer.stop().elapsed();

			m_state.deepCopy(value);

			if (m_elapsed < Physics::TimeStep)
			{
				m_thread.sleep(Physics::TimeStep - m_elapsed);
			}
			
			return true;
		}
		else
		{
			return Debug::logError("Physics | Unable to call Physics::endUpdate");
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}