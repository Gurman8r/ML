#ifndef _ML_EDITOR_PLUGIN_HPP_
#define _ML_EDITOR_PLUGIN_HPP_

#include <ML/Editor/Export.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Engine/Plugin.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_EDITOR_API EditorPlugin : public Plugin
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit EditorPlugin(EventSystem & eventSystem);
		
		virtual ~EditorPlugin() {}

		virtual void onEvent(const Event & value) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void onStart	(const StartEvent	& ev) {};
		virtual void onUpdate	(const UpdateEvent	& ev) {};
		virtual void onDraw		(const DrawEvent	& ev) {};
		virtual void onGui		(const GuiEvent		& ev) {};
		virtual void onExit		(const ExitEvent	& ev) {};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_PLUGIN_HPP_