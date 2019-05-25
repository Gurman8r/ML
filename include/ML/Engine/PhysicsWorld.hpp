#ifndef _ML_PHYSICS_HPP_
#define _ML_PHYSICS_HPP_

#include <ML/Engine/PhysicsState.hpp>
#include <ML/Core/Timer.hpp>
#include <ML/Core/Thread.hpp>

#define ML_PHYSICS_TIMESTEP static_cast<Milliseconds>(15)

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class Rigidbody;

	/* * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API PhysicsWorld final
		: public IObject
		, public IDisposable
		, public INonCopyable
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * */
		static const vec3 Gravity;

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		PhysicsWorld();
		~PhysicsWorld();

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		bool dispose() override;

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		bool createLinkToRigidbody(Rigidbody * rb);

		template <class ... Args>
		inline Rigidbody * createNewRigidbody(Args && ... args)
		{
			if (Rigidbody * temp = new Rigidbody(std::forward<Args>(args)...))
			{
				if (createLinkToRigidbody(temp))
				{
					return temp;
				}
				delete temp;
				Debug::logError("Physics | Failed creating new Rigidbody");
			}
			return NULL;
		}
		
		Rigidbody * getLinkedRigidbody(const int32_t index) const;

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		bool beginUpdate(PhysicsState & value);
		bool endUpdate(const PhysicsState & value);

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		template <
			class Fun, 
			class ... Args
		> inline bool launchFun(Fun && fun, Args & ... args)
		{
			return m_thread.launch(fun, (args)...);
		}

		template <
			class Fun, 
			class ... Args
		> inline bool updateFun(Fun && fun, Args && ... args)
		{
			PhysicsState temp;
			if (beginUpdate(temp))
			{
				for (int32_t i = 0, imax = temp.size(); i < imax; i++)
				{
					fun(i, temp, (args)...);
				}
				return endUpdate(temp);
			}
			return false;
		}

		template <
			class Fun, 
			class ... Args
		> inline bool syncFun(Fun && fun, Args && ... args)
		{
			PhysicsState temp;
			if (!m_updating && temp.deepCopy(m_state))
			{
				fun(temp, (args)...);
			}
			return temp;
		}
		
	public:
		/* * * * * * * * * * * * * * * * * * * * */
		inline const PhysicsState &	state()		const	{ return m_state;	}
		inline const std::mutex &	mutex()		const	{ return m_mutex;	}
		inline const Thread &		thread()	const	{ return m_thread;	}
		inline const Timer &		timer()		const	{ return m_timer;	}
		inline const Duration &		elapsed()	const	{ return m_elapsed; }

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		bool			m_updating;
		PhysicsState	m_state;
		std::mutex		m_mutex;
		Thread			m_thread;
		Timer			m_timer;
		Duration		m_elapsed;

		List<Rigidbody *> m_rb;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PHYSICS_HPP_