#ifndef _ML_ENGINE_HPP_
#define _ML_ENGINE_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/Timer.hpp>
#include <ML/Core/IEventListener.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class Application;
	class Audio;
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
		, public IEventListener
	{
	public:
		explicit Engine(
			EventSystem		& eventSystem,
			Preferences		& prefs,
			Resources		& resources,
			RenderWindow	& window,
			NetClient		& client,
			NetServer		& server,
			Audio			& audio
		);

		~Engine();

	public:
		void onEvent(const IEvent * value) override;

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
		Audio			& audio()		const;
		NetClient		& client()		const;
		EventSystem		& eventSystem()	const;
		Preferences		& prefs()		const;
		Resources		& resources()	const;
		NetServer		& server()		const;
		RenderWindow	& window()		const;

	public:
		inline const Timer		&	mainTimer()	const { return m_mainTimer;	}
		inline const Timer		&	loopTimer()	const { return m_loopTimer;	}
		inline const Duration	&	frameTime()	const { return m_frameTime;	}
		inline const uint32_t		frameRate()	const { return m_frameRate;	}
		inline const int32_t		netMode()	const { return m_netMode;	}

	private:
		Audio			&	m_audio;
		NetClient		&	m_client;
		EventSystem		&	m_eventSystem;
		Preferences		&	m_prefs;
		Resources		&	m_resources;
		NetServer		&	m_server;
		RenderWindow	&	m_window;
		
		Application *	m_app;
		Timer			m_mainTimer;
		Timer			m_loopTimer;
		Duration		m_frameTime;
		uint32_t		m_frameCounter;
		uint32_t		m_frameRate;
		float			m_nextSecond;
		float			m_prevSecond;
		int32_t			m_netMode;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENGINE_HPP_