#include <ML/Editor/EditorProfiler.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Engine/GameTime.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	EditorProfiler::EditorProfiler(Editor & editor)
		: EditorGui { editor, "Profiler", false }
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool EditorProfiler::drawGui(const GuiEvent & ev)
	{
		if (beginDraw(0))
		{
			graph.update("##Framerate", (float_t)ev.time.frameRate(), "fps {0}");
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void EditorProfiler::GraphLines::update(C_String label, const float_t sample, const String & fmt)
	{
		if (refresh == 0.0f)
		{
			refresh = (float_t)ImGui::GetTime();
		}

		while (refresh < ImGui::GetTime())
		{
			values[offset] = sample;
			offset = (offset + 1) % IM_ARRAYSIZE(values);
			refresh += (1.0f / 60.0f);
		}

		ImGui::PlotLines(
			label,
			values,
			IM_ARRAYSIZE(values),
			offset,
			fmt.format(sample).c_str(),
			min,
			max,
			{ size[0], size[1] });
	}

	/* * * * * * * * * * * * * * * * * * * * */
}