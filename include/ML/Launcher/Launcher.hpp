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

#define ML_Launcher ::ml::Launcher::getInstance()

namespace ml
{
	class Launcher final : public I_Singleton<Launcher>
	{
		friend struct I_Singleton<Launcher>;

		Launcher();

		~Launcher() {}

		bool m_running { false };

	public:
		Preferences 	prefs;
		GameTime		time;
		EventSystem		eventSystem;
		RenderWindow	window;
		Engine			engine;
		Editor			editor;
		PluginLoader	plugins;

		int32_t run();
	};
}

#endif // !_ML_LAUNCHER_HPP_