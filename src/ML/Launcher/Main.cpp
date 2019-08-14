/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/StateMachine.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Engine/Engine.hpp>
#include <ML/Engine/GameTime.hpp>
#include <ML/Engine/PluginLoader.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Graphics/RenderWindow.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace ml;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static Preferences 	g_Preferences	{ "../../../ML.ini" };
static GameTime		g_Time			{ };
static EventSystem	g_EventSystem	{ };
static RenderWindow g_Window		{ g_EventSystem };
static Engine		g_Engine		{ g_EventSystem };
static Editor		g_Editor		{ g_EventSystem };
static PluginLoader g_Plugins		{ g_EventSystem };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

enum class State { Exit = -1, Startup, MainLoop, Shutdown };

static StateMachine<State> g_ProgramStates
{
{ State::Startup, []()
{
	g_EventSystem.fireEvent(EnterEvent	{ g_Time, g_Preferences, g_Window });
	g_EventSystem.fireEvent(LoadEvent	{ g_Time, g_Preferences, g_Window });
	g_EventSystem.fireEvent(StartEvent	{ g_Time, g_Window });
	return g_ProgramStates(State::MainLoop);
} },
{ State::MainLoop, []()
{
	while (g_Window.isOpen())
	{
		g_EventSystem.fireEvent(BeginFrameEvent	{ g_Time, g_Window, g_Engine });
		g_EventSystem.fireEvent(UpdateEvent		{ g_Time, g_Window, g_Engine });
		g_EventSystem.fireEvent(BeginDrawEvent	{ g_Time, g_Window, g_Engine });
		g_EventSystem.fireEvent(DrawEvent		{ g_Time, g_Window, g_Engine });
		g_EventSystem.fireEvent(EndDrawEvent	{ g_Time, g_Window, g_Engine });
		g_EventSystem.fireEvent(BeginGuiEvent	{ g_Time, g_Window, g_Editor });
		g_EventSystem.fireEvent(GuiEvent		{ g_Time, g_Window, g_Editor });
		g_EventSystem.fireEvent(EndGuiEvent		{ g_Time, g_Window, g_Editor });
		g_EventSystem.fireEvent(EndFrameEvent	{ g_Time, g_Window, g_Engine });
	}
	return g_ProgramStates(State::Shutdown);
} },
{ State::Shutdown, []()
{
	g_EventSystem.fireEvent(UnloadEvent { g_Time, g_Window, g_Engine });
	g_EventSystem.fireEvent(ExitEvent	{ g_Time });
	return g_ProgramStates(State::Exit);
} },
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <ML/Graphics/Mesh.hpp>

int32_t test_assimp()
{
	if (Mesh * mesh { Mesh::loadFromAssimp(ML_FS.pathTo(
		"../../../assets/meshes/cow.obj"
	)) })
	{
		Debug::log("Filename:\t{0}",	mesh->filename());
		Debug::log("Directory:\t{0}",	mesh->directory());
		Debug::log("Vertices:\t{0}",	mesh->vertices().size());
		Debug::log("Indices:\t{0}",		mesh->indices().size());
		Debug::log("Contiguous:\t{0}",	mesh->contiguous().size());

		delete mesh;
	}
	else
	{
		Debug::logError("Failed loading mesh");
	}

	return Debug::pause(EXIT_SUCCESS);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	template <class T> static inline String to_string(const T value)
	{
		return std::to_string(value);
	}

	template <class T> static inline String to_string(const T & value)
	{
		try { return std::to_string(value); }
		catch (std::invalid_argument&)
		{
			SStream ss; 
			ss << value;
			return ss.str();
		}
	}
}

int32_t main()
{
	// Load Plugin List
	if (g_Plugins.loadFromFile(ML_FS.pathTo(g_Preferences.get_string(
		"Launcher", "plugin_list", ""
	))))
	{
		g_Plugins.loadLibraries();	// Load Libraries
		g_Plugins.loadPlugins();	// Load Plugins
	}

	// Run Controller
	g_ProgramStates(State::Startup);

	// Cleanup Plugins
	g_Plugins.dispose();

	// Goodbye!
	return EXIT_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */