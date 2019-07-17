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

		struct State final
		{
			size_t current	{ 0 };	// Working index
			size_t failures	{ 0 };	// Failed attempts
			size_t successes{ 0 };	// Successful attempts
			size_t maximum	{ 0 };	// Number of things to do
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Worker() {}
		~Worker() { dispose(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool dispose() override
		{
			return reset().m_thread.dispose();
		}

		template <
			class Fun, class ... Args
		> inline bool launch(size_t numJobs, Fun && fun, Args && ... args)
		{
			if (available())
			{
				reset().m_state.maximum = numJobs;
				m_thread.launch(fun, std::forward<Args>(args)...);
				return true;
			}
			return false;
		}

		template <
			class Fun, class ... Args
		> inline Worker & process(Fun && fun, Args && ... args)
		{
			const bool result = fun(std::forward<Args>(args)...);
			m_state.successes += (size_t)(result);
			m_state.failures += (size_t)(!result);
			m_state.current++;
			return (*this);
		}

		inline Worker & reset()
		{
			if (!working())
			{
				m_state.current		= 0;
				m_state.failures	= 0;
				m_state.successes	= 0;
				m_state.maximum		= 0;
			}
			return (*this);
		}

		inline Worker & sleep(const Duration & value)
		{
			if (working())
			{
				m_thread.sleep(value);
			}
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		inline auto available()	const -> bool			{ return !m_thread.alive() && !working(); }
		inline auto done()		const -> bool			{ return !working() && (current() > 0); }
		inline auto current()	const -> size_t			{ return m_state.current; }
		inline auto failures()	const -> size_t			{ return m_state.failures; }
		inline auto maximum()	const -> size_t			{ return m_state.maximum; }
		inline auto progress()	const -> float_t		{ return alg::delta_cast<float_t>(current(), maximum()); }
		inline auto state()		const -> const State &	{ return m_state; }
		inline auto successes()	const -> size_t			{ return m_state.successes; }
		inline auto working()	const -> bool			{ return (maximum() > 0) && (current() < maximum()); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		State	m_state;
		Thread	m_thread;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_WORKER_HPP_