/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Engine/Plugin.hpp>
#include <ML/Engine/Engine.hpp>
#include <ML/Engine/GameTime.hpp>
#include <ML/Core/Preferences.hpp>
#include <ML/Engine/Resources.hpp>
#include <ML/Engine/SharedLibrary.hpp>
#include <ML/Core/StateMachine.hpp>
#include <ML/Graphics/RenderWindow.hpp>

/* * * * * * * * * * * * * * * * * * * * */

# ifndef ML_CONFIG_INI
# define ML_CONFIG_INI "../../../ML.ini"
# endif

/* * * * * * * * * * * * * * * * * * * * */

using namespace ml;

/* * * * * * * * * * * * * * * * * * * * */

int32_t main()
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static Preferences	g_Preferences	{ ML_CONFIG_INI };
	static EventSystem	g_EventSystem	{};
	static Resources	g_Resources		{};
	static GameTime		g_Time			{};
	static RenderWindow g_Window		{ g_EventSystem };
	static Engine		g_Engine		{ g_EventSystem };
	static Editor		g_Editor		{ g_EventSystem };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum State { Enter, Loop, Exit };

	static StateMachine<State> g_ControlFlow =
	{
	{ State::Enter, []()
	{	/* Enter */
		g_EventSystem.fireEvent(EnterEvent(
			g_Preferences,
			g_Window
		));
		/* Load Content */
		g_EventSystem.fireEvent(LoadEvent(
			g_Preferences,
			g_Resources
		));
		/* Start */
		g_EventSystem.fireEvent(StartEvent(
			g_Time,
			g_Resources,
			g_Window
		));
		return g_ControlFlow(State::Loop);
	} },
	{ State::Loop, []()
	{	/* Loop */
		while (g_Window.isOpen())
		{
			/* Begin Frame */
			g_Time.beginLoop();
			g_Window.pollEvents();

			/* Update */
			g_EventSystem.fireEvent(UpdateEvent(
				g_Time,
				g_Resources,
				g_Window
			));

			/* Draw */
			g_EventSystem.fireEvent(DrawEvent(
				g_Time,
				g_Resources,
				g_Window
			));

			/* Gui */
			g_EventSystem.fireEvent(BeginGuiEvent());
			g_EventSystem.fireEvent(GuiEvent(
				g_Time,
				g_Resources,
				g_Editor
			));
			g_EventSystem.fireEvent(EndGuiEvent());

			/* End Frame */
			g_Window.swapBuffers();
			g_Time.endLoop();
		}
		return g_ControlFlow(State::Exit);
	} },
	{ State::Exit, []()
	{	/* Exit */
		g_EventSystem.fireEvent(ExitEvent(
			g_Resources,
			g_Window
		));
		return g_ControlFlow.NoState;
	} },
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Load Plugins
	static Map<SharedLibrary *, Plugin *> g_Plugins;
	if (auto file = std::ifstream(ML_FS.getPathTo(g_Preferences.GetString(
		"Engine",
		"plugin_list",
		"../../../assets/data/plugins.txt"
	))))
	{
		String line;
		while (std::getline(file, line))
		{
			if (!line || (line.trim().front() == '#')) 
				continue;
			
			auto library = new SharedLibrary(ML_FS.getPathTo(line
				.replaceAll("$(Configuration)", ML_CONFIGURATION)
				.replaceAll("$(PlatformTarget)", ML_PLATFORM_TARGET)
			));

			auto plugin = g_Plugins.insert({
				library,
				library->callFunction<Plugin *>(ML_str(ML_Plugin_Main), g_EventSystem)
			}).first->second;
		}
		file.close();
	}

	// Run Controller
	g_ControlFlow(State::Enter);

	// Cleanup Plugins
	for (auto & pair : g_Plugins)
	{
		if (pair.second) { delete pair.second; }
		if (pair.first)  { delete pair.first;  }
	}

	// Goodbye!
	return EXIT_SUCCESS;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * */