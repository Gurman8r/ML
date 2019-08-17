#ifndef _ML_LAUNCHER_HPP_
#define _ML_LAUNCHER_HPP_

#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/StateMachine.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Engine/Engine.hpp>
#include <ML/Engine/GameTime.hpp>
#include <ML/Engine/PluginLoader.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Graphics/RenderWindow.hpp>

#ifndef ML_CONFIG_INI
#define ML_CONFIG_INI "../../../ML.ini"
#endif

#define ML_Launcher _ML Launcher::getInstance()

namespace ml
{
	class Launcher final : public I_Singleton<Launcher>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend struct I_Singleton<Launcher>;

		Launcher()
			: running		{ false }
			, prefs			{ ML_CONFIG_INI }
			, time			{ }
			, eventSystem	{ }
			, window		{ eventSystem }
			, engine		{ eventSystem }
			, editor		{ eventSystem }
			, plugins		{ eventSystem }
		{
		}

		~Launcher() {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:
		bool			running;		// 
		Preferences 	prefs;			// 
		GameTime		time;			// 
		EventSystem		eventSystem;	// 
		RenderWindow	window;			// 
		Engine			engine;			// 
		Editor			editor;			// 
		PluginLoader	plugins;		// 

		int32_t operator()(int32_t argc, char ** argv);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_LAUNCHER_HPP_