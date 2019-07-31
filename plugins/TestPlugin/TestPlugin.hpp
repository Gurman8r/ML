#ifndef _TEST_PLUGIN_HPP_
#define _TEST_PLUGIN_HPP_

#include <ML/Editor/EditorPlugin.hpp>

extern "C"
{
	ML_PLUGIN_API ml::Plugin * ML_Plugin_Main(ml::EventSystem & eventSystem);
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_PLUGIN_API TestPlugin final : public EditorPlugin
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit TestPlugin(EventSystem & eventSystem);
		
		~TestPlugin() {}

		void onEvent(const Event & value) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		void onStart	(const StartEvent	& ev) override;
		void onUpdate	(const UpdateEvent	& ev) override;
		void onDraw		(const DrawEvent	& ev) override;
		void onGui		(const GuiEvent		& ev) override;
		void onExit		(const ExitEvent	& ev) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
	
	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_TEST_PLUGIN_HPP_