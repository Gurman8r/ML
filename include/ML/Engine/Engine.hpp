#ifndef _ML_ENGINE_HPP_
#define _ML_ENGINE_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/Timer.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class Application;
	class EventSystem;
	class Preferences;
	class Resources;

	/* * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API Engine final
		: public ITrackable
		, public INonCopyable
	{
	public:
		Engine(EventSystem & eventSystem, Preferences & prefs, Resources & resources);
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
		EventSystem &	eventSystem()	const;
		Preferences &	prefs()			const;
		Resources	&	resources()		const;

	public:
		inline const	Application *	app()			const	{ return m_application;	}
		inline			Application *	app()					{ return m_application;	}
		inline const	Timer		&	mainTimer()		const	{ return m_mainTimer;	}
		inline const	Timer		&	loopTimer()		const	{ return m_loopTimer;	}
		inline const	Duration	&	elapsed()		const	{ return m_elapsed;		}
		inline const	uint32_t		frameRate()		const	{ return m_frameRate;	}

	private:
		EventSystem &	m_eventSystem;
		Preferences	&	m_prefs;
		Resources	&	m_resources;
		
		Application *	m_application;
		Timer			m_mainTimer;
		Timer			m_loopTimer;
		Duration		m_elapsed;
		uint32_t		m_frameCounter;
		uint32_t		m_frameRate;
		float			m_nextSecond;
		float			m_prevSecond;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENGINE_HPP_