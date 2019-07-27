#ifndef _ML_ENGINE_HPP_
#define _ML_ENGINE_HPP_

#include <ML/Engine/EngineEvents.hpp>
#include <ML/Engine/Asset.hpp>
#include <ML/Core/EventListener.hpp>
#include <ML/Core/Matrix.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct Image;
	struct Script;

	/* * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API Engine final
		: public I_Newable
		, public I_NonCopyable
		, public EventListener
	{
	public:
		explicit Engine(EventSystem & eventSystem);
		
		~Engine() {}

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
		Asset<Image>	m_icon		{ "icon" };
		Asset<Script>	m_script	{};

		vec2	m_cursorPos		{ 0 };
		float_t	m_deltaTime		{ 0 };
		int32_t m_frameCount	{ 0 };
		float_t m_frameRate		{ 0 };
		vec2	m_viewport		{ 0 };
		float_t	m_totalTime		{ 0 };

	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENGINE_HPP_