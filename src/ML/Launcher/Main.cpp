/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/EditorApplication.hpp>
#include <ML/Engine/Engine.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Engine/Plugin.hpp>
#include <ML/Engine/Resources.hpp>
#include <ML/Engine/StateMachine.hpp>
#include <ML/Script/Interpreter.hpp>

/* * * * * * * * * * * * * * * * * * * * */

# ifndef ML_CONFIG_INI
# define ML_CONFIG_INI "../../../ML.ini"
# endif

/* * * * * * * * * * * * * * * * * * * * */

int32_t main()
{
	// Setup Launcher
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static ml::Preferences	g_LauncherPrefs	= {};
	static ml::EventSystem	g_EventSystem	= {};
	static ml::Resources	g_Resources		= {};
	static ml::Engine		g_Engine		= { g_EventSystem, g_LauncherPrefs, g_Resources };
	static ml::Editor		g_Editor		= { g_Engine };

	/* * * * * * * * * * * * * * * * * * * * */
	
	if (!g_LauncherPrefs.loadFromFile(ML_CONFIG_INI))
	{
		return ml::Debug::logError("Failed Loading Preferences: \'{0}\'", ML_CONFIG_INI)
			|| ml::Debug::pause(EXIT_FAILURE);
	}


	// Setup Flow Control
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum State : int32_t
	{
		NO_STATE = ML_STATE_NONE,
		Enter, Load, Start, Loop, Unload, Exit,
		MAX_STATE
	};

	/* * * * * * * * * * * * * * * * * * * * */

	static ml::StateMachine<State> g_States =
	{
	{ State::Enter, []()
	{	/* On Enter */
		g_EventSystem.fireEvent(ml::EnterEvent(g_Engine));
		return g_States(State::Load);
	} },

	{ State::Load, []()
	{	/* On Load */
		g_EventSystem.fireEvent(ml::LoadEvent(g_Engine));
		return g_States(State::Start);
	} },

	{ State::Start, []()
	{	/* On Start */
		g_EventSystem.fireEvent(ml::StartEvent(g_Engine));
		return g_States(State::Loop);
	} },

	{ State::Loop, []()
	{	g_Engine.loopFun([&]()
		{
			/* On Update */ 
			g_EventSystem.fireEvent(ml::UpdateEvent(g_Engine));
			
			/* On Draw */
			g_EventSystem.fireEvent(ml::DrawEvent(g_Engine));
			
			/* On Gui */ 
			g_EventSystem.fireEvent(ml::GuiEvent(g_Editor));
		});
		return g_States(State::Unload);
	} },

	{ State::Unload, []()
	{	/* On Unload */
		g_EventSystem.fireEvent(ml::UnloadEvent(g_Engine));
		return g_States(State::Exit);
	} },

	{ State::Exit, []()
	{	/* On Exit */
		g_EventSystem.fireEvent(ml::ExitEvent(g_Engine));
		return g_States(ML_STATE_NONE);
	} },
	};


	// Launch Application
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	if (auto lib = ml::SharedLibrary(ML_FS.getPathTo(
		g_LauncherPrefs.GetString("Launcher", "user_dll", "") + ML_DLL_STR("")
	)))
	{
		if (auto app = lib.callFun<ml::Application *>(ML_str(ML_Plugin_Main), &g_EventSystem))
		{
			if (g_Engine.launchApp(app))
			{
				g_States(State::Enter);
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
			return ml::Debug::logError("Failed Calling Plugin Main")
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