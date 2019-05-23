#ifndef _TEST_PLUGIN_HPP_
#define _TEST_PLUGIN_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Editor/EditorPlugin.hpp>

/* * * * * * * * * * * * * * * * * * * * */

# if defined(ML_SYSTEM_WINDOWS)
#	pragma comment(lib, ML_LIB_STR("Audio")		)
#	pragma comment(lib, ML_LIB_STR("Core")		)
#	pragma comment(lib, ML_LIB_STR("Editor")	)
#	pragma comment(lib, ML_LIB_STR("Engine")	)
#	pragma comment(lib, ML_LIB_STR("Graphics")	)
#	pragma comment(lib, ML_LIB_STR("Network")	)
#	pragma comment(lib, ML_LIB_STR("Script")	)
#	pragma comment(lib, ML_LIB_STR("Window")	)
# endif

/* * * * * * * * * * * * * * * * * * * * */

extern "C"
{
	ML_PLUGIN_API ml::Plugin * ML_Plugin_Main(ml::EventSystem & eventSystem);
}

/* * * * * * * * * * * * * * * * * * * * */

namespace DEMO
{
	class ML_PLUGIN_API TestPlugin final : public ml::EditorPlugin
	{
	public:
		explicit TestPlugin(ml::EventSystem & eventSystem);
		~TestPlugin();

	public:
		void onEvent(const ml::Event * value) override;

	private:
		void onEnter	(const ml::EnterEvent	& ev) override;
		void onStart	(const ml::StartEvent	& ev) override;
		void onUpdate	(const ml::UpdateEvent	& ev) override;
		void onDraw		(const ml::DrawEvent	& ev) override;
		void onGui		(const ml::GuiEvent		& ev) override;
		void onExit		(const ml::ExitEvent	& ev) override;
	};
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_TEST_PLUGIN_HPP_