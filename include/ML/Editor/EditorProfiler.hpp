#ifndef _ML_EDITOR_PROFILER_HPP_
#define _ML_EDITOR_PROFILER_HPP_

#include <ML/Editor/EditorForm.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API EditorProfiler final : public EditorForm
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend class Editor;

		explicit EditorProfiler(Editor & editor);

		void onUpdate(const UpdateEvent & ev) override;

		bool onGui(const GuiEvent & ev) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:
		struct GraphLines
		{
			Array<float_t, 512> values	{ uninit };
			int32_t		offset			{ 0 };
			float_t		refresh			{ 0.0f };
			float_t		min				{ 0.0f };
			float_t		max				{ 0.0f };
			vec2		size			{ 0.0f, 80.0f };

			void update(C_String label, float_t sample, C_String text);
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