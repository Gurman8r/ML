/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Audio/Audio.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Engine/Application.hpp>
#include <ML/Engine/Engine.hpp>
#include <ML/Engine/GameTime.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Engine/Resources.hpp>
#include <ML/Engine/Plugin.hpp>
#include <ML/Engine/StateMachine.hpp>
#include <ML/Graphics/RenderWindow.hpp>
#include <ML/Network/NetClient.hpp>
#include <ML/Network/NetServer.hpp>

/* * * * * * * * * * * * * * * * * * * * */

# ifndef ML_CONFIG_INI
# define ML_CONFIG_INI "../../../ML.ini"
# endif

/* * * * * * * * * * * * * * * * * * * * */

int32_t main()
{
	// Setup Systems
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static ml::Preferences	g_Prefs		{ ML_CONFIG_INI };
	static ml::EventSystem	g_EventSys	{};
	static ml::Resources	g_Res		{};
	static ml::GameTime		g_Time		{};
	static ml::Audio		g_Audio		{ g_EventSys }; // WIP
	static ml::NetClient	g_Client	{ g_EventSys }; // WIP
	static ml::NetServer	g_Server	{ g_EventSys }; // WIP
	static ml::RenderWindow g_Window	{ g_EventSys };
	static ml::Engine		g_Engine	{ g_EventSys, g_Prefs, g_Time, g_Res, g_Window };
	static ml::Editor		g_Editor	{ g_EventSys };


	// Setup Control Flow
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum State { Enter, Load, Start, Loop, Shutdown };

	/* * * * * * * * * * * * * * * * * * * * */

	static ml::StateMachine<State> g_Control =
	{
	{ State::Enter, []()
	{	/* Enter */
		g_EventSys.fireEvent(ml::EnterEvent(
			g_Prefs,
			g_Res,
			g_Window
		));
		return g_Control(State::Load);
	} },
	{ State::Load, []()
	{	/* Load */
		g_EventSys.fireEvent(ml::LoadEvent(
			g_Prefs,
			g_Res,
			g_Window
		));
		return g_Control(State::Start);
	} },
	{ State::Start, []()
	{	/* Start */
		g_EventSys.fireEvent(ml::StartEvent(
			g_Engine,
			g_Res,
			g_Window
		));
		return g_Control(State::Loop);
	} },
	{ State::Loop, []()
	{	/* Loop */
		while (g_Engine.isRunning())
		{
			/* Begin Frame */
			g_Engine.beginFrame();

			/* Update */
			g_EventSys.fireEvent(ml::UpdateEvent(
				g_Time,
				g_Res,
				g_Window
			));

			/* Draw */
			g_EventSys.fireEvent(ml::DrawEvent(
				g_Time,
				g_Res,
				g_Window
			));

			/* Gui */
			g_EventSys.fireEvent(ml::BeginGuiEvent());
			g_EventSys.fireEvent(ml::GuiEvent(
				g_Time,
				g_Res,
				g_Editor
			));
			g_EventSys.fireEvent(ml::EndGuiEvent());

			/* End Frame */
			g_Engine.endFrame();
		}
		return g_Control(State::Shutdown);
	} },
	{ State::Shutdown, []()
	{	/* Shutdown */
		g_EventSys.fireEvent(ml::ShutdownEvent(
			g_Res,
			g_Window
		));
		return g_Control(ML_STATE_NONE);
	} },
	};


	// Launch Application
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ml::Plugin plugin;

	// Load Library from File
	if (auto lib = ml::Plugin(ML_FS.getPathTo(
		g_Prefs.GetString("Engine", "user_dll", "") + ML_DLL_STR("")
	)))
	{	// Load Application from Library
		if (auto app = g_Engine.launchApp(
			lib.callFun<ml::Application *>(ML_str(ML_Plugin_Main), g_EventSys)
		))
		{	// Run Control
			g_Control(State::Enter);

			// Free Application
			return g_Engine.freeApp(app);
		}
		else
		{
			return ml::Debug::logError("Failed Launching Application")
				|| ml::Debug::pause(EXIT_FAILURE);
		}
	}
	else
	{
		return ml::Debug::logError("Failed Loading Library: \'{0}\'", lib.filename())
			|| ml::Debug::pause(EXIT_FAILURE);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * */