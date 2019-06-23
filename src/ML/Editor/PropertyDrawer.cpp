#include <ML/Editor/PropertyDrawer.hpp>
#include <ML/Graphics/Uniform.hpp>
#include <ML/Engine/Content.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Editor/EditorUtility.hpp>
#include <ML/Core/Debug.hpp>

#include <ML/Audio/Sound.hpp>
#include <ML/Engine/Entity.hpp>
#include <ML/Graphics/CubeMap.hpp>
#include <ML/Graphics/Font.hpp>
#include <ML/Graphics/Model.hpp>
#include <ML/Graphics/Sprite.hpp>
#include <ML/Graphics/Surface.hpp>
#include <ML/Graphics/Uniform.hpp>
#include <ML/Script/Script.hpp>

namespace ml
{
	template <
		class T
	> inline static bool basic_asset_dropdown(const String & label, const T * value)
	{
		int32_t index = ML_Content.getIndexOf<T>(value);
		if (ML_EditorUtility.StringCombo(
			label.c_str(),
			index,
			ML_Content.keys<T>()
		))
		{
			value = ML_Content.getByIndex<T>(index);
			return true;
		}
		return false;
	}
}

namespace ml
{
	// CubeMap Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool CubeMapDrawer::operator()(const String & label, const CubeMap * value) const
	{
		return basic_asset_dropdown<CubeMap>(label, value);
	}


	// Entity Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool EntityDrawer::operator()(const String & label, const Entity * value) const
	{
		return basic_asset_dropdown<Entity>(label, value);
	}


	// Font Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool FontDrawer::operator()(const String & label, const Font * value) const
	{
		return basic_asset_dropdown<Font>(label, value);
	}


	// Image Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool ImageDrawer::operator()(const String & label, const Image * value) const
	{
		return basic_asset_dropdown<Image>(label, value);
	}


	// Material Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool MaterialDrawer::operator()(const String & label, const Material * value) const
	{
		return basic_asset_dropdown<Material>(label, value);
	}


	// Mesh Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool MeshDrawer::operator()(const String & label, const Mesh * value) const
	{
		return basic_asset_dropdown<Mesh>(label, value);
	}


	// Model Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool ModelDrawer::operator()(const String & label, const Model * value) const
	{
		return basic_asset_dropdown<Model>(label, value);
	}


	// Script Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool ScriptDrawer::operator()(const String & label, const Script * value) const
	{
		return basic_asset_dropdown<Script>(label, value);
	}


	// Shader Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool ShaderDrawer::operator()(const String & label, const Shader * value) const
	{
		return basic_asset_dropdown<Shader>(label, value);
	}
	

	// Sound Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool SoundDrawer::operator()(const String & label, const Sound * value) const
	{
		return basic_asset_dropdown<Sound>(label, value);
	}


	// Sprite Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool SpriteDrawer::operator()(const String & label, const Sprite * value) const
	{
		return basic_asset_dropdown<Sprite>(label, value);
	}

	
	// Surface Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool SurfaceDrawer::operator()(const String & label, const Surface * value) const
	{
		return basic_asset_dropdown<Surface>(label, value);
	}

	
	// Texture Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool TextureDrawer::operator()(const String & label, const Texture * value) const
	{
		return basic_asset_dropdown<Texture>(label, value);
	}


	// Uniform Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool UniformDrawer::operator()(const String & label, Uniform * value) const
	{
		if (!value) { return false; }
		switch (value->type)
		{
			// Flt
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_flt::ID:
			if (float * temp = impl::toFloat(value))
			{
				const String name = "##" + label + "##Float##Uni" + value->name;
				ImGui::DragFloat(name.c_str(), temp, 0.1f);
				if (auto u = value->as<uni_flt>()) 
					u->data = (*temp); return true;
			}
			break;

			// Int
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_int::ID:
			if (int32_t * temp = impl::toInt(value))
			{
				const String name = "##" + label + "##Int##Uni" + value->name;
				ImGui::DragInt(name.c_str(), temp, 0.1f);
				if (auto u = value->as<uni_int>()) 
					u->data = (*temp); return true;
			}
			break;

			// Vec2
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_vec2::ID:
			if (vec2 * temp = impl::toVec2(value))
			{
				const String name = "##" + label + "##Vec2##Uni" + value->name;
				ImGui::DragFloat2(name.c_str(), &(*temp)[0], 0.1f);
				if (auto u = value->as<uni_vec2>())
					u->data = (*temp); return true;
			}

			// Vec3
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_vec3::ID:
			if (vec3 * temp = impl::toVec3(value))
			{
				const String name = "##" + label + "##Vec3##Uni" + value->name;
				ImGui::DragFloat3(name.c_str(), &(*temp)[0], 0.1f);
				if (auto u = value->as<uni_vec3>())
					u->data = (*temp); return true;
			}

			// Vec4
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_vec4::ID:
			if (vec4 * temp = impl::toVec4(value))
			{
				const String name = "##" + label + "##Vec4##Uni" + value->name;
				ImGui::DragFloat4(name.c_str(), &(*temp)[0], 0.1f);
				if (auto u = value->as<uni_vec4>())
					u->data = (*temp); return true;
			}

			// Col4
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_col4::ID:
			if (vec4 * temp = impl::toCol4(value))
			{
				const String name = "##" + label + "##Col4##Uni" + value->name;
				ImGui::ColorEdit4(name.c_str(), &(*temp)[0]);
				if (auto u = value->as<uni_col4>())
					u->data = (*temp); return true;
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
				if (auto u = value->as<uni_mat3>())
					u->data = (*temp); return true;
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
				if (auto u = value->as<uni_mat4>())
					u->data = (*temp); return true;
			}

			// Tex
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_tex2::ID:
			if (auto u = value->as<uni_tex2>())
			{
				return TextureDrawer()("##Texture##Uni", u->data);;
			}
		}
		ImGui::SameLine();
		ML_EditorUtility.HelpMarker("This uniform cannot be modified.");
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

}