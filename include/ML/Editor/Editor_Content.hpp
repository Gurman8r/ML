#ifndef _ML_EDITOR_CONTENT_HPP_
#define _ML_EDITOR_CONTENT_HPP_

#include <ML/Editor/Editor_Widget.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API Editor_Content final : public Editor_Widget
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend class Editor;

		struct Layout;

		Editor_Content();

		~Editor_Content() {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void onEvent(Event const & value) override;

		bool draw() override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		String m_typename { "Type" };
		String m_itemname { "Name" };
		void * m_selected { nullptr };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:
		inline auto select_item(String const & type, String const & name, void * obj)
		{
			m_typename = type;
			m_itemname = name;
			m_selected = obj;
		}

		inline auto select_none()
		{
			return select_item({}, {}, nullptr);
		}

		inline auto const & type_name() const { return m_typename; }
		inline auto const & item_name() const { return m_itemname; }
		inline auto const & selected() const { return m_selected; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_CONTENT_HPP_