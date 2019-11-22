#include <ML/Editor/Editor_Profiler.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/PerformanceTracker.hpp>
#include <ML/Window/WindowEvents.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Engine/Engine.hpp>
#include <ML/Core/StringUtility.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Editor_Profiler::Editor_Profiler()
		: Editor_Widget { "Profiler", "Ctrl+Alt+P", ML_Engine.prefs().get_bool("Editor", "show_profiler", false) }
		, m_graphs {}
	{
		ML_EventSystem.addListener<LoadEvent>(this);
		ML_EventSystem.addListener<UpdateEvent>(this);
		ML_EventSystem.addListener<DockspaceEvent>(this);
		ML_EventSystem.addListener<KeyEvent>(this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Editor_Profiler::onEvent(Event const & value)
	{
		Editor_Widget::onEvent(value);

		switch (*value)
		{
		case LoadEvent::ID: if (auto ev{ value.as<LoadEvent>() })
		{
			/* * * * * * * * * * * * * * * * * * * * */

			ML_Editor.mainMenuBar()
				.addMenu("View", [&]()
				{
					ImGui::PushID(ML_ADDRESSOF(this));
					ImGui::MenuItem(getTitle(), getHotkey(), openPtr());
					ImGui::PopID();
				});

			m_graphs.push_back(GraphLines{});
			m_graphs[0].m_min = 0.f;
			m_graphs[0].m_max = 0.033f;

			m_graphs.push_back(GraphLines{});
			m_graphs[1].m_min = 0.f;
			m_graphs[1].m_max = 70.f;

			/* * * * * * * * * * * * * * * * * * * * */
		} break;
		case UpdateEvent::ID: if (auto ev{ value.as<UpdateEvent>() })
		{
			/* * * * * * * * * * * * * * * * * * * * */

			const auto dt{ ML_Engine.time().deltaTime() };
			const auto fr{ ML_Engine.time().frameRate() };

			m_graphs[0].update("Delta Time", dt, util::to_string(dt).c_str());
			m_graphs[1].update("Frame Rate", fr, util::to_string(fr).c_str());

			/* * * * * * * * * * * * * * * * * * * * */
		} break;
		case DockspaceEvent::ID: if (auto ev{ value.as<DockspaceEvent>() })
		{
			/* * * * * * * * * * * * * * * * * * * * */

			if (Editor_Dockspace & d{ ev.dockspace }; d.isOpen())
			{
				d.dockWindow(getTitle(), d.getNode(d.LeftDn));
			}

			/* * * * * * * * * * * * * * * * * * * * */
		} break;
		case KeyEvent::ID: if (auto ev = value.as<KeyEvent>())
		{
			/* * * * * * * * * * * * * * * * * * * * */

			if (ev.getPress(KeyCode::P, { { 0, 1, 1, 0 } })) { toggleOpen(); }

			/* * * * * * * * * * * * * * * * * * * * */
		} break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Editor_Profiler::draw()
	{
		ImGui::SetNextWindowSize({ 640, 480 }, ImGuiCond_FirstUseEver);
		if (beginDraw(ImGuiWindowFlags_None))
		{
			ImGuiStyle & style = ImGui::GetStyle();
			if (ImGui::BeginTabBar("Profiler Tabs"))
			{
				// Graphs
				/* * * * * * * * * * * * * * * * * * * * */
				if (ImGui::BeginTabItem("Graphs"))
				{
					for (auto & elem : m_graphs)
					{
						elem.render();
					}
					ImGui::EndTabItem();
				}

				// Traces
				/* * * * * * * * * * * * * * * * * * * * */
				if (ImGui::BeginTabItem("Traces"))
				{
					for (auto const & elem : ML_PerformanceTracker.traces())
					{
						ImGui::Text("%s | %fs", elem.first, elem.second.count());
					}

					ImGui::EndTabItem();
				}

				// Allocations
				/* * * * * * * * * * * * * * * * * * * * */
				if (ImGui::BeginTabItem("Allocations"))
				{
					if (ImGui::BeginChild("Active Allocations", { 0, 0 }, true))
					{
						ImGui::Text("Active Allocations: %u", ML_MemoryTracker.records().size());

						if (ImGui::BeginChild("Active Allocation##Content", { 0, 0 }, true))
						{
							ImGui::Columns(4, "Allocations##Columns");
							ImGui::Text("Address"); ImGui::NextColumn();
							ImGui::Text("Index"); ImGui::NextColumn();
							ImGui::Text("Size"); ImGui::NextColumn();
							ImGui::Text("Type"); ImGui::NextColumn();
							ImGui::Separator();

							for (auto const & [ ptr, rec ] : ML_MemoryTracker.records())
							{
								ImGui::Columns(4, "Allocations##Columns");
								ImGui::Text("%p", rec->value);
								ImGui::NextColumn();
								ImGui::Text("%u", rec->index);
								ImGui::NextColumn();
								ImGui::Text("%u", rec->size);
								ImGui::NextColumn();
								ImGui::Text("%s", rec->get_rtti().name());
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

	void Editor_Profiler::GraphLines::update(C_String label, float_t sample, C_String text)
	{
		m_label = label;
		m_text = text;

		const auto dt{ ML_Engine.time().deltaTime() };

		const auto tt{ ML_Engine.time().totalTime() };

		if (m_refresh == 0.0f)
		{
			m_refresh = tt;
		}

		while (m_refresh < tt)
		{
			m_values[m_offset] = sample;
			
			m_offset = (m_offset + 1) % m_values.size();
			
			m_refresh += dt;
		}
	}

	void Editor_Profiler::GraphLines::render()
	{
		m_size[0] =
			(ImGui::GetContentRegionAvail().x -
			(4 * ImGui::GetStyle().ItemSpacing.x)) * (m_label ? 0.75f : 1.0f);

		ImGui::PlotLines(
			m_label.c_str(),
			m_values.data(),
			(int32_t)m_values.size(),
			m_offset,
			m_text.c_str(),
			m_min,
			m_max,
			{ m_size[0], m_size[1] }
		);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}