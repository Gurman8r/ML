#ifndef _ML_LOCK_HPP_
#define _ML_LOCK_HPP_

#include <ML/Core/ITrackable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	using Mutex = typename std::mutex;

	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class _Mtx,
		class _Lck = std::unique_lock<_Mtx>
	> struct Lock
		: public ITrackable
		, public INonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * */

		using mutex_type = typename _Mtx;
		using lock_type  = typename _Lck;

		/* * * * * * * * * * * * * * * * * * * * */

		mutex_type	mtx;
		lock_type	lck;

		/* * * * * * * * * * * * * * * * * * * * */

		Lock()
			: mtx()
			, lck(mtx)
		{
		}

		virtual ~Lock() {}

		/* * * * * * * * * * * * * * * * * * * * */

		virtual void lock() = 0;
		virtual void unlock() = 0;

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct MutexLock final : public Lock<Mutex>
	{
		MutexLock() : Lock() {}

		~MutexLock() {}

		inline void lock() override
		{
			this->lck.lock();
		}

		inline void unlock() override
		{
			this->lck.unlock();
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_LOCK_HPP_