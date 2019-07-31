#ifndef _ML_EDITOR_CONTENT_HPP_
#define _ML_EDITOR_CONTENT_HPP_

#include <ML/Editor/EditorGui.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API EditorContent final : public EditorGui
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend class Editor;

		explicit EditorContent(Editor & editor);

		bool drawGui(const GuiEvent & ev) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_CONTENT_HPP_