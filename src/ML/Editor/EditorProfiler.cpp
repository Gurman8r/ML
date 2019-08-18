#include <ML/Editor/EditorProfiler.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Engine/GameTime.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	EditorProfiler::EditorProfiler(Editor & editor)
		: EditorForm { editor, "Profiler", false }
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool EditorProfiler::onGui(const GuiEvent & ev)
	{
		ImGuiStyle & style = ImGui::GetStyle();
		if (beginDraw(ImGuiWindowFlags_None))
		{
			graph.update("##Framerate", (float_t)ev.time.frameRate() + ev.time.elapsed().delta(), "fps {0}");
			
			ImGui::Separator();
			
			if (ImGui::BeginChild(
				"Allocations",
				{ 0, (ImGui::GetContentRegionAvail().y - 4 * style.ItemSpacing.y) / 2 },
				true
			))
			{
				ImGui::Text("Allocations: %u", ML_MemoryTracker.records().size());
				ImGui::Separator();

				ImGui::Columns(4);
				ImGui::Text("Type");
				ImGui::NextColumn();
				ImGui::Text("Index");
				ImGui::NextColumn();
				ImGui::Text("Size");
				ImGui::NextColumn();
				ImGui::Text("Address");
				ImGui::Columns(1);

				ImGui::BeginChild("Allocation##Content", { 0, 0 }, true);
				for (const auto & pair : ML_MemoryTracker.records())
				{
					const MemoryTracker::Record * r { pair.second };

					ImGui::Columns(4);
					ImGui::Text("%s", static_cast<const I_Newable *>(r->ptr)->get_type_name());
					ImGui::NextColumn();
					ImGui::Text("%u", r->index);
					ImGui::NextColumn();
					ImGui::Text("%u", r->size);
					ImGui::NextColumn();
					ImGui::Text("%p", r->ptr);
					ImGui::Columns(1);
				}
				ImGui::EndChild();
			}
			ImGui::EndChild();
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
			{ size[0], size[1] }
		);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}