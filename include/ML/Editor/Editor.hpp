#ifndef _ML_EDITOR_HPP_
#define _ML_EDITOR_HPP_

#include <ML/Core/EventListener.hpp>
#include <ML/Editor/Editor_About.hpp>
#include <ML/Editor/Editor_Content.hpp>
#include <ML/Editor/Editor_Dockspace.hpp>
#include <ML/Editor/Editor_Explorer.hpp>
#include <ML/Editor/Editor_Inspector.hpp>
#include <ML/Editor/Editor_MainMenuBar.hpp>
#include <ML/Editor/Editor_Manual.hpp>
#include <ML/Editor/Editor_Profiler.hpp>
#include <ML/Editor/Editor_Terminal.hpp>

#define ML_Editor ::ml::Editor::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API Editor final : public Singleton<Editor>, public EventListener
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend struct Singleton<Editor>;

		Editor();

		~Editor();

		void onEvent(const Event & value) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool m_show_imgui_demo{ false };

		ptr_t<Editor_About		> m_about;
		ptr_t<Editor_Dockspace	> m_dockspace;
		ptr_t<Editor_Content	> m_content;
		ptr_t<Editor_Explorer	> m_explorer;
		ptr_t<Editor_Inspector	> m_inspector;
		ptr_t<Editor_MainMenuBar> m_mainMenuBar;
		ptr_t<Editor_Manual		> m_manual;
		ptr_t<Editor_Profiler	> m_profiler;
		ptr_t<Editor_Terminal	> m_terminal;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:
		inline auto & about()		{ return *m_about; }
		inline auto & content()		{ return *m_content; }
		inline auto & dockspace()	{ return *m_dockspace; }
		inline auto & explorer()	{ return *m_explorer; }
		inline auto & inspector()	{ return *m_inspector; }
		inline auto & mainMenuBar()	{ return *m_mainMenuBar; }
		inline auto & manual()		{ return *m_manual; }
		inline auto & profiler()	{ return *m_profiler; }
		inline auto & terminal()	{ return *m_terminal; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_HPP_