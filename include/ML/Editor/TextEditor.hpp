#ifndef _ML_TEXT_EDITOR_HPP_
#define _ML_TEXT_EDITOR_HPP_

#include <ML/Editor/BaseWidget.hpp>
#include <ML/Editor/Document.hpp>

namespace ml
{
	class ML_EDITOR_API TextEditor final
		: public BaseWidget
	{
		friend class Editor;

	private:
		TextEditor(Editor & editor);
		~TextEditor();

	public:
		bool drawGui(const GuiEvent * ev, bool * p_open) override;

	private:
		void draw_menu();
		void draw_tabs();
		bool edit_document(int32_t index);
		bool edit_document_name(char * buf, Document * doc);
		bool edit_document_data(Document * doc);

	public:
		inline Document * get_document(const size_t index)
		{
			return ((!m_files.empty())
				? ((index < m_files.size())
					? (&m_files.at(index))
					: (NULL))
				: (NULL));
		}

		inline Document * get_selected_document()
		{
			return get_document((size_t)m_selected);
		}

	private:
		int32_t m_selected;
		List<Document> m_files;
	};
}

#endif // !_ML_TEXT_EDITOR_HPP_