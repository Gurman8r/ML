#ifndef _ML_ENGINE_HPP_
#define _ML_ENGINE_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Engine/EngineEvents.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/EventListener.hpp>
#include <ML/Core/String.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API Engine final
		: public I_Newable
		, public I_NonCopyable
		, public EventListener
	{
	public:
		explicit Engine(EventSystem & eventSystem);
		~Engine();

	public:
		void onEvent(const Event * value) override;

	private:
		void onEnter		(const EnterEvent & ev);
		void onLoad			(const LoadEvent & ev);
		void onStart		(const StartEvent & ev);
		void onBeginFrame	(const BeginFrameEvent & ev);
		void onUpdate		(const UpdateEvent & ev);
		void onBeginDraw	(const BeginDrawEvent & ev);
		void onDraw			(const DrawEvent & ev);
		void onEndDraw		(const EndDrawEvent & ev);
		void onEndFrame		(const EndFrameEvent & ev);
		void onUnload		(const UnloadEvent & ev);
		void onExit			(const ExitEvent & ev);

	private:
		String m_bootScript;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENGINE_HPP_