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
		void onBeginFrame	(const BeginFrameEvent & ev);
		void onUpdate		(const UpdateEvent & ev);
		void onBeginDraw	(const BeginDrawEvent & ev);
		void onDraw			(const DrawEvent & ev);
		void onEndDraw		(const EndDrawEvent & ev);
		void onEndFrame		(const EndFrameEvent & ev);
		void onUnload		(const UnloadEvent & ev);
		void onExit			(const ExitEvent & ev);

	private:
		Ref<Image>	m_icon			{ "icon" };
		vec2			m_cursorPos		{ uninit };
		float_t			m_deltaTime		{ uninit };
		int32_t			m_frameCount	{ uninit };
		float_t			m_frameRate		{ uninit };
		vec2			m_viewport		{ uninit };
		float_t			m_totalTime		{ uninit };

	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENGINE_HPP_