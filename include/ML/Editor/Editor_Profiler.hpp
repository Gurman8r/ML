#ifndef _ML_EDITOR_PROFILER_HPP_
#define _ML_EDITOR_PROFILER_HPP_

#include <ML/Editor/Editor_Widget.hpp>

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
			using Samples = typename Array<float_t, 256>;

			Samples		m_values	{ 0 };
			int32_t		m_offset	{ 0 };
			float_t		m_refresh	{ 0.0f };
			float_t		m_min		{ 0.0f };
			float_t		m_max		{ 0.0f };
			vec2		m_size		{ 0.0f, 80.0f };
			String		m_label		{ "" };
			String		m_text		{ "" };

			void update(C_String label, float_t sample, C_String text);
			void render();
		};

		ArrayList<GraphLines> m_graphs;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_PROFILER_HPP_