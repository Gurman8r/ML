#ifndef _ML_ENGINE_HPP_
#define _ML_ENGINE_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/ITrackable.hpp>
#include <ML/Core/IEventListener.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Application;
	class Audio;
	class EventSystem;
	class GameTime;
	class NetClient;
	class NetServer;
	class Preferences;
	class RenderWindow;
	class Resources;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API Engine final
		: public ITrackable
		, public INonCopyable
		, public IEventListener
	{
	public:
		explicit Engine(
			EventSystem		& eventSystem,
			Preferences		& prefs,
			GameTime		& time,
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
		bool	isRunning() const;
		void	beginFrame();
		void	endFrame();

	public:
		Audio			& audio()		const;
		NetClient		& client()		const;
		EventSystem		& eventSystem()	const;
		Preferences		& prefs()		const;
		Resources		& resources()	const;
		NetServer		& server()		const;
		RenderWindow	& window()		const;
		GameTime		& time()		const;

	private:
		Audio			& m_audio;
		NetClient		& m_client;
		EventSystem		& m_eventSystem;
		Preferences		& m_prefs;
		Resources		& m_resources;
		NetServer		& m_server;
		RenderWindow	& m_window;
		GameTime		& m_time;
		
		Application *	m_app;
		int32_t			m_networkMode;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENGINE_HPP_