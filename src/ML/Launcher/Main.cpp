/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Audio/Audio.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Engine/Application.hpp>
#include <ML/Engine/Engine.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Engine/Resources.hpp>
#include <ML/Engine/SharedLibrary.hpp>
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

	static ml::Preferences	g_Prefs			{ ML_CONFIG_INI };
	static ml::EventSystem	g_EventSystem	{};
	static ml::Resources	g_Resources		{};
	static ml::Audio		g_Audio			{ g_EventSystem };
	static ml::NetClient	g_Client		{ g_EventSystem };
	static ml::NetServer	g_Server		{ g_EventSystem };
	static ml::RenderWindow g_Window		{ g_EventSystem };
	static ml::Engine		g_Engine		{ g_EventSystem, g_Prefs, g_Resources, g_Window, g_Client, g_Server, g_Audio };
	static ml::Editor		g_Editor		{ g_Engine };


	// Setup Flow Controller
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum State { Enter, Load, Start, Loop, Unload };

	/* * * * * * * * * * * * * * * * * * * * */

	static ml::StateMachine<State> g_Control =
	{
	{ State::Enter, []()
	{	/* Enter */
		g_EventSystem.fireEvent(ml::EnterEvent(g_Engine));
		return g_Control(State::Load);
	} },
	{ State::Load, []()
	{	/* Load */
		g_EventSystem.fireEvent(ml::LoadEvent(g_Engine));
		return g_Control(State::Start);
	} },
	{ State::Start, []()
	{	/* Start */
		g_EventSystem.fireEvent(ml::StartEvent(g_Engine));
		return g_Control(State::Loop);
	} },
	{ State::Loop, []()
	{	g_Engine.loopFun([&]()
		{
			/* Update */ 
			g_EventSystem.fireEvent(ml::UpdateEvent(g_Engine));

			/* Draw */
			g_EventSystem.fireEvent(ml::DrawEvent(g_Engine));

			/* Gui */
			g_EventSystem.fireEvent(ml::BeginGuiEvent(g_Editor));
			g_EventSystem.fireEvent(ml::DrawGuiEvent(g_Editor));
			g_EventSystem.fireEvent(ml::EndGuiEvent(g_Editor));
			
		});
		return g_Control(State::Unload);
	} },
	{ State::Unload, []()
	{	/* Unload */
		g_EventSystem.fireEvent(ml::UnloadEvent(g_Engine));
		return g_Control(ML_STATE_NONE);
	} },
	};


	// Launch Application
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Load Library from File
	if (auto lib = ml::SharedLibrary(ML_FS.getPathTo(
		g_Prefs.GetString("Engine", "user_dll", "") + ML_DLL_STR("")
	)))
	{	// Load Application from Library
		if (auto app = g_Engine.launchApp(
			lib.callFun<ml::Application *>(ML_str(ML_Plugin_Main), g_EventSystem)
		))
		{	// Run State Machine
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