#ifndef _ML_WORKER_HPP_
#define _ML_WORKER_HPP_

#include <ML/Core/IRequest.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API Worker
		: public ITrackable
	{
	public:
		using Condition		= std::condition_variable;
		using UniqueLock	= std::unique_lock<std::mutex>;

	public:
		Worker();
		~Worker();

	public:
		void run();
		void stop();

	public:
		void setRequest(IRequest * value);
		void getCondition(Condition * & value);

	private:
		bool		m_running;
		bool		m_ready;
		IRequest *	m_request;
		std::mutex	m_mutex;
		UniqueLock	m_lock;
		Condition	m_cv;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_WORKER_HPP_