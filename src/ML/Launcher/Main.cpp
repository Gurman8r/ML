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

	static ml::Preferences	g_Preferences	{ ML_CONFIG_INI };
	static ml::EventSystem	g_EventSystem	{};
	static ml::Resources	g_Resources		{};
	static ml::GameTime		g_Time			{};
	static ml::Audio		g_Audio			{ g_EventSystem }; // WIP
	static ml::NetClient	g_Client		{ g_EventSystem }; // WIP
	static ml::NetServer	g_Server		{ g_EventSystem }; // WIP
	static ml::RenderWindow g_Window		{ g_EventSystem };
	static ml::Engine		g_Engine		{ g_EventSystem, g_Preferences, g_Window };
	static ml::Editor		g_Editor		{ g_EventSystem };


	// Setup Control Flow
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
		g_EventSystem.fireEvent(ml::LoadContentEvent(
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
		return g_Control(g_Control.NoState);
	} },
	};


	// Launch Application
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Load Plugin from File
	if (auto lib = ml::Plugin(ML_FS.getPathTo(
		g_Preferences.GetString("Engine", "user_dll", "") + ML_DLL_STR("")
	)))
	{	// Load Application from Plugin
		if (auto app = lib.callFun<ml::Application *>(ML_str(ML_Plugin_Main), g_EventSystem))
		{	
			// Run Control
			g_Control(State::Enter);

			// Free Application
			delete app;

			// Goodbye!
			return EXIT_SUCCESS;
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