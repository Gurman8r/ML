#ifndef _ML_ENGINE_HPP_
#define _ML_ENGINE_HPP_

#include <ML/Engine/EngineEvents.hpp>
#include <ML/Engine/ContentManager.hpp>
#include <ML/Engine/GameTime.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Engine/CommandRegistry.hpp>
#include <ML/Engine/PluginManager.hpp>
#include <ML/Graphics/RenderWindow.hpp>

#ifndef ML_CONFIG_INI
#define ML_CONFIG_INI "../../../ML.ini"
#endif

#define ML_Engine ::ml::Engine::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API Engine final : public Singleton<Engine>, public EventListener
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		friend struct Singleton<Engine>;

		Engine();

		~Engine();
		
		void onEvent(const Event & value) override;
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void onEnter		(const EnterEvent & ev);
		void onLoad			(const LoadEvent & ev);
		void onBeginStep	(const BeginStepEvent & ev);
		void onUpdate		(const UpdateEvent & ev);
		void onBeginDraw	(const BeginDrawEvent & ev);
		void onDraw			(const DrawEvent & ev);
		void onEndDraw		(const EndDrawEvent & ev);
		void onEndStep		(const EndStepEvent & ev);
		void onUnload		(const UnloadEvent & ev);
		void onExit			(const ExitEvent & ev);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		CommandRegistry	m_commands;
		ContentManager	m_content;
		PluginManager	m_plugins;
		Preferences		m_prefs;
		GameTime		m_time;
		RenderWindow	m_window;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:
		inline auto & commands()	{ return m_commands; }
		inline auto & content()		{ return m_content; }
		inline auto & plugins()		{ return m_plugins; }
		inline auto & prefs()		{ return m_prefs; }
		inline auto & time()		{ return m_time; }
		inline auto & window()		{ return m_window; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENGINE_HPP_