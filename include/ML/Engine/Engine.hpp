#ifndef _ML_ENGINE_HPP_
#define _ML_ENGINE_HPP_

#include <ML/Engine/EngineEvents.hpp>
#include <ML/Engine/Ref.hpp>
#include <ML/Core/I_EventListener.hpp>
#include <ML/Core/Matrix.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct Image;

	/* * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API Engine final
		: public I_Newable
		, public I_NonCopyable
		, public I_EventListener
	{
	public:
		explicit Engine(EventSystem & eventSystem);
		
		~Engine() {}

		void onEvent(const Event & value) override;

	private:
		void onEnter		(const EnterEvent & ev);
		void onLoad			(const LoadEvent & ev);
		void onStart		(const StartEvent & ev);
		void onBeginFrame	(const BeginLoopEvent & ev);
		void onUpdate		(const UpdateEvent & ev);
		void onBeginDraw	(const BeginDrawEvent & ev);
		void onDraw			(const DrawEvent & ev);
		void onEndDraw		(const EndDrawEvent & ev);
		void onEndFrame		(const EndLoopEvent & ev);
		void onUnload		(const UnloadEvent & ev);
		void onExit			(const ExitEvent & ev);

	private:
		vec2	m_cursorPos		{ NULL };
		float_t	m_deltaTime		{ NULL };
		int32_t	m_frameCount	{ NULL };
		float_t	m_frameRate		{ NULL };
		vec2	m_viewport		{ NULL };
		float_t	m_totalTime		{ NULL };
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENGINE_HPP_