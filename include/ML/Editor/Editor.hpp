#ifndef _ML_EDITOR_HPP_
#define _ML_EDITOR_HPP_

#include <ML/Core/I_EventListener.hpp>
#include <ML/Editor/EditorAbout.hpp>
#include <ML/Editor/EditorContent.hpp>
#include <ML/Editor/EditorDockspace.hpp>
#include <ML/Editor/EditorExplorer.hpp>
#include <ML/Editor/EditorInspector.hpp>
#include <ML/Editor/EditorManual.hpp>
#include <ML/Editor/EditorProfiler.hpp>
#include <ML/Editor/EditorTerminal.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API Editor final
		: public I_Newable
		, public I_NonCopyable
		, public I_EventListener
	{
	public:
		explicit Editor(EventSystem & eventSystem);
		
		~Editor() {}

		void onEvent(const Event & value) override;

	private:
		void onEnter	(const EnterEvent & ev);
		void onUpdate	(const UpdateEvent & ev);
		void onBeginGui	(const BeginGuiEvent & ev);
		void onGui		(const GuiEvent & ev);
		void onEndGui	(const EndGuiEvent & ev);
		void onUnload	(const UnloadEvent & ev);
		void onExit		(const ExitEvent & ev);

	public:
		inline auto about()		-> EditorAbout		&	{ return m_about; }
		inline auto content()	-> EditorContent	&	{ return m_content; }
		inline auto dockspace() -> EditorDockspace	&	{ return m_dockspace; }
		inline auto explorer()	-> EditorExplorer	&	{ return m_explorer; }
		inline auto inspector() -> EditorInspector	&	{ return m_inspector; }
		inline auto profiler()	-> EditorProfiler	&	{ return m_profiler; }
		inline auto terminal()	-> EditorTerminal	&	{ return m_terminal; }

	private:
		EditorAbout		m_about;
		EditorDockspace	m_dockspace;
		EditorContent	m_content;
		EditorExplorer	m_explorer;
		EditorInspector	m_inspector;
		EditorManual	m_manual;
		EditorProfiler	m_profiler;
		EditorTerminal	m_terminal;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_HPP_