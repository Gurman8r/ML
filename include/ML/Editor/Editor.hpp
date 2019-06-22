#ifndef _ML_EDITOR_HPP_
#define _ML_EDITOR_HPP_

#include <ML/Engine/EngineEvents.hpp>
#include <ML/Editor/TerminalGui.hpp>
#include <ML/Editor/BrowserGui.hpp>
#include <ML/Editor/DockspaceGui.hpp>
#include <ML/Editor/ResourceGui.hpp>
#include <ML/Editor/ProfilerGui.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API Editor final
		: public I_Newable
		, public EventListener
		, public I_NonCopyable
	{
	public:
		explicit Editor(EventSystem & eventSystem);
		~Editor();

	public:
		void onEvent(const Event * value) override;

	private:
		void onEnter	(const EnterEvent & ev);
		void onBeginGui	(const BeginGuiEvent & ev);
		void onGui		(const GuiEvent & ev);
		void onEndGui	(const EndGuiEvent & ev);
		void onExit		(const ExitEvent & ev);

	private:
		DockspaceGui	m_dockspace;
		BrowserGui		m_browser;
		ProfilerGui		m_profiler;
		ResourceGui		m_resources;
		TerminalGui		m_terminal;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_HPP_