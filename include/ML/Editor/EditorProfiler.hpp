#ifndef _ML_EDITOR_PROFILER_HPP_
#define _ML_EDITOR_PROFILER_HPP_

#include <ML/Editor/EditorGui.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API EditorProfiler final : public EditorGui
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend class Editor;

		explicit EditorProfiler(Editor & editor);

		bool drawGui(const GuiEvent & ev) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:
		struct GraphLines
		{
			enum { Size = 256U };

			float_t	values[Size]= { 0 };
			int32_t	offset		= 0;
			float_t	refresh		= 0.0f;
			float_t	min			= 1.f;
			float_t	max			= 128.f;
			vec2	size		= { 0, 80 };

			void update(C_String label, const float_t sample, const String & fmt = "{0}");
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		GraphLines graph;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_PROFILER_HPP_