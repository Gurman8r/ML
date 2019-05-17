#ifndef _ML_DISPATCHER_HPP_
#define _ML_DISPATCHER_HPP_

#include <ML/Core/Worker.hpp>
#include <ML/Core/List.hpp>

#define ML_Dispatcher ml::Dispatcher::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API Dispatcher final
		: public ITrackable
		, public ISingleton<Dispatcher>
	{
		friend class ISingleton<Dispatcher>;

	private:
		Dispatcher();
		~Dispatcher();

	public:
		bool init(size_t value);
		bool stop();

	public:
		void addRequest(IRequest * value);
		bool addWorker(Worker * value);

	private:
		std::queue<IRequest *>	m_requests;
		std::queue<Worker *>	m_workers;
		std::mutex				m_requestMutex;
		std::mutex				m_workersMutex;
		List<Worker *>			m_allWorkers;
		List<std::thread *>		m_threads;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DISPATCHER_HPP_