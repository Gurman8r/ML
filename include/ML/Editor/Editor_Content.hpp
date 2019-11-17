#ifndef _ML_EDITOR_CONTENT_HPP_
#define _ML_EDITOR_CONTENT_HPP_

#include <ML/Editor/Editor_Base.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API Editor_Content final : public Editor_Base
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend class Editor;

		struct Layout;

		Editor_Content();

		~Editor_Content() {}

		void onEvent(const Event & value) override;

		bool draw() override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		String m_typename { "Type" };
		String m_itemname { "Name" };
		voidptr_t m_selected { nullptr };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:
		inline auto select_item(const String & type, const String & name, voidptr_t obj)
		{
			m_typename = type;
			m_itemname = name;
			m_selected = obj;
		}

		inline auto select_none()
		{
			return select_item({}, {}, nullptr);
		}

		inline const auto & type_name() const { return m_typename; }
		inline const auto & item_name() const { return m_itemname; }
		inline const auto & selected() const { return m_selected; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_CONTENT_HPP_