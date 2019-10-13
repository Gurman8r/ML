#ifndef _ML_EDITOR_EXPLORER_HPP_
#define _ML_EDITOR_EXPLORER_HPP_

#include <ML/Editor/EditorWindow.hpp>
#include <ML/Editor/FileBrowser.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API EditorExplorer final : public EditorWindow
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend class Editor;

		explicit EditorExplorer();

		void update(const UpdateEvent & ev) override;

		bool draw(const GuiEvent & ev) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		FileBrowser m_browser;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_EXPLORER_HPP_