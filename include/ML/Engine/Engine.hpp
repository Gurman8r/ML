#ifndef _ML_ENGINE_HPP_
#define _ML_ENGINE_HPP_

#include <ML/Engine/EngineEvents.hpp>
#include <ML/Engine/ContentManager.hpp>
#include <ML/Engine/GameTime.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Engine/CommandRegistry.hpp>
#include <ML/Engine/PluginManager.hpp>
#include <ML/Graphics/RenderWindow.hpp>

#ifndef ML_INI_FILENAME
#define ML_INI_FILENAME "ML.ini"
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

		CommandRegistry	* m_commands;
		ContentManager	* m_content;
		PluginManager	* m_plugins;
		Preferences		* m_prefs;
		GameTime		* m_time;
		RenderWindow	* m_window;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:
		inline auto & commands()	{ return *m_commands; }
		inline auto & content()		{ return *m_content; }
		inline auto & plugins()		{ return *m_plugins; }
		inline auto & prefs()		{ return *m_prefs; }
		inline auto & time()		{ return *m_time; }
		inline auto & window()		{ return *m_window; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENGINE_HPP_