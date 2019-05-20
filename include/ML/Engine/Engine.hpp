#ifndef _ML_ENGINE_HPP_
#define _ML_ENGINE_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/Timer.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class Application;
	class EventSystem;
	class NetClient;
	class NetServer;
	class Preferences;
	class RenderWindow;
	class Resources;

	/* * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API Engine final
		: public ITrackable
		, public INonCopyable
	{
	public:
		Engine(
			EventSystem & eventSystem,
			Preferences & prefs,
			Resources	& resources,
			NetClient	& client,
			NetServer	& server
		);

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
		EventSystem	& eventSystem()	const;
		Preferences	& prefs()		const;
		Resources	& resources()	const;
		NetClient	& client()		const;
		NetServer	& server()		const;

	public:
		inline const Timer		&	mainTimer()	const { return m_mainTimer;	}
		inline const Timer		&	loopTimer()	const { return m_loopTimer;	}
		inline const uint32_t		frameRate()	const { return m_frameRate;	}
		inline const Duration	&	frameTime()	const { return m_frameTime;	}

	private:
		EventSystem &	m_eventSystem;
		Preferences	&	m_prefs;
		Resources	&	m_resources;
		NetClient	&	m_client;
		NetServer	&	m_server;
		
		Application *	m_app;
		Timer			m_mainTimer;
		Timer			m_loopTimer;
		Duration		m_frameTime;
		uint32_t		m_frameCounter;
		uint32_t		m_frameRate;
		float			m_nextSecond;
		float			m_prevSecond;
		bool			m_isClient;
		bool			m_isServer;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENGINE_HPP_