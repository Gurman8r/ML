#ifndef _ML_EDITOR_CONTENT_HPP_
#define _ML_EDITOR_CONTENT_HPP_

#include <ML/Editor/EditorWindow.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API EditorContent final : public EditorWindow
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend class Editor;

		struct Layout;

		explicit EditorContent();

		~EditorContent() {}

		void onUpdate(const UpdateEvent & ev) override;

		bool onGui(const GuiEvent & ev) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:
		String m_typename { "Type" };
		String m_itemname { "Name" };
		void * m_selected { nullptr };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_CONTENT_HPP_