/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/EventSystem.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Engine/Engine.hpp>
#include <ML/Engine/PluginManager.hpp>

ml::int32_t main()
{
	using namespace ml;

	// Initialize Systems
	ML_EventSystem;
	ML_Engine;
	ML_Editor;
	ML_Plugins;

	// Startup Sequence
	ML_EventSystem.fireEvent<EnterEvent>();
	ML_EventSystem.fireEvent<LoadEvent>();
	ML_EventSystem.fireEvent<StartEvent>();

	// Main Loop
	while (ML_Engine.window.is_open())
	{
		ML_EventSystem.fireEvent<BeginLoopEvent>();
		ML_EventSystem.fireEvent<UpdateEvent>();
		ML_EventSystem.fireEvent<BeginDrawEvent>();
		ML_EventSystem.fireEvent<DrawEvent>();
		ML_EventSystem.fireEvent<EndDrawEvent>();
		ML_EventSystem.fireEvent<BeginGuiEvent>();
		ML_EventSystem.fireEvent<GuiEvent>();
		ML_EventSystem.fireEvent<EndGuiEvent>();
		ML_EventSystem.fireEvent<EndLoopEvent>();
	}

	// Shutdown Sequence
	ML_EventSystem.fireEvent<UnloadEvent>();
	ML_EventSystem.fireEvent<ExitEvent>();

	// Goodbye!
	return EXIT_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */