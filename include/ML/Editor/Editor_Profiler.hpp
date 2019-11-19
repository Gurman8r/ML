#ifndef _ML_EDITOR_PROFILER_HPP_
#define _ML_EDITOR_PROFILER_HPP_

#include <ML/Editor/Editor_Base.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API Editor_Profiler final : public Editor_Base
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend class Editor;

		Editor_Profiler();

		~Editor_Profiler() {}

		void onEvent(Event const & value) override;

		bool draw() override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:
		struct GraphLines
		{
			Array<float_t, 256> values { NULL };

			int32_t	offset	{ 0 };
			float_t	refresh	{ 0.0f };
			float_t	min		{ 0.0f };
			float_t	max		{ 0.0f };
			vec2	size	{ 0.0f, 80.0f };

			void draw(C_String label, float_t sample, C_String text);
			void render();

		private:
			String		m_label		{};
			String		m_text		{};
			float_t		m_sample	{ 0.0f };
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		Array<GraphLines, 2> graphs;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_PROFILER_HPP_