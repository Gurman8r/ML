#ifndef _ML_EDITOR_INSPECTOR_HPP_
#define _ML_EDITOR_INSPECTOR_HPP_

#include <ML/Editor/EditorForm.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API EditorInspector final : public EditorForm
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend class Editor;

		struct Layout;

		explicit EditorInspector(Editor & editor);

		void onUpdate(const UpdateEvent & ev) override;

		bool onGui(const GuiEvent & ev) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_INSPECTOR_HPP_