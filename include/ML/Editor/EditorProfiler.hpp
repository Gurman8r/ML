#ifndef _ML_EDITOR_PROFILER_HPP_
#define _ML_EDITOR_PROFILER_HPP_

#include <ML/Editor/EditorForm.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API EditorProfiler final : public EditorForm
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend class Editor;

		explicit EditorProfiler(Editor & editor);

		bool onGui(const GuiEvent & ev) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:
		struct GraphLines
		{
			enum { Size = 256U };

			float_t	values[Size];
			int32_t	offset;
			float_t	refresh;
			float_t	min;
			float_t	max;
			vec2	size;

			void update(C_String label, const float_t sample, const String & fmt = "{0}");
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		GraphLines graphs[1];

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_PROFILER_HPP_