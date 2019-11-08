#ifndef _ML_THREAD_HPP_
#define _ML_THREAD_HPP_

#include <ML/Core/Duration.hpp>
#include <ML/Core/Disposable.hpp>
#include <ML/Core/Newable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Wrapper and manager for std::thread
	struct Thread final : public Newable, public Disposable, public NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Thread() : m_thr(nullptr) {}

		~Thread() { this->dispose(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool alive() const
		{
			return m_thr;
		}

		inline bool joinable() const
		{
			return alive() && m_thr->joinable();
		}

		inline bool dispose() override
		{
			if (joinable())
			{
				m_thr->join();
			}
			if (alive())
			{
				delete m_thr;
				m_thr = nullptr;
			}
			return !(m_thr);
		}

		inline Thread & sleep(const Duration & value)
		{
			if (joinable())
			{
				std::this_thread::sleep_for(value.base());
			}
			return (*this);
		}

		template <
			class Fun, class ... Args, class = std::enable_if_t<
				!std::is_same_v<std::remove_cv_t<std::remove_reference_t<Fun>>, std::thread>
			>
		> inline std::thread * launch(Fun && fun, Args && ... args)
		{
			return (!alive()
				? (m_thr = new std::thread(fun, std::forward<Args>(args)...))
				: nullptr
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: std::thread * m_thr;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_THREAD_HPP_