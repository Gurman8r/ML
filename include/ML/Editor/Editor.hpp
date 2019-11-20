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

		friend Singleton<Editor>;

		Editor();

		~Editor();

		void onEvent(Event const & value) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool m_show_imgui_demo{ false };

		Editor_About		* m_about		{ nullptr };
		Editor_Dockspace	* m_dockspace	{ nullptr };
		Editor_Content		* m_content		{ nullptr };
		Editor_Explorer		* m_explorer	{ nullptr };
		Editor_Inspector	* m_inspector	{ nullptr };
		Editor_MainMenuBar	* m_mainMenuBar	{ nullptr };
		Editor_Manual		* m_manual		{ nullptr };
		Editor_Profiler		* m_profiler	{ nullptr };
		Editor_Terminal		* m_terminal	{ nullptr };

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