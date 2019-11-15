#ifndef _ML_EDITOR_MAIN_MENU_HPP_
#define _ML_EDITOR_MAIN_MENU_HPP_

#include <ML/Editor/EditorComponent.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API Editor_MainMenuBar final : public EditorComponent
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend class Editor;

		Editor_MainMenuBar();

		~Editor_MainMenuBar() { this->dispose(); }

		bool dispose() override;

		void update() override;

		bool beginDraw(int32_t flags) override;

		bool draw() override;

		bool endDraw() override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using key_type = typename std::pair<uint32_t, String>;

		using value_type = typename std::function<void()>;

		template <class T = typename void> struct pair_less {
			using first_argument_type = typename T;
			using second_argument_type = typename T;
			using result_type = typename bool;
			constexpr bool operator()(const T & lhs, const T & rhs) const {
				return lhs.first < rhs.first;
			}
		};

		uint32_t m_index{ 0 };

		std::multimap<key_type, value_type, pair_less<key_type>> m_menus;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:
		inline auto getOrder(const String & value) const
		{
			static uint32_t index{ 0 };
			static HashMap<String, uint32_t> order{};
			auto it{ order.find(value) };
			if (it == order.end())
			{
				it = order.insert({ value, index++ }).first;
			}
			return it->second;
		}

		inline auto addMenu(const String & name, value_type && fun)
		{
			return m_menus.insert({ { getOrder(name), name }, fun });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_MAIN_MENU_HPP_