#include <ML/Physics/PhysicsState.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	PhysicsState::PhysicsState()  { dispose(); }

	PhysicsState::~PhysicsState() { dispose(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	PhysicsState & PhysicsState::deepCopy(const PhysicsState & other)
	{
		if (other)
		{
			m_size	= other.m_size;
			m_pos	= other.m_pos;
			m_rot	= other.m_rot;
			m_mat	= other.m_mat;
			m_inv	= other.m_inv;
		}
		else
		{
			dispose();
		}
		return (*this);
	}

	bool PhysicsState::dispose()
	{
		m_size = 0;
		m_pos.clear();
		m_rot.clear();
		m_mat.clear();
		m_inv.clear();
		return empty();
	}

	bool PhysicsState::empty() const
	{
		return (size() == 0);
	}

	PhysicsState & PhysicsState::resize(const size_t value)
	{
		if (m_size = (int32_t)(value))
		{
			m_pos.resize(m_size);
			m_rot.resize(m_size);
			m_mat.resize(m_size);
			m_inv.resize(m_size);
		}
		return (*this);
	}

	int32_t PhysicsState::size() const
	{
		if (m_size)
		{
			if ((m_pos.size() != m_size) ||
				(m_rot.size() != m_size) ||
				(m_mat.size() != m_size) ||
				(m_inv.size() != m_size))
			{
				Debug::logError("Physics | State data size mismatch");
			}
		}
		return m_size;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}