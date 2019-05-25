/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Engine/Plugin.hpp>
#include <ML/Engine/Engine.hpp>
#include <ML/Engine/GameTime.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Engine/Resources.hpp>
#include <ML/Engine/SharedLibrary.hpp>
#include <ML/Engine/StateMachine.hpp>
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

#include <ML/Core/Array.hpp>
#include <ML/Core/XString.hpp>

int32_t main()
{
	constexpr matrix_4<int32_t> m0({
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0
	});

	constexpr matrix_4<int32_t> m1({
		1, 1, 1, 1,
		1, 1, 1, 1,
		1, 1, 1, 1,
		1, 1, 1, 1
	});

	constexpr matrix_4<int32_t> ma({
		0x0, 0x1, 0x2, 0x3,
		0x4, 0x5, 0x6, 0x7,
		0x8, 0x9, 0xA, 0xB,
		0xC, 0xD, 0xE, 0xF
	});

	constexpr matrix_4<int32_t> mb({
		0x0, 0x1, 0x2, 0x3,
		0x4, 0x5, 0x6, 0x7,
		0x8, 0x9, 0xA, 0xB,
		0xC, 0xD, 0xE, 0xF
	});

	constexpr matrix_4<int32_t> mi = matrix_4<int32_t>::identity();

	constexpr matrix_4<int32_t> mc = ma + mb;

	constexpr vector_t<float, 4> va({ 1, 2, 3, 4 });
	constexpr vector_t<float, 4> vb({ });
	constexpr vector_t<float, 4> vc({ 0 });
	constexpr vector_t<float, 4> vd({ 1, 2, 3, 4 });

	static_assert(0 == 0, "?");
	static_assert(0 != 1, "?");

	constexpr vector_2f f1 { { 1, 2 } };
	constexpr vector_2f f2 { { 3, 4 } };
	constexpr vector_2f f3 { f1 + f2 };
	constexpr vector_2f f4 { vector_2f { { 10, 20 } } * 0.5f };
	

	Debug::log("{0}", mi);
	Debug::log("{0}", f2);
	Debug::log("{0}", f3);
	Debug::log("{0}", f4);

	return Debug::pause(0);

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

	/* * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */