#include <ML/Editor/PropertyDrawer.hpp>
#include <ML/Graphics/Uniform.hpp>
#include <ML/Engine/Content.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Editor/EditorUtility.hpp>
#include <ML/Graphics/Texture.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	int32_t TextureDrawer::operator()(const String & label, const Texture * value) const
	{
		int32_t index = ML_Content.getIndexOf(value);
		if (ML_EditorUtility.StringCombo(
			label.c_str(),
			index,
			ML_Content.keys<Texture>()
		))
		{
			value = ML_Content.getByIndex<Texture>(index);
			return 1;
		}
		return 0;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	int32_t UniformDrawer::operator()(const String & label, Uniform * value) const
	{
		if (!value) { return 0; }
		switch (value->type)
		{
			// Flt
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_flt::ID:
			if (float * temp = impl::toFloat(value))
			{
				const String name = "##" + label + "##Float##Uni" + value->name;
				ImGui::DragFloat(name.c_str(), temp, 0.1f);
				if (auto u = value->as<uni_flt>()) { u->data = (*temp); return true; }
				else
				{
					ImGui::SameLine();
					ML_EditorUtility.HelpMarker("This uniform cannot be modified.");
					return false;
				}
			}
			break;

			// Int
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_int::ID:
			if (int32_t * temp = impl::toInt(value))
			{
				const String name = "##" + label + "##Int##Uni" + value->name;
				ImGui::DragInt(name.c_str(), temp, 0.1f);
				if (auto u = value->as<uni_int>()) { u->data = (*temp); return true; }
				else
				{
					ImGui::SameLine();
					ML_EditorUtility.HelpMarker("This uniform cannot be modified.");
					return false;
				}
			}
			break;

			// Vec2
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_vec2::ID:
			if (vec2 * temp = impl::toVec2(value))
			{
				const String name = "##" + label + "##Vec2##Uni" + value->name;
				ImGui::DragFloat2(name.c_str(), &(*temp)[0], 0.1f);
				if (auto u = value->as<uni_vec2>()) { u->data = (*temp); return true; }
				else
				{
					ImGui::SameLine();
					ML_EditorUtility.HelpMarker("This uniform cannot be modified.");
					return false;
				}
			}

			// Vec3
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_vec3::ID:
			if (vec3 * temp = impl::toVec3(value))
			{
				const String name = "##" + label + "##Vec3##Uni" + value->name;
				ImGui::DragFloat3(name.c_str(), &(*temp)[0], 0.1f);
				if (auto u = value->as<uni_vec3>()) { u->data = (*temp); return true; }
				else
				{
					ImGui::SameLine();
					ML_EditorUtility.HelpMarker("This uniform cannot be modified.");
					return false;
				}
			}

			// Vec4
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_vec4::ID:
			if (vec4 * temp = impl::toVec4(value))
			{
				const String name = "##" + label + "##Vec4##Uni" + value->name;
				ImGui::DragFloat4(name.c_str(), &(*temp)[0], 0.1f);
				if (auto u = value->as<uni_vec4>()) { u->data = (*temp); return true; }
				else
				{
					ImGui::SameLine();
					ML_EditorUtility.HelpMarker("This uniform cannot be modified.");
					return false;
				}
			}

			// Col4
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_col4::ID:
			if (vec4 * temp = impl::toCol4(value))
			{
				const String name = "##" + label + "##Col4##Uni" + value->name;
				ImGui::ColorEdit4(name.c_str(), &(*temp)[0]);
				if (auto u = value->as<uni_col4>()) { u->data = (*temp); return true; }
				else
				{
					ImGui::SameLine();
					ML_EditorUtility.HelpMarker("This uniform cannot be modified.");
					return false;
				}
			}

			// Mat3
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_mat3::ID:
			if (mat3 * temp = impl::toMat3(value))
			{
				const String name = "##" + label + "##Mat3##Uni" + value->name;
				ImGui::DragFloat4((name + "##00").c_str(), &(*temp)[0], 3);
				ImGui::DragFloat4((name + "##03").c_str(), &(*temp)[3], 3);
				ImGui::DragFloat4((name + "##06").c_str(), &(*temp)[6], 3);
				if (auto u = value->as<uni_mat3>()) { u->data = (*temp); return true; }
				else
				{
					ImGui::SameLine();
					ML_EditorUtility.HelpMarker("This uniform cannot be modified.");
					return false;
				}
			}

			// Mat4
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_mat4::ID:
			if (mat4 * temp = impl::toMat4(value))
			{
				const String name = "##" + label + "##Mat3##Uni" + value->name;
				ImGui::DragFloat4((name + "##00").c_str(), &(*temp)[0], 3);
				ImGui::DragFloat4((name + "##04").c_str(), &(*temp)[4], 3);
				ImGui::DragFloat4((name + "##08").c_str(), &(*temp)[8], 3);
				ImGui::DragFloat4((name + "##12").c_str(), &(*temp)[12], 3);
				if (auto u = value->as<uni_mat4>()) { u->data = (*temp); return true; }
				else
				{

					ImGui::SameLine();
					ML_EditorUtility.HelpMarker("This uniform cannot be modified.");
					return false;
				}
			}

			// Tex
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_tex2::ID:
			if (auto u = value->as<uni_tex2>())
			{
				return PropertyDrawer<const Texture>()("##Texture##Uni", u->data);
			}
			
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}