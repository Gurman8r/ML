#include <ML/Editor/EditorProfiler.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Engine/GameTime.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	EditorProfiler::EditorProfiler(Editor & editor)
		: EditorForm { editor, "Profiler", false }
		, graphs { { { uninit }, 0, 0.f, 0.f, 0.f, { 0, 80 } } }
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool EditorProfiler::onGui(const GuiEvent & ev)
	{
		ImGuiStyle & style = ImGui::GetStyle();
		if (beginDraw(ImGuiWindowFlags_None))
		{
			const float max_width { ImGui::GetContentRegionAvail().x - 4 * style.ItemSpacing.x };
			const float max_height { ImGui::GetContentRegionAvail().y - 4 * style.ItemSpacing.y };

			if (ImGui::BeginChild("Profiler Graphs", { 0, max_height / 6 }, true))
			{
				ImGui::Text("Graphs");
				if (ImGui::BeginChild("Profiler Graphs##Content Area", { 0, 0 }, true))
				{
					const float dt { ImGui::GetIO().DeltaTime };
					graphs[0].update("##DeltaTime", dt, std::to_string(dt));
				}
				ImGui::EndChild();
			}
			ImGui::EndChild();
			
			ImGui::Separator();
			
			if (ImGui::BeginChild("Active Allocations##Profiler", { 0, max_height / 2 }, true))
			{
				ImGui::Text("Active Allocations: %u", ML_MemoryTracker.records().size());

				if (ImGui::BeginChild("Active Allocation##Content##Profiler", { 0, 0 }, true))
				{
					ImGui::Columns(4, "Allocations##Columns##Profiler");
					ImGui::Text("Type"); ImGui::NextColumn();
					ImGui::Text("Index"); ImGui::NextColumn();
					ImGui::Text("Size"); ImGui::NextColumn();
					ImGui::Text("Address"); ImGui::NextColumn();
					ImGui::Separator();

					for (const auto & pair : ML_MemoryTracker.records())
					{
						const MemoryTracker::Record * r { pair.second };
						const I_Newable * ptr { static_cast<const I_Newable *>(r->ptr) };

						ImGui::Columns(4, "Allocations##Columns##Profiler");
						ImGui::Text("%s", ptr->get_type_name());
						ImGui::NextColumn();
						ImGui::Text("%u", r->index);
						ImGui::NextColumn();
						ImGui::Text("%u", r->size);
						ImGui::NextColumn();
						ImGui::Text("%p", r->ptr);
						ImGui::Columns(1);
					}
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
			offset = (offset + 1) % ML_ARRAYSIZE(values);
			refresh += (1.0f / 60.0f);
		}

		if (sample >= max)
		{
			max += ImGui::GetIO().DeltaTime;
		}

		ImGui::PlotLines(
			label,
			values,
			ML_ARRAYSIZE(values),
			offset,
			fmt.c_str(),
			min,
			max,
			{ size[0], size[1] }
		);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}