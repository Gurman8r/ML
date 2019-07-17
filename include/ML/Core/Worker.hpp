#ifndef _ML_WORKER_HPP_
#define _ML_WORKER_HPP_

#include <ML/Core/Thread.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct Worker final
		: public I_Disposable
		, public I_NonCopyable
		, public I_Newable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Worker() {}
		~Worker() { dispose(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool dispose() override
		{
			return reset().m_thread.dispose();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class Fun, class ... Args
		> inline Worker & process(Fun && fun, Args && ... args)
		{
			bool result = fun(std::forward<Args>(args)...);
			m_succeeded += (size_t)(result);
			m_failed += (size_t)(!result);
			m_attempts++;
			return (*this);
		}

		inline Worker & reset()
		{
			if (!isWorking())
			{
				m_attempts	= 0;
				m_failed	= 0;
				m_incomplete	= 0;
				m_succeeded = 0;
			}
			return (*this);
		}

		inline Worker & sleep(const Duration & value)
		{
			if (isWorking())
			{
				m_thread.sleep(value);
			}
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class Fun, class ... Args
		> inline bool launch(size_t numJobs, Fun && fun, Args && ... args)
		{
			if (isAvailable())
			{
				reset().m_incomplete = numJobs;
				m_thread.launch(fun, std::forward<Args>(args)...);
				return true;
			}
			return false;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline size_t attempts()	const { return m_attempts; }
		inline size_t failed()		const { return m_failed; }
		inline size_t incomplete()	const { return m_incomplete; }
		inline size_t succeeded()	const { return m_succeeded; }
		
		inline float_t progress() const
		{
			return ((isWorking()) ? (float_t)attempts() / (float_t)incomplete() : 0.0f);
		}
		
		inline bool isAvailable()	const { return !m_thread.alive() && !isWorking(); }
		inline bool isDone()		const { return !isWorking() && (attempts() > 0); }
		inline bool isWorking()		const { return (incomplete() > 0) && (attempts() < incomplete()); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		Thread m_thread		{};
		size_t m_attempts	{ 0 };	// Total attempts
		size_t m_failed		{ 0 };	// Failed attempts
		size_t m_incomplete	{ 0 };	// Total number of jobs
		size_t m_succeeded	{ 0 };	// Successful attempts

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_WORKER_HPP_