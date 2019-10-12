#include <ML/Editor/EditorProfiler.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Engine/GameTime.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EditorProfiler::EditorProfiler()
		: EditorWindow { "Profiler", "Ctrl+Alt+P", false }
		, graphs {}
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void EditorProfiler::onUpdate(const UpdateEvent & ev)
	{
		const float_t dt { ev.time.elapsed().delta() };
		graphs[0].update(ev, "Delta Time", dt, std::to_string(dt).c_str());

		const float_t fr = { (float_t)ev.time.frameRate() };
		graphs[1].update(ev, "Frame Rate", fr, std::to_string(fr).c_str());
	}

	bool EditorProfiler::onGui(const GuiEvent & ev)
	{
		ImGuiStyle & style = ImGui::GetStyle();
		if (beginDraw(ImGuiWindowFlags_None))
		{
			if (ImGui::BeginTabBar("Profiler Tabs"))
			{
				// Graphs
				/* * * * * * * * * * * * * * * * * * * * */
				if (ImGui::BeginTabItem("Graphs"))
				{
					for (size_t i = 0; i < graphs.size(); i++)
					{
						graphs[i].render(ev);
					}
					ImGui::EndTabItem();
				}

				// Active Allocations
				/* * * * * * * * * * * * * * * * * * * * */
				if (ImGui::BeginTabItem("Allocations"))
				{
					if (ImGui::BeginChild("Active Allocations", { 0, 0 }, true))
					{
						ImGui::Text("Active Allocations: %u", ML_Memory.records().size());

						if (ImGui::BeginChild("Active Allocation##Content", { 0, 0 }, true))
						{
							ImGui::Columns(4, "Allocations##Columns");
							ImGui::Text("Address"); ImGui::NextColumn();
							ImGui::Text("Index"); ImGui::NextColumn();
							ImGui::Text("Size"); ImGui::NextColumn();
							ImGui::Text("Type"); ImGui::NextColumn();
							ImGui::Separator();

							for (const auto & pair : ML_Memory.records())
							{
								const MemoryManager::Record * r { pair.second };
								const I_Newable * ptr { static_cast<const I_Newable *>(r->ptr) };

								ImGui::Columns(4, "Allocations##Columns");
								ImGui::Text("%p", r->ptr);
								ImGui::NextColumn();
								ImGui::Text("%u", r->index);
								ImGui::NextColumn();
								ImGui::Text("%u", r->size);
								ImGui::NextColumn();
								ImGui::Text("%s", ptr->get_type_info().name());
								ImGui::Columns(1);
							}
						}
						ImGui::EndChild();
					}
					ImGui::EndChild();
					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void EditorProfiler::GraphLines::update(const UpdateEvent & ev, C_String label, float_t sample, C_String text)
	{
		const float_t dt { ev.time.elapsed().delta() };

		m_label		= label;
		m_sample	= sample;
		m_text		= text;

		if (m_sample >= max)
		{
			max += m_sample * dt / 2;
		}
		else if (m_sample <= min)
		{
			min -= m_sample * dt / 2;
		}

		if (refresh == 0.0f) { refresh = ev.time.totalTime(); }

		while (refresh < ev.time.totalTime())
		{
			values[offset] = m_sample;
			offset = (offset + 1) % values.size();
			refresh += dt;
		}
	}

	void EditorProfiler::GraphLines::render(const GuiEvent & ev)
	{
		size[0] =
			(ImGui::GetContentRegionAvail().x -
			(4 * ImGui::GetStyle().ItemSpacing.x)) * (m_label ? 0.75f : 1.0f);

		ImGui::PlotLines(
			m_label.c_str(),
			values.data(),
			(int32_t)values.size(),
			offset,
			m_text.c_str(),
			min,
			max,
			{ size[0], size[1] }
		);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}