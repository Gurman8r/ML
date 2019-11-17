#include <ML/Editor/Editor_Profiler.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Window/WindowEvents.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Engine/Engine.hpp>
#include <ML/Core/StringUtility.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Editor_Profiler::Editor_Profiler()
		: Editor_Base { "Profiler", "Ctrl+Alt+P", ML_Engine.prefs().get_bool("Editor", "show_profiler", false) }
		, graphs {}
	{
		ML_EventSystem.addListener<LoadEvent>(this);
		ML_EventSystem.addListener<UpdateEvent>(this);
		ML_EventSystem.addListener<DockspaceEvent>(this);
		ML_EventSystem.addListener<KeyEvent>(this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Editor_Profiler::onEvent(const Event & value)
	{
		Editor_Base::onEvent(value);

		switch (*value)
		{
		case LoadEvent::ID: if (auto ev{ value.as<LoadEvent>() })
		{
			/* * * * * * * * * * * * * * * * * * * * */

			ML_Editor.mainMenuBar().addMenu("Window", [&]()
			{
				ImGui::PushID(ML_ADDRESSOF(this));
				ImGui::MenuItem(getTitle(), getHotkey(), openPtr());
				ImGui::PopID();
			});

			/* * * * * * * * * * * * * * * * * * * * */
		} break;
		case UpdateEvent::ID: if (auto ev{ value.as<UpdateEvent>() })
		{
			/* * * * * * * * * * * * * * * * * * * * */

			const float_t dt{ ML_Engine.time().deltaTime() };
			graphs[0].draw("Delta Time", dt, util::to_string(dt).c_str());

			const float_t fr{ (float_t)ML_Engine.time().frameRate() };
			graphs[1].draw("Frame Rate", fr, util::to_string(fr).c_str());

			/* * * * * * * * * * * * * * * * * * * * */
		} break;
		case DockspaceEvent::ID: if (auto ev{ value.as<DockspaceEvent>() })
		{
			/* * * * * * * * * * * * * * * * * * * * */

			if (Editor_Dockspace & d{ ev->dockspace }; d.isOpen())
			{
				d.dockWindow(getTitle(), d.getNode(d.LeftDn));
			}

			/* * * * * * * * * * * * * * * * * * * * */
		} break;
		case KeyEvent::ID: if (auto ev = value.as<KeyEvent>())
		{
			/* * * * * * * * * * * * * * * * * * * * */

			if (ev->getPress(KeyCode::P, { { 0, 1, 1, 0 } })) { toggleOpen(); }

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
					for (size_t i = 0; i < graphs.size(); i++)
					{
						graphs[i].render();
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

							for (const auto & [ ptr, rec ] : ML_Memory.records())
							{
								ImGui::Columns(4, "Allocations##Columns");
								ImGui::Text("%p", rec->ptr);
								ImGui::NextColumn();
								ImGui::Text("%u", rec->index);
								ImGui::NextColumn();
								ImGui::Text("%u", rec->size);
								ImGui::NextColumn();
								ImGui::Text("%s", ((const Newable *)rec->ptr)->get_type_info().name());
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

	void Editor_Profiler::GraphLines::draw(C_String label, float_t sample, C_String text)
	{
		const float_t dt { ML_Engine.time().deltaTime() };

		m_label = label;
		m_sample = sample;
		m_text = text;

		if (m_sample >= max)
		{
			max += m_sample * dt / 2;
		}
		else if (m_sample <= min)
		{
			min -= m_sample * dt / 2;
		}

		const float_t t { (float_t)ImGui::GetTime() };

		if (refresh == 0.0f) { refresh = t; }

		while (refresh < t)
		{
			values[offset] = m_sample;
			offset = (offset + 1) % values.size();
			refresh += dt;
		}
	}

	void Editor_Profiler::GraphLines::render()
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