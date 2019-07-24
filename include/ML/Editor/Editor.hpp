#ifndef _ML_EDITOR_HPP_
#define _ML_EDITOR_HPP_

#include <ML/Engine/EngineEvents.hpp>
#include <ML/Editor/Terminal.hpp>
#include <ML/Editor/Browser.hpp>
#include <ML/Editor/Dockspace.hpp>
#include <ML/Editor/ResourceGui.hpp>
#include <ML/Editor/Profiler.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API Editor final
		: public I_Newable
		, public I_NonCopyable
		, public EventListener
	{
	public:
		explicit Editor(EventSystem & eventSystem);
		
		~Editor() {}

		void onEvent(const Event * value) override;

	private:
		void onEnter	(const EnterEvent & ev);
		void onBeginGui	(const BeginGuiEvent & ev);
		void onGui		(const GuiEvent & ev);
		void onEndGui	(const EndGuiEvent & ev);
		void onExit		(const ExitEvent & ev);

	public:
		inline auto dockspace() -> Dockspace &		{ return m_dockspace; }
		inline auto browser()	-> Browser &		{ return m_browser; }
		inline auto profiler()	-> Profiler &		{ return m_profiler; }
		inline auto resources() -> ResourceGui &	{ return m_resources; }
		inline auto terminal()	-> Terminal &		{ return m_terminal; }

	private:
		Dockspace	m_dockspace;
		Browser		m_browser;
		Profiler	m_profiler;
		ResourceGui	m_resources;
		Terminal	m_terminal;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_HPP_