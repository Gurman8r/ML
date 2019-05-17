#include <ML/Core/Worker.hpp>
#include <ML/Core/Dispatcher.hpp>
#include <ML/Core/Duration.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Worker::Worker()
		: m_running(true)
		, m_ready(false)
		, m_request(NULL)
		, m_mutex()
		, m_lock(UniqueLock(m_mutex))
		, m_cv()
	{
	}

	Worker::~Worker()
	{
	}
	
	/* * * * * * * * * * * * * * * * * * * * */

	void Worker::run()
	{
		while (m_running)
		{
			if (m_ready)
			{
				m_ready = false;
				m_request->process();
				m_request->finish();
			}

			if (ML_Dispatcher.addWorker(this))
			{
				while (!m_ready && m_running)
				{
					if (m_cv.wait_for(m_lock, Seconds(1)) == std::cv_status::timeout)
					{
						// timed out, keep waiting unless worker is stopped by dispatch
					}
				}
			}
		}
	}
	
	void Worker::stop()
	{
		m_running = false;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Worker::setRequest(IRequest * value)
	{
		m_request = value;
		m_ready = true;
	}
	
	void Worker::getCondition(Condition *& value)
	{
		value = &m_cv;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}