#ifndef _ML_EDITOR_PROFILER_HPP_
#define _ML_EDITOR_PROFILER_HPP_

#include <ML/Editor/Editor_Widget.hpp>
#include <ML/Core/BitMask.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API Editor_Profiler final : public Editor_Widget
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend class Editor;

		Editor_Profiler();

		~Editor_Profiler() {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void onEvent(Event const & value) override;

		bool draw() override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		struct GraphLines
		{
			using Samples = typename std::vector<float_t>;

			Samples		m_values	{ 0 };
			int32_t		m_offset	{ 0 };
			float_t		m_refresh	{ 0.0f };
			float_t		m_min		{ 0.0f };
			float_t		m_max		{ 0.0f };
			vec2		m_size		{ 0.0f, 80.0f };
			String		m_label		{ "" };
			String		m_text		{ "" };

			GraphLines() = default;
			GraphLines(GraphLines const &) = default;

			void update(C_String label, float_t sample, C_String text);
			void render();

		private: mask8_t m_flags{ 0 };
		};

		std::vector<GraphLines> m_graphs;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_PROFILER_HPP_