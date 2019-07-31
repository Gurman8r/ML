#ifndef _ML_PLUGIN_HPP_
#define _ML_PLUGIN_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Engine/EngineEvents.hpp>
#include <ML/Core/I_EventListener.hpp>
#include <ML/Core/I_Newable.hpp>

#define ML_PLUGIN_API	ML_API_EXPORT
#define ML_Plugin_Main	ML_Plugin_Main

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct EventSystem;

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API Plugin
		: public I_Newable
		, public I_NonCopyable
		, public I_EventListener
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit Plugin(EventSystem & eventSystem);
		
		virtual ~Plugin() {}

		virtual void onEvent(const Event & value) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void onStart	(const StartEvent	& ev) = 0;
		virtual void onUpdate	(const UpdateEvent	& ev) = 0;
		virtual void onDraw		(const DrawEvent	& ev) = 0;
		virtual void onExit		(const ExitEvent	& ev) = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PLUGIN_HPP_