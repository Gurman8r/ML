#ifndef _ML_ENGINE_HPP_
#define _ML_ENGINE_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/Timer.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class Application;
	class EventSystem;

	/* * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API Engine final
		: public ITrackable
		, public INonCopyable
	{
	public:
		Engine(EventSystem & eventSystem);
		~Engine();

	public:
		Application * launchApp(Application * app);
		int32_t freeApp(Application * value);
		bool isRunning() const;
	
	private:
		void beginFrame();
		void endFrame();

	public:
		template <
			class Fun, class ... Args
		> inline void loopFun(Fun && fun, Args && ... args)
		{
			while (this->isRunning())
			{
				this->beginFrame();
				fun((args)...);
				this->endFrame();
			}
		}

	public:
		inline const Application *	app()			const	{ return m_app;			}
		inline		 Application *	app()					{ return m_app;			}
		inline EventSystem &		getEventSystem()const	{ return m_eventSystem; }
		inline const Timer &		mainTimer()		const	{ return m_mainTimer;	}
		inline const Timer &		loopTimer()		const	{ return m_loopTimer;	}
		inline const Duration &		elapsed()		const	{ return m_elapsed;	}
		inline const uint32_t		frameRate()		const	{ return m_frameRate;	}

	private:
		Application *	m_app;
		EventSystem &	m_eventSystem;
		Timer			m_mainTimer;
		Timer			m_loopTimer;
		Duration		m_elapsed;
		uint32_t		m_frameCount;
		uint32_t		m_frameRate;
		float			m_nextSecond;
		float			m_prevSecond;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENGINE_HPP_