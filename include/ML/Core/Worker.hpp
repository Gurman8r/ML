#ifndef _ML_WORKER_HPP_
#define _ML_WORKER_HPP_

#include <ML/Core/Thread.hpp>
#include <ML/Core/Trigger.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct Worker final
		: public I_Disposable
		, public I_NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Trigger trigger {};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Worker() {}
		~Worker() { dispose(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool dispose() override
		{
			return reset().m_thread.dispose();
		}

		inline float_t progress() const
		{
			return ((isWorking()) ? (float_t)m_attempts / (float_t)m_count : 0.0f);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline Worker & note(bool success)
		{
			m_succeeded += (int32_t)(success);
			m_failed += (int32_t)(!success);
			m_attempts++;
			return (*this);
		}

		inline Worker & reset()
		{
			m_attempts = 0;
			m_succeeded = 0;
			m_failed = 0;
			m_count = 0;
			return (*this);
		}

		inline Worker & sleep(const Duration & value)
		{
			m_thread.sleep(value);
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class Fun = typename void(*)()
		> inline bool launch(size_t maxValue, Fun && fun)
		{
			if (isAvailable())
			{
				reset().m_count = (int32_t)maxValue;
				m_thread.launch(fun);
				return true;
			}
			return false;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool isAvailable()	const { return !m_thread.alive() && !isWorking(); }
		inline bool isDone()		const { return !isWorking() && (attempts() > 0); }
		inline bool isWorking()		const { return (count() > 0) && (attempts() < count()); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline int32_t attempts()	const { return m_attempts;	}
		inline int32_t succeeded()	const { return m_succeeded; }
		inline int32_t failed()		const { return m_failed;	}
		inline int32_t count()		const { return m_count;		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		Thread	m_thread	{};
		int32_t m_attempts	{ 0 };	// Total attempts
		int32_t m_succeeded	{ 0 };	// Successful attempts
		int32_t m_failed	{ 0 };	// Failed attempts
		int32_t m_count		{ 0 };	// Number of Elements

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_WORKER_HPP_