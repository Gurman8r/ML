#include <ML/Editor/Profiler.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Engine/Engine.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Profiler::Profiler(Editor & editor, bool open)
		: BaseWidget("Profiler", editor, open)
	{
	}

	Profiler::~Profiler()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Profiler::drawGui(const GuiEvent & ev)
	{
		if (beginDraw(ImGuiWindowFlags_MenuBar))
		{
			/* * * * * * * * * * * * * * * * * * * * */

			if (ImGui::BeginMenuBar())
			{
				ImGui::Text("%s (WIP)", getTitle());
				ImGui::EndMenuBar();
			}

			/* * * * * * * * * * * * * * * * * * * * */

			graph.update("##Framerate", (float)ev.editor.engine().frameRate(), "fps {0}");

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