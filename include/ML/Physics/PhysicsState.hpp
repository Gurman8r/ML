#ifndef _ML_PHYSICS_STATE_HPP_
#define _ML_PHYSICS_STATE_HPP_

#include <ML/Physics/Export.hpp>
#include <ML/Core/IDisposable.hpp>
#include <ML/Core/Vector3.hpp>
#include <ML/Core/Quaternion.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class PhysicsWorld;

	/* * * * * * * * * * * * * * * * * * * * */

	class ML_PHYSICS_API PhysicsState final
		: public ITrackable
		, public IDisposable
		, public INonCopyable
	{
		friend class PhysicsWorld;

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		enum Type : size_t
		{
			T_Pos,
			T_Rot,
			T_Mat,
			T_Inv,

			MAX_PHYSICS_STATE_TYPE
		};


	private:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		PhysicsState();
		~PhysicsState();


	public: // Helper Functions
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		PhysicsState &	deepCopy(const PhysicsState & other);
		bool			dispose() override;
		bool			empty() const;
		PhysicsState &	resize(const size_t value);
		int32_t			size() const;


	public: // Operators
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		inline operator bool() const { return !(this->empty()); }


	private: // Getters
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		template <class T>
		inline bool get(const int32_t index, const List<T> & data, T & value) const
		{
			if (static_cast<size_t>(index) < data.size())
			{
				value = T(data[static_cast<size_t>(index)]);
				return true;
			}
			return false;
		}

	public:
		// Get Vec3
		template <Type ID, class T = vec3> 
		inline bool get(const int32_t index, vec3 & value) const
		{
			switch (ID)
			{
			case PhysicsState::T_Pos: return get<vec3>(index, m_pos, value);
			default:
				return false;
			}
		}

		// Get Quat
		template <Type ID, class T = quat> 
		inline bool get(const int32_t index, quat & value) const
		{
			switch (ID)
			{
			case PhysicsState::T_Rot: return get<quat>(index, m_rot, value);
			default:
				return false;
			}
		}

		// Get Mat4
		template <Type ID, class T = mat4> 
		inline bool get(const int32_t index, mat4 & value) const
		{
			switch (ID)
			{
			case PhysicsState::T_Mat: return get<mat4>(index, m_mat, value);
			case PhysicsState::T_Inv: return get<mat4>(index, m_inv, value);
			default:
				return false;
			}
		}


	private: // Setters
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		template <class T> 
		inline bool set(const int32_t index, List<T> & data, const T & value)
		{
			if (index >= 0)
			{
				if (static_cast<size_t>(index) < data.size())
				{
					data[static_cast<size_t>(index)] = T(value);
					return true;
				}
				else
				{
					data.resize(static_cast<size_t>(index + 1));
					return set(index, data, value);
				}
			}
			else
			{
				return false;
			}
		}

	public:
		// Set Vec3
		template <Type ID, class T = vec3> 
		inline bool set(const int32_t index, const vec3 & value)
		{
			switch (ID)
			{
			case PhysicsState::T_Pos: return set<vec3>(index, m_pos, value);
			default:
				return false;
			}
		}

		// Set Quat
		template <Type ID, class T = quat> 
		inline bool set(const int32_t index, const quat & value)
		{
			switch (ID)
			{
			case PhysicsState::T_Rot: return set<quat>(index, m_rot, value);
			default: 
				return false;
			}
		}

		// Set Mat4
		template <Type ID, class T = mat4> 
		inline bool set(const int32_t index, const mat4 & value)
		{
			switch (ID)
			{
			case PhysicsState::T_Mat: return set<mat4>(index, m_mat, value);
			case PhysicsState::T_Inv: return set<mat4>(index, m_inv, value);
			default:
				return false;
			}
		}


	private:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		int32_t		m_size;
		List<vec3>	m_pos;
		List<quat>	m_rot;
		List<mat4>	m_mat;
		List<mat4>	m_inv;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PHYSICS_STATE_HPP_