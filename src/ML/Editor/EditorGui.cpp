#include <ML/Editor/EditorGui.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Engine/Resources.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Editor/EditorEvents.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EditorGui::EditorGui(EventSystem & eventSystem, CString title)
		: EventListener	(eventSystem)
		, m_title		(title)
		, m_open		(true)
		, m_good		(false)
		, m_flags		(ImGuiWindowFlags_None)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool EditorGui::beginDraw(int32_t flags)
	{
		return m_good = ImGui::Begin(m_title, &m_open, (m_flags = flags));
	}

	bool EditorGui::endDraw()
	{
		ImGui::End();
		return m_good;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool EditorGui::LabelField(CString label)
	{
		if (label)
		{
			CString label_end;
			if ((label_end = ImGui::FindRenderedTextEnd(label)) != label)
			{
				ImGui::SameLine(0, GImGui->Style.ItemInnerSpacing.x);
				ImGui::TextEx(label, label_end);
				return true;
			}
		}
		return false;
	}

	bool EditorGui::Vec2Field(CString label, vec2 & value, float speed)
	{
		if (!ImGui::GetCurrentWindow()->SkipItems)
		{
			bool changed = false;
			ImGui::BeginGroup();
			ImGui::PushID(label);
			{
				ImGui::PushItemWidth(64);
				if (ImGui::DragFloat("##X", &value[0], speed, 0.f, 0.f, "X:%.3f"))
				{
					changed = true;
				}
				ImGui::SameLine();
				if (ImGui::DragFloat("##Y", &value[1], speed, 0.f, 0.f, "Y:%.3f"))
				{
					changed = true;
				}
				ImGui::PopItemWidth();

				EditorGui::LabelField(label);
			}
			ImGui::PopID();
			ImGui::EndGroup();
			return changed;
		}
		return false;
	}

	bool EditorGui::Vec3Field(CString label, vec3 & value, float speed)
	{
		if (!ImGui::GetCurrentWindow()->SkipItems)
		{
			bool changed = false;
			ImGui::BeginGroup();
			ImGui::PushID(label);
			{
				ImGui::PushItemWidth(64);
				if (ImGui::DragFloat("##X", &value[0], speed, 0.f, 0.f, "X:%.3f"))
				{
					changed = true;
				}
				ImGui::SameLine();
				if (ImGui::DragFloat("##Y", &value[1], speed, 0.f, 0.f, "Y:%.3f"))
				{
					changed = true;
				}
				ImGui::SameLine();
				if (ImGui::DragFloat("##Z", &value[2], speed, 0.f, 0.f, "Z:%.3f"))
				{
					changed = true;
				}
				ImGui::PopItemWidth();

				EditorGui::LabelField(label);
			}
			ImGui::PopID();
			ImGui::EndGroup();
			return changed;
		}
		return false;
	}

	bool EditorGui::Vec4Field(CString label, vec4 & value, float speed)
	{
		if (!ImGui::GetCurrentWindow()->SkipItems)
		{
			bool changed = false;
			ImGui::BeginGroup();
			ImGui::PushID(label);
			{
				ImGui::PushItemWidth(64);
				if (ImGui::DragFloat("##X", &value[0], speed, 0.f, 0.f, "X:%.3f"))
				{
					changed = true;
				}
				ImGui::SameLine();
				if (ImGui::DragFloat("##Y", &value[1], speed, 0.f, 0.f, "Y:%.3f"))
				{
					changed = true;
				}
				ImGui::SameLine();
				if (ImGui::DragFloat("##Z", &value[2], speed, 0.f, 0.f, "Z:%.3f"))
				{
					changed = true;
				}
				ImGui::SameLine();
				if (ImGui::DragFloat("##W", &value[3], speed, 0.f, 0.f, "W:%.3f"))
				{
					changed = true;
				}
				ImGui::PopItemWidth();

				EditorGui::LabelField(label);
			}
			ImGui::PopID();
			ImGui::EndGroup();
			return changed;
		}
		return false;
	}

	bool EditorGui::QuatField(CString label, quat & value, float speed)
	{
		if (!ImGui::GetCurrentWindow()->SkipItems)
		{
			bool changed = false;
			ImGui::BeginGroup();
			ImGui::PushID(label);
			{
				ImGui::PushItemWidth(64);
				if (ImGui::DragFloat("##X", &value[0], speed, 0.f, 0.f, "X:%.3f"))
				{
					changed = true;
				}
				ImGui::SameLine();
				if (ImGui::DragFloat("##Y", &value[1], speed, 0.f, 0.f, "Y:%.3f"))
				{
					changed = true;
				}
				ImGui::SameLine();
				if (ImGui::DragFloat("##Z", &value[2], speed, 0.f, 0.f, "Z:%.3f"))
				{
					changed = true;
				}
				ImGui::SameLine();
				if (ImGui::DragFloat("##W", &value[3], speed, 0.f, 0.f, "W:%.3f"))
				{
					changed = true;
				}
				ImGui::PopItemWidth();

				EditorGui::LabelField(label);
			}
			ImGui::PopID();
			ImGui::EndGroup();
			return changed;
		}
		return false;
	}

	bool EditorGui::Mat3Field(CString label, mat3 & value, float speed)
	{
		return false;
	}

	bool EditorGui::Mat4Field(CString label, mat4 & value, float speed)
	{
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}