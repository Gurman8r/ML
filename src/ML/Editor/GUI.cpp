#include <ML/Editor/GUI.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Editor/ImGui_Helper.hpp>
#include <ML/Graphics/Shader.hpp>

namespace ml
{
	template <
		class	T, 
		size_t	X, 
		size_t	Y
	> inline static bool EditMatrix(CString label, Matrix<T, X, Y> & value, float speed)
	{
		if (!ImGui::GetCurrentWindow()->SkipItems)
		{
			bool changed = false;
			ImGui::PushID(label);
			{
				ImGui::BeginGroup();
				ImGui::PushItemWidth(64);
				for (size_t y = 0; y < value.Rows; y++)
				{
					for (size_t x = 0; x < value.Cols; x++)
					{
						const size_t i = (y * value.Cols + x);
						const String l = String("##{0}").format(i);

						if (ImGui::DragFloat(l.c_str(), &value[i], speed))
						{
							changed = true;
						}
						ImGui::SameLine();
					}
					ImGui::NewLine();
				}
				ImGui::PopItemWidth();
				ImGui::EndGroup();
			}
			ImGui::PopID();
			return changed;
		}
		return false;
	}
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	bool GUI::DrawLabel(CString label)
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

	/* * * * * * * * * * * * * * * * * * * * */

	bool GUI::EditVec2f(CString label, vec2 & value, float speed)
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

				DrawLabel(label);
			}
			ImGui::PopID();
			ImGui::EndGroup();
			return changed;
		}
		return false;
	}

	bool GUI::EditVec3f(CString label, vec3 & value, float speed)
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

				DrawLabel(label);
			}
			ImGui::PopID();
			ImGui::EndGroup();
			return changed;
		}
		return false;
	}

	bool GUI::EditVec4f(CString label, vec4 & value, float speed)
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

				DrawLabel(label);
			}
			ImGui::PopID();
			ImGui::EndGroup();
			return changed;
		}
		return false;
	}

	bool GUI::EditQuat(CString label, quat & value, float speed)
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

				DrawLabel(label);
			}
			ImGui::PopID();
			ImGui::EndGroup();
			return changed;
		}
		return false;
	}

	bool GUI::EditMat3f(CString label, mat3 & value, float speed)
	{
		return EditMatrix<float, 3, 3>(label, value, speed);
	}

	bool GUI::EditMat4f(CString label, mat4 & value, float speed)
	{
		return EditMatrix<float, 4, 4>(label, value, speed);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool GUI::BeginWindow(CString label, bool * p_open, int32_t flags)
	{
		return ImGui::Begin(label, p_open, flags);
	}

	void GUI::EndWindow()
	{
		return ImGui::End();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}