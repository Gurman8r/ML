#ifndef _ML_EDITOR_IMPORTER_HPP_
#define _ML_EDITOR_IMPORTER_HPP_

#include <ML/Editor/EditorWindow.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API EditorImporter final : public EditorWindow
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend class Editor;

		explicit EditorImporter(Editor & editor);

		bool onGui(const GuiEvent & ev) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_IMPORTER_HPP_