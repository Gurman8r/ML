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
	class PhysicsWorld;
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
			RenderWindow	& window
		);

		~Engine() {}

	public:
		void onEvent(const IEvent * value) override;

	public:
		Application * launchApp(Application * value);
		int32_t freeApp(Application * value);

	public:
		bool isRunning() const;
		void beginFrame();
		void endFrame();

	public:
		EventSystem		& eventSystem()	const;
		Preferences		& prefs()		const;
		Resources		& resources()	const;
		GameTime		& time()		const;
		RenderWindow	& window()		const;

	private:
		Application		* m_app;
		EventSystem		& m_eventSystem;
		Preferences		& m_prefs;
		Resources		& m_resources;
		GameTime		& m_time;
		RenderWindow	& m_window;
		
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENGINE_HPP_