/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Engine/SharedLibrary.hpp>
#include <ML/Core/StateMachine.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Engine/Engine.hpp>
#include <ML/Engine/GameTime.hpp>
#include <ML/Engine/Plugin.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Graphics/RenderWindow.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

# ifndef ML_CONFIG_INI
# define ML_CONFIG_INI "../../../ML.ini"
# endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace ml;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static Preferences 	g_Preferences	{ ML_CONFIG_INI };
static PluginMap	g_Plugins		{ };
static GameTime		g_Time			{ };
static EventSystem	g_EventSystem	{ };
static RenderWindow g_Window		{ g_EventSystem };
static Engine		g_Engine		{ g_EventSystem };
static Editor		g_Editor		{ g_EventSystem };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

enum class State { Exit = -1, Startup, MainLoop, Shutdown };

static StateMachine<State> g_ControlFlow
{
{ State::Startup, []()
{	
	/* Enter */
	/* * * * * * * * * * * * * * * * * * * * */
	g_EventSystem.fireEvent(EnterEvent(
		g_Time,
		g_Preferences,
		g_Window
	));

	/* Load */
	/* * * * * * * * * * * * * * * * * * * * */
	g_EventSystem.fireEvent(LoadEvent(
		g_Time,
		g_Preferences
	));

	/* Start */
	/* * * * * * * * * * * * * * * * * * * * */
	g_EventSystem.fireEvent(StartEvent(
		g_Time,
		g_Window
	));
	return g_ControlFlow(State::MainLoop);
} },
{ State::MainLoop, []()
{	
	while (g_Window.isOpen())
	{
		/* Begin Frame */
		/* * * * * * * * * * * * * * * * * * * * */
		g_EventSystem.fireEvent(BeginFrameEvent(
			g_Time, 
			g_Window
		));

		/* Update */
		/* * * * * * * * * * * * * * * * * * * * */
		g_EventSystem.fireEvent(UpdateEvent(
			g_Time,
			g_Window
		));

		/* Draw */
		/* * * * * * * * * * * * * * * * * * * * */
		g_EventSystem.fireEvent(BeginDrawEvent(
			g_Time,
			g_Window
		));
		g_EventSystem.fireEvent(DrawEvent(
			g_Time,
			g_Window
		));
		g_EventSystem.fireEvent(EndDrawEvent(
			g_Time,
			g_Window
		));

		/* Gui */
		/* * * * * * * * * * * * * * * * * * * * */
		g_EventSystem.fireEvent(BeginGuiEvent(
			g_Time
		));
		g_EventSystem.fireEvent(GuiEvent(
			g_Time,
			g_Editor
		));
		g_EventSystem.fireEvent(EndGuiEvent(
			g_Time
		));

		/* End Frame */
		/* * * * * * * * * * * * * * * * * * * * */
		g_EventSystem.fireEvent(EndFrameEvent(
			g_Time, 
			g_Window
		));
	}
	return g_ControlFlow(State::Shutdown);
} },
{ State::Shutdown, []()
{	
	/* Unload */
	/* * * * * * * * * * * * * * * * * * * * */
	g_EventSystem.fireEvent(UnloadEvent(
		g_Time,
		g_Window
	));

	/* Exit */
	/* * * * * * * * * * * * * * * * * * * * */
	g_EventSystem.fireEvent(ExitEvent(
		g_Time
	));
	return g_ControlFlow(State::Exit);
} },
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32_t main()
{
	// Load Plugins
	if (Ifstream file { ML_FS.pathTo(g_Preferences.GetString(
		"Launcher",
		"plugin_list",
		"../../../assets/plugins.txt"
	)) })
	{	
		String line;
		while (std::getline(file, line))
		{
			if (line.empty() || (line.trim().front() == '#'))
				continue;
			
			// Load Library
			auto library = new SharedLibrary(ML_FS.pathTo(line
				.replaceAll("$(Configuration)", ML_CONFIGURATION)
				.replaceAll("$(PlatformTarget)", ML_PLATFORM_TARGET)
			));

			// Load Plugin
			if (auto plugin = g_Plugins.insert({
				library,
				library->callFunction<Plugin *>(ML_str(ML_Plugin_Main), g_EventSystem)
				}).first->second)
			{
				Debug::log("Loaded Plugin: \'{0}\'", line);
			}
		}
		file.close();
	}

	// Run Controller
	g_ControlFlow(State::Startup);

	// Cleanup Plugins
	for (auto & pair : g_Plugins)
	{
		if (pair.second) { delete pair.second; }
		if (pair.first)  { delete pair.first;  }
	}

	// Goodbye!
	return EXIT_SUCCESS;

	/* * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */