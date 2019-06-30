#ifndef _ML_ENGINE_HPP_
#define _ML_ENGINE_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Engine/EngineEvents.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/EventListener.hpp>
#include <ML/Core/String.hpp>
#include <ML/Core/Matrix.hpp>

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

	public:
		inline auto cursorPos()		const -> const vec2 &		{ return m_cursorPos; }
		inline auto deltaTime()		const -> const float_t &	{ return m_deltaTime; }
		inline auto frameCount()	const -> const int32_t &	{ return m_frameCount; }
		inline auto resolution()	const -> const vec2 &		{ return m_resolution; }
		inline auto totalTime()		const -> const float_t &	{ return m_totalTime; }

	private:
		String	m_bootScript;

		vec2	m_cursorPos		= {};
		float_t	m_deltaTime		= 0;
		int32_t m_frameCount	= 0;
		vec2	m_resolution	= {};
		float_t	m_totalTime		= 0;

	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENGINE_HPP_