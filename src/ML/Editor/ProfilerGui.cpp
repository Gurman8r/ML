#include <ML/Editor/ProfilerGui.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Engine/GameTime.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	ProfilerGui::ProfilerGui(EventSystem & eventSystem)
		: EditorGui(eventSystem, "Profiler")
	{
	}

	ProfilerGui::~ProfilerGui()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool ProfilerGui::drawGui(const GuiEvent & ev)
	{
		if (beginDraw())
		{
			graph.update("##Framerate", (float)ev.time.frameRate(), "fps {0}");
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void ProfilerGui::GraphLines::update(CString label, const float sample, const String & fmt)
	{
		if (refresh == 0.0f)
		{
			refresh = (float)ImGui::GetTime();
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