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

		struct State final : public I_NonNewable
		{
			size_t current	{ 0 };	// Working process index
			size_t success	{ 0 };	// Number of successful attempts
			size_t failure	{ 0 };	// Number of failed attempts
			size_t maximum	{ 0 };	// Total number of things to process
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Worker() {}
		~Worker() { dispose(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool dispose() override
		{
			return reset().m_thread.dispose();
		}

		inline Worker & finalize()
		{
			m_lock = false;
			return (*this);
		}

		template <
			class Fun, class ... Args
		> inline bool launch(size_t numJobs, Fun && fun, Args && ... args)
		{
			if (available())
			{
				m_lock = true;
				reset().m_state.maximum = numJobs;
				m_thread.launch(fun, std::forward<Args>(args)...);
				return true;
			}
			return false;
		}

		template <
			class Fun, class ... Args
		> inline bool process(Fun && fun, Args && ... args)
		{
			const bool result = fun(std::forward<Args>(args)...);
			m_state.success += (size_t)(result);
			m_state.failure += (size_t)(!result);
			m_state.current++;
			return result;
		}

		inline Worker & reset()
		{
			if (!working())
			{
				m_state.current	= 0;
				m_state.success	= 0;
				m_state.failure	= 0;
				m_state.maximum	= 0;
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
		
		inline bool available()	const { return !m_thread.alive() && !working(); }
		inline bool done()		const { return !working() && (current() > 0); }
		inline bool working()	const { return m_lock && (maximum() > 0) && (current() < maximum()); }
		inline auto current()	const -> size_t { return m_state.current; }
		inline auto failure()	const -> size_t { return m_state.failure; }
		inline auto maximum()	const -> size_t { return m_state.maximum; }
		inline auto progress()	const -> float_t { return alg::delta_cast<float_t>(current(), maximum()); }
		inline auto success()	const -> size_t { return m_state.success; }
		inline auto state()		const -> volatile const State &	{ return m_state; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		Thread			m_thread	= {};
		volatile State	m_state		= {};
		volatile bool	m_lock		= false;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_WORKER_HPP_