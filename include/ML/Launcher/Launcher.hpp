#ifndef _ML_LAUNCHER_HPP_
#define _ML_LAUNCHER_HPP_

#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Engine/GameTime.hpp>
#include <ML/Engine/PluginManager.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Graphics/RenderWindow.hpp>

#ifndef ML_CONFIG_INI
#define ML_CONFIG_INI "ML.ini"
#endif

#define ML_Launcher ::ml::Launcher::getInstance()

namespace ml
{
	struct Launcher final : public I_NonNewable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Launcher()
			: prefs { ML_CONFIG_INI }
			, time { }
			, window { }
		{
		}

		~Launcher() {}

		Preferences 	prefs;
		GameTime		time;
		RenderWindow	window;

		int32_t operator()(int32_t argc, char ** argv);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_LAUNCHER_HPP_