#ifndef _ML_EDITOR_HPP_
#define _ML_EDITOR_HPP_

#include <ML/Core/EventListener.hpp>
#include <ML/Editor/EditorAbout.hpp>
#include <ML/Editor/EditorContent.hpp>
#include <ML/Editor/EditorDockspace.hpp>
#include <ML/Editor/EditorExplorer.hpp>
#include <ML/Editor/EditorInspector.hpp>
#include <ML/Editor/EditorManual.hpp>
#include <ML/Editor/EditorProfiler.hpp>
#include <ML/Editor/EditorTerminal.hpp>

#define ML_Editor ::ml::Editor::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API Editor final : public Singleton<Editor>, public EventListener
	{
		friend struct Singleton<Editor>;

		Editor();
		~Editor() {}

		void onEvent(const Event & value) override;

		void onEnter	(const EnterEvent & ev);
		void onUpdate	(const UpdateEvent & ev);
		void onBeginGui	(const BeginGuiEvent & ev);
		void onGui		(const GuiEvent & ev);
		void onEndGui	(const EndGuiEvent & ev);
		void onUnload	(const UnloadEvent & ev);
		void onExit		(const ExitEvent & ev);

		EditorAbout		m_about;
		EditorDockspace	m_dockspace;
		EditorContent	m_content;
		EditorExplorer	m_explorer;
		EditorInspector	m_inspector;
		EditorManual	m_manual;
		EditorProfiler	m_profiler;
		EditorTerminal	m_terminal;

	public:
		inline auto & about()		{ return m_about; }
		inline auto & content()		{ return m_content; }
		inline auto & dockspace()	{ return m_dockspace; }
		inline auto & explorer()	{ return m_explorer; }
		inline auto & inspector()	{ return m_inspector; }
		inline auto & profiler()	{ return m_profiler; }
		inline auto & terminal()	{ return m_terminal; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_HPP_