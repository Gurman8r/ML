#ifndef _ML_APPLICATION_HPP_
#define _ML_APPLICATION_HPP_

#include <ML/Engine/EngineEvents.hpp>
#include <ML/Core/IEventListener.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class EventSystem;

	/* * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API Application
		: public ITrackable
		, public INonCopyable
		, public IEventListener
	{
	public:
		explicit Application(EventSystem & eventSystem);
		virtual ~Application();

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

#endif // !_ML_APPLICATION_HPP_