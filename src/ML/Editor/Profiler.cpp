#include <ML/Editor/Profiler.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Engine/Engine.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Profiler::Profiler()
		: GUI_Window("Profiler")
	{
	}

	Profiler::~Profiler()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Profiler::onEvent(const IEvent * value)
	{
	}

	bool Profiler::drawGui(bool * p_open)
	{
		if (beginDraw(p_open, ImGuiWindowFlags_MenuBar))
		{
			/* * * * * * * * * * * * * * * * * * * * */

			if (ImGui::BeginMenuBar())
			{
				ImGui::Text("%s (WIP)", title());
				ImGui::EndMenuBar();
			}

			/* * * * * * * * * * * * * * * * * * * * */

			graph.update("##Framerate", (float)ML_Engine.frameRate(), "fps {0}");

			/* * * * * * * * * * * * * * * * * * * * */
		}
		return endDraw();
	}

	void Profiler::GraphLines::update(CString label, const float sample, const String & fmt)
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