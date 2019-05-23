/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/StateMachine.hpp>
#include <ML/Core/TypeID.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Engine/Plugin.hpp>
#include <ML/Engine/Engine.hpp>
#include <ML/Engine/GameTime.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Engine/Resources.hpp>
#include <ML/Engine/SharedLibrary.hpp>
#include <ML/Graphics/RenderWindow.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

# ifndef ML_CONFIG_INI
# define ML_CONFIG_INI "../../../ML.ini"
# endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace ml;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static Preferences	g_Preferences	{ ML_CONFIG_INI };
static EventSystem	g_EventSystem	{};
static Resources	g_Resources		{};
static GameTime		g_Time			{};
static RenderWindow g_Window		{ g_EventSystem };
static Engine		g_Engine		{ g_EventSystem };
static Editor		g_Editor		{ g_EventSystem };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

enum State { Startup, Loop, Shutdown };

static StateMachine<State> g_ControlFlow
{
{ State::Startup, []()
{	/* Enter */
	/* * * * * * * * * * * * * * * * * * * * */
	g_EventSystem.fireEvent(EnterEvent(
		g_Preferences,
		g_Window
	));

	/* Load */
	/* * * * * * * * * * * * * * * * * * * * */
	g_EventSystem.fireEvent(LoadEvent(
		g_Preferences,
		g_Resources
	));

	/* Start */
	/* * * * * * * * * * * * * * * * * * * * */
	g_EventSystem.fireEvent(StartEvent(
		g_Time,
		g_Resources,
		g_Window
	));
	return g_ControlFlow(State::Loop);
} },
{ State::Loop, []()
{	while (g_Window.isOpen())
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
			g_Resources,
			g_Window
		));

		/* Draw */
		/* * * * * * * * * * * * * * * * * * * * */
		g_EventSystem.fireEvent(BeginDrawEvent());
		g_EventSystem.fireEvent(DrawEvent(
			g_Time,
			g_Resources,
			g_Window
		));
		g_EventSystem.fireEvent(EndDrawEvent());

		/* Gui */
		/* * * * * * * * * * * * * * * * * * * * */
		g_EventSystem.fireEvent(BeginGuiEvent());
		g_EventSystem.fireEvent(GuiEvent(
			g_Time,
			g_Resources,
			g_Editor
		));
		g_EventSystem.fireEvent(EndGuiEvent());

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
{	/* Unload */
	/* * * * * * * * * * * * * * * * * * * * */
	g_EventSystem.fireEvent(UnloadEvent(
		g_Resources,
		g_Window
	));

	/* Exit */
	/* * * * * * * * * * * * * * * * * * * * */
	g_EventSystem.fireEvent(ExitEvent());
	return g_ControlFlow.NoState;
} },
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32_t main()
{
	Debug::log("{0}", nameof<XString>());
	Debug::log("{0}", nameof<int>());
	Debug::log("{0}", nameof<float>());
	Debug::log("{0}", nameof<Transform>());
	Debug::log("{0}", nameof<std::string>());
	Debug::log("{0}", nameof<String>());
	Debug::log("{0}", nameof<int(*)(int, char**)>());

	// Setup
	Map<SharedLibrary *, Plugin *> plugins;
	if (auto file = std::ifstream(ML_FS.getPathTo(g_Preferences.GetString(
		"Launcher",
		"plugin_list",
		"../../../assets/data/plugins.txt"
	))))
	{	
		String line;
		while (std::getline(file, line))
		{
			if (line.empty() || (line.trim().front() == '#'))
				continue;
			
			// Load Library
			auto library = new SharedLibrary(ML_FS.getPathTo(line
				.replaceAll("$(Configuration)", ML_CONFIGURATION)
				.replaceAll("$(PlatformTarget)", ML_PLATFORM_TARGET)
			));

			// Load Plugin
			auto plugin = plugins.insert({
				library,
				library->callFunction<Plugin *>(ML_str(ML_Plugin_Main), g_EventSystem)
			}).first->second;
		}
		file.close();
	}

	// Run
	g_ControlFlow(State::Startup);

	// Cleanup
	for (auto & pair : plugins)
	{
		if (pair.second) { delete pair.second; }
		if (pair.first)  { delete pair.first;  }
	}

	// Goodbye!
	return EXIT_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */