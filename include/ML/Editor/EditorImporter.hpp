#ifndef _ML_EDITOR_IMPORTER_HPP_
#define _ML_EDITOR_IMPORTER_HPP_

#include <ML/Editor/EditorForm.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API EditorImporter final 
		: public EditorForm
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend class Editor;

		explicit EditorImporter(Editor & editor);

		void onUpdate(const UpdateEvent & ev) override;

		bool onGui(const GuiEvent & ev) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:
		bool open_file_dialog(const String & label, bool * p_open, String & path);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_IMPORTER_HPP_