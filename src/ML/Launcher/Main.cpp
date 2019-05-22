/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Engine/Plugin.hpp>
#include <ML/Engine/Engine.hpp>
#include <ML/Engine/GameTime.hpp>
#include <ML/Core/Preferences.hpp>
#include <ML/Engine/Resources.hpp>
#include <ML/Core/SharedLibrary.hpp>
#include <ML/Core/StateMachine.hpp>
#include <ML/Graphics/RenderWindow.hpp>

/* * * * * * * * * * * * * * * * * * * * */

# ifndef ML_CONFIG_INI
# define ML_CONFIG_INI "../../../ML.ini"
# endif

/* * * * * * * * * * * * * * * * * * * * */

int32_t main()
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static ml::Preferences	g_Preferences	{ ML_CONFIG_INI };
	static ml::EventSystem	g_EventSystem	{};
	static ml::Resources	g_Resources		{};
	static ml::GameTime		g_Time			{};
	static ml::RenderWindow g_Window		{ g_EventSystem };
	static ml::Engine		g_Engine		{ g_EventSystem };
	static ml::Editor		g_Editor		{ g_EventSystem };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum State { Enter, Loop, Exit };

	static ml::StateMachine<State> g_Control =
	{
	{ State::Enter, []()
	{	/* Enter */
		g_EventSystem.fireEvent(ml::EnterEvent(
			g_Preferences,
			g_Window
		));
		/* Load Content */
		g_EventSystem.fireEvent(ml::LoadEvent(
			g_Preferences,
			g_Resources
		));
		/* Start */
		g_EventSystem.fireEvent(ml::StartEvent(
			g_Time,
			g_Resources,
			g_Window
		));
		return g_Control(State::Loop);
	} },
	{ State::Loop, []()
	{	/* Loop */
		while (g_Window.isOpen())
		{
			/* Begin Frame */
			g_Time.beginLoop();
			g_Window.pollEvents();

			/* Update */
			g_EventSystem.fireEvent(ml::UpdateEvent(
				g_Time,
				g_Resources,
				g_Window
			));

			/* Draw */
			g_EventSystem.fireEvent(ml::DrawEvent(
				g_Time,
				g_Resources,
				g_Window
			));

			/* Gui */
			g_EventSystem.fireEvent(ml::BeginGuiEvent());
			g_EventSystem.fireEvent(ml::GuiEvent(
				g_Time,
				g_Resources,
				g_Editor
			));
			g_EventSystem.fireEvent(ml::EndGuiEvent());

			/* End Frame */
			g_Window.swapBuffers();
			g_Time.endLoop();
		}
		return g_Control(State::Exit);
	} },
	{ State::Exit, []()
	{	/* Exit */
		g_EventSystem.fireEvent(ml::ExitEvent(
			g_Resources,
			g_Window
		));
		return g_Control.NoState;
	} },
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Load Plugins
	static ml::Map<ml::SharedLibrary *, ml::Plugin *> g_Plugins;
	if (auto file = std::ifstream(ML_FS.getPathTo(g_Preferences.GetString(
		"Engine",
		"plugin_list",
		"../../../assets/data/plugins.txt"
	))))
	{
		ml::String line;
		while (std::getline(file, line))
		{
			if (line && (line.trim().front() != '#'))
			{
				auto lib = new ml::SharedLibrary(ML_FS.getPathTo(line
					.replaceAll("$(Configuration)", ML_CONFIGURATION)
					.replaceAll("$(PlatformTarget)", ML_PLATFORM_TARGET)
				));
				g_Plugins.insert({ 
					lib, 
					lib->callFun<ml::Plugin *>("ML_Plugin_Main", g_EventSystem) 
				});
			}
		}
		file.close();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Run Controller
	g_Control(State::Enter);

	// Cleanup
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