#ifndef _ML_EDITOR_HPP_
#define _ML_EDITOR_HPP_

#include <ML/Core/I_EventListener.hpp>
#include <ML/Engine/EngineEvents.hpp>
#include <ML/Editor/EditorTerminal.hpp>
#include <ML/Editor/EditorExplorer.hpp>
#include <ML/Editor/EditorImporter.hpp>
#include <ML/Editor/EditorDockspace.hpp>
#include <ML/Editor/EditorContent.hpp>
#include <ML/Editor/EditorProfiler.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
		void onBeginGui	(const BeginGuiEvent & ev);
		void onGui		(const GuiEvent & ev);
		void onEndGui	(const EndGuiEvent & ev);
		void onExit		(const ExitEvent & ev);

	public:
		inline auto content()	-> EditorContent	&	{ return m_content; }
		inline auto browser()	-> EditorExplorer	&	{ return m_browser; }
		inline auto dockspace() -> EditorDockspace	&	{ return m_dockspace; }
		inline auto importer()	-> EditorImporter	&	{ return m_importer; }
		inline auto profiler()	-> EditorProfiler	&	{ return m_profiler; }
		inline auto terminal()	-> EditorTerminal	&	{ return m_terminal; }

		inline bool show_menubar() const { return m_show_menubar; }

	private:
		bool m_redirect_cout		{ false };
		bool m_show_menubar			{ true };
		bool m_show_imgui_demo		{ false };
		bool m_show_imgui_metrics	{ false };
		bool m_show_imgui_style		{ false };
		bool m_show_imgui_about		{ false };

		EditorExplorer	m_browser;
		EditorContent	m_content;
		EditorDockspace	m_dockspace;
		EditorImporter	m_importer;
		EditorProfiler	m_profiler;
		EditorTerminal	m_terminal;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_HPP_