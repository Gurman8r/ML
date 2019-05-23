#ifndef _ML_PLUGIN_HPP_
#define _ML_PLUGIN_HPP_

#include <ML/Engine/PluginAPI.hpp>
#include <ML/Engine/EngineEvents.hpp>
#include <ML/Core/EventListener.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class EventSystem;

	/* * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API Plugin
		: public ITrackable
		, public INonCopyable
		, public EventListener
	{
	public:
		explicit Plugin(EventSystem & eventSystem);
		virtual ~Plugin();

	public:
		virtual void onEvent(const Event * value) override;

	public:
		virtual void onEnter	(const EnterEvent	& ev) = 0;
		virtual void onStart	(const StartEvent	& ev) = 0;
		virtual void onUpdate	(const UpdateEvent	& ev) = 0;
		virtual void onDraw		(const DrawEvent	& ev) = 0;
		virtual void onExit		(const ExitEvent	& ev) = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PLUGIN_HPP_