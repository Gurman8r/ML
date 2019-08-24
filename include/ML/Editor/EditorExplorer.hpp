#ifndef _ML_EDITOR_EXPLORER_HPP_
#define _ML_EDITOR_EXPLORER_HPP_

#include <ML/Editor/EditorForm.hpp>
#include <ML/Editor/FileBrowser.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API EditorExplorer final : public EditorForm
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend class Editor;

		explicit EditorExplorer(Editor & editor);

		void onUpdate(const UpdateEvent & ev) override;

		bool onGui(const GuiEvent & ev) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		FileBrowser m_browser;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_EXPLORER_HPP_