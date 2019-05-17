#include <ML/Core/Dispatcher.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Dispatcher::Dispatcher()
	{
	}

	Dispatcher::~Dispatcher()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Dispatcher::init(size_t value)
	{
		std::thread * t = NULL;
		Worker * w = NULL;

		for (size_t i = 0; i < value; i++)
		{
			w = new Worker();
			
			m_allWorkers.push_back(w);

			t = new std::thread(&Worker::run, w);

			m_threads.push_back(t);
		}

		return (bool)(value);
	}

	bool Dispatcher::stop()
	{
		for (Worker * w : m_allWorkers)
		{
			w->stop();
		}
		Debug::log("Stopped Workers");

		for (std::thread * t : m_threads)
		{
			t->join();
		}
		Debug::log("Joined Threads");

		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Dispatcher::addRequest(IRequest * value)
	{
		m_workersMutex.lock();

		if (!m_workers.empty())
		{
			Worker * w = m_workers.front();
			w->setRequest(value);
			Worker::Condition * cv;
			w->getCondition(cv);
			cv->notify_one();
			m_workers.pop();
			m_workersMutex.unlock();
		}
		else
		{
			m_workersMutex.unlock();
			m_requestMutex.lock();
			m_requests.push(value);
			m_requestMutex.unlock();
		}
	}

	bool Dispatcher::addWorker(Worker * value)
	{
		bool wait = true;
		
		m_requestMutex.lock();
		
		if (!m_requests.empty())
		{
			IRequest * r = m_requests.front();
			value->setRequest(r);
			m_requests.pop();
			wait = false;
			m_requestMutex.unlock();
		}
		else
		{
			m_requestMutex.unlock();
			m_workersMutex.lock();
			m_workers.push(value);
			m_workersMutex.unlock();
		}
		return wait;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}