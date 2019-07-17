#ifndef _ML_EDITOR_PLUGIN_HPP_
#define _ML_EDITOR_PLUGIN_HPP_

#include <ML/Editor/Export.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Engine/Plugin.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_EDITOR_API EditorPlugin
		: public Plugin
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		EditorPlugin(EventSystem & eventSystem);
		
		virtual ~EditorPlugin() {}

		virtual void onEvent(const Event * value) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void onStart	(const StartEvent	& ev) = 0;
		virtual void onUpdate	(const UpdateEvent	& ev) = 0;
		virtual void onDraw		(const DrawEvent	& ev) = 0;
		virtual void onGui		(const GuiEvent		& ev) = 0;
		virtual void onExit		(const ExitEvent	& ev) = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_PLUGIN_HPP_