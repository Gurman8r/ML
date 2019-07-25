#include <ML/Editor/PropertyDrawer.hpp>
#include <ML/Engine/Content.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Editor/EditorUtility.hpp>
#include <ML/Core/Debug.hpp>

#include <ML/Audio/Sound.hpp>
#include <ML/Engine/Entity.hpp>
#include <ML/Graphics/Font.hpp>
#include <ML/Graphics/Material.hpp>
#include <ML/Graphics/Model.hpp>
#include <ML/Graphics/Sprite.hpp>
#include <ML/Graphics/Surface.hpp>
#include <ML/Script/Script.hpp>

#include <ML/Core/Rect.hpp>
#include <ML/Graphics/Renderer.hpp>

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	template <
		class T
	> static inline bool asset_dropdown(const String & label, const T *& value)
	{
		int32_t index = ML_Content.getIndexOf<T>(value);
		if (ML_EditorUtility.StringCombo(
			label.c_str(),
			index,
			ML_Content.getKeys<T>()
		))
		{
			return (value = ML_Content.getByIndex<T>(index));
		}
		return false;
	}
}

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	// Entity Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool EntityPropertyDrawer::operator()(const String & label, const_pointer & value) const
	{
		return asset_dropdown<Entity>(label, value);
	}

	bool EntityPropertyDrawer::operator()(const String & label, const_reference value) const
	{
		return false;
	}

	bool EntityPropertyDrawer::operator()(const String & label, reference value) const
	{
		ImGui::PushID(label.c_str());

		/* * * * * * * * * * * * * * * * * * * * */

		if (Renderer * r = value.get<Renderer>())
		{
			RenderStates & states = r->states();
			if (ImGui::CollapsingHeader("Renderer"))
			{
				/* * * * * * * * * * * * * * * * * * * * */

				const Model * model = (const Model *)r->drawable();
				if (ModelPropertyDrawer()("Model##Renderer", model))
				{
					r->drawable() = model;
				}

				const Material * material = r->material();
				if (MaterialPropertyDrawer()("Material##Renderer", material))
				{
					r->material() = material;
				}

				/* * * * * * * * * * * * * * * * * * * * */

				if (ImGui::TreeNode("Alpha Testing"))
				{
					ImGui::Checkbox("##Enabled##Alpha Testing", &states.alpha.enabled);

					int32_t index = GL::indexOf(states.alpha.comp);
					if (ImGui::Combo(
						"Comparison##Alpha Testing",
						&index,
						GL::Comp_names,
						IM_ARRAYSIZE(GL::Comp_names)
					))
					{
						GL::valueAt(index, states.alpha.comp);
					}

					ImGui::DragFloat("Coeff##Alpha Testing", &states.alpha.coeff);

					ImGui::TreePop();
				}

				/* * * * * * * * * * * * * * * * * * * * */

				if (ImGui::TreeNode("Blending"))
				{
					ImGui::Checkbox("Enabled##Blending", &states.blend.enabled);

					auto factor_combo = [](C_String label, int32_t & index)
					{
						return ImGui::Combo(
							label,
							&index,
							GL::Factor_names,
							IM_ARRAYSIZE(GL::Factor_names)
						);
					};

					int32_t srcRGB = GL::indexOf(states.blend.srcRGB);
					if (factor_combo("Src RGB##Blending", srcRGB))
					{
						GL::valueAt(srcRGB, states.blend.srcRGB);
					}

					int32_t srcAlpha = GL::indexOf(states.blend.srcAlpha);
					if (factor_combo("Src Alpha##Blending", srcAlpha))
					{
						GL::valueAt(srcAlpha, states.blend.srcAlpha);
					}

					int32_t dstRGB = GL::indexOf(states.blend.dstRGB);
					if (factor_combo("Dst RGB##Blending", dstRGB))
					{
						GL::valueAt(dstRGB, states.blend.dstRGB);
					}

					int32_t dstAlpha = GL::indexOf(states.blend.dstAlpha);
					if (factor_combo("Dst Alpha##Blending", dstAlpha))
					{
						GL::valueAt(dstAlpha, states.blend.dstAlpha);
					}

					ImGui::TreePop();
				}

				/* * * * * * * * * * * * * * * * * * * * */

				if (ImGui::TreeNode("Culling"))
				{
					ImGui::Checkbox("Enabled##Culling", &states.culling.enabled);

					int32_t index = GL::indexOf(states.culling.face);
					if (ImGui::Combo(
						"Face##Culling",
						&index,
						GL::Face_names,
						IM_ARRAYSIZE(GL::Face_names)
					))
					{
						GL::valueAt(index, states.culling.face);
					}

					ImGui::TreePop();
				}

				/* * * * * * * * * * * * * * * * * * * * */

				if (ImGui::TreeNode("Depth Testing"))
				{
					ImGui::Checkbox("Enabled##Depth Testing", &states.depth.enabled);

					int32_t index = GL::indexOf(states.depth.comp);
					if (ImGui::Combo(
						"Comparison##Depth Testing",
						&index,
						GL::Comp_names,
						IM_ARRAYSIZE(GL::Comp_names)
					))
					{
						GL::valueAt(index, states.depth.comp);
					}

					ImGui::TreePop();
				}

				/* * * * * * * * * * * * * * * * * * * * */

				if (ImGui::TreeNode("Texture"))
				{
					ImGui::Checkbox("Enabled##Texture", &states.texture.enabled);

					int32_t index = GL::indexOf(states.texture.target);
					if (ImGui::Combo(
						"Target##Texture",
						&index,
						"Texture 2D\0"
						"Texture 3D\0"
						"Texture Cube Map\0"))
					{
						GL::valueAt(index, states.texture.target);
					}

					ImGui::TreePop();
				}

				/* * * * * * * * * * * * * * * * * * * * */

				if (ImGui::TreeNode("Misc"))
				{
					ImGui::Checkbox("Multisample##Misc", &states.misc.multisample);

					ImGui::Checkbox("Framebuffer SRGB##Misc", &states.misc.framebufferSRGB);

					ImGui::TreePop();
				}

				/* * * * * * * * * * * * * * * * * * * * */
			}
		}

		/* * * * * * * * * * * * * * * * * * * * */

		ImGui::PopID();
		return false;
	}


	// Font Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool FontPropertyDrawer::operator()(const String & label, const_pointer & value) const
	{
		return asset_dropdown<Font>(label, value);
	}

	bool FontPropertyDrawer::operator()(const String & label, const_reference value) const
	{
		ImGui::Text("%s", value.getInfo().family.c_str());
		return false;
	}

	bool FontPropertyDrawer::operator()(const String & label, reference value) const
	{
		return false;
	}


	// Image Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool ImagePropertyDrawer::operator()(const String & label, const_pointer & value) const
	{
		return asset_dropdown<Image>(label, value);
	}

	bool ImagePropertyDrawer::operator()(const String & label, const_reference value) const
	{
		ImGui::Text("Size: %i x %i", value.width(), value.height());
		ImGui::Text("Channels: %i", value.channels());
		return false;
	}

	bool ImagePropertyDrawer::operator()(const String & label, reference value) const
	{
		return (*this)(label, (const_reference)value);
	}


	// Material Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool MaterialPropertyDrawer::operator()(const String & label, const_pointer & value) const
	{
		return asset_dropdown<Material>(label, value);
	}

	bool MaterialPropertyDrawer::operator()(const String & label, const_reference value) const
	{
		
		return false;
	}

	bool MaterialPropertyDrawer::operator()(const String & label, reference value) const
	{
		ImGui::PushID(label.c_str());

		/* * * * * * * * * * * * * * * * * * * * */

		const Shader * shader = value.shader();
		if (ShaderPropertyDrawer()("Shader##Material", shader))
		{
			value.shader() = shader;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		if (ImGui::TreeNode(("Uniforms##" + label).c_str()))
		{
			// new uniform editor
			Uniform * u = nullptr;
			if (UniformPropertyDrawer()(("##NewUniform##Material" + label).c_str(), u))
			{
				if (!value.addUniform(u))
				{
					delete u;
					Debug::logError("A uniform with that name already exists");
				}
			}

			// do nothing if empty
			if (!value.uniforms().empty())
				ImGui::Separator();

			// to remove
			List<List<Uniform *>::iterator> toRemove;

			for (auto it = value.uniforms().begin();
				it != value.uniforms().end();
				it++)
			{
				// name
				const String name("##Uni##" + (*it)->name + "##Material##" + label);

				// Uniform Header
				ImGui::PushStyleColor(
					ImGuiCol_Header,
					{ 0.367f, 0.258f, 0.489f, 0.580f }
				);

				if (ImGui::TreeNode(((*it)->name + name).c_str()))
				{
					ImGui::PopStyleColor();

					if ((*it))
					{
						float_t height = 1;
						if ((*it)->type == uni_mat3::ID) { height = 3; }
						else if ((*it)->type == uni_mat4::ID) { height = 4; }

						ImGui::PushID(name.c_str());
						ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
						ImGui::BeginChild(
							("UniformChild##" + name).c_str(),
							{ -1, (32 * height) + (height == 1 ? 8 : -8) },
							true,
							ImGuiWindowFlags_NoScrollWithMouse
						);

						if (UniformPropertyDrawer()(name, (Uniform &)(*(*it))))
						{
							ImGui::SameLine();
							if (ImGui::Button(("Remove##" + name).c_str()))
							{
								toRemove.push_back(it);
							}
						}

						ImGui::EndChild();
						ImGui::PopStyleVar();
						ImGui::PopID();
					}
					
					ImGui::TreePop();
				}
				else
				{
					ImGui::PopStyleColor();
				}

				ImGui::Separator();
			}

			for (auto & it : toRemove)
			{
				if (*it) delete (*it);
				value.uniforms().erase(it);
			}

			/* * * * * * * * * * * * * * * * * * * * */

			ImGui::TreePop();
		}

		/* * * * * * * * * * * * * * * * * * * * */

		ImGui::PopID();

		return false;
	}


	// Mesh Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool MeshPropertyDrawer::operator()(const String & label, const_pointer & value) const
	{
		return asset_dropdown<Mesh>(label, value);
	}

	bool MeshPropertyDrawer::operator()(const String & label, const_reference value) const
	{
		ImGui::Text("Vertices: %u", value.vertices().size());
		ImGui::Text("Indices: %u", value.indices().size());
		ImGui::Text("Contiguous: %u", value.contiguous().size());
		return false;
	}

	bool MeshPropertyDrawer::operator()(const String & label, reference value) const
	{
		return (*this)(label, (const_reference)value);
	}


	// Model Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool ModelPropertyDrawer::operator()(const String & label, const_pointer & value) const
	{
		return asset_dropdown<Model>(label, value);
	}

	bool ModelPropertyDrawer::operator()(const String & label, const_reference value) const
	{
		ImGui::Text("%s", label.c_str());
		return false;
	}

	bool ModelPropertyDrawer::operator()(const String & label, reference value) const
	{
		return (*this)(label, (const_reference)value);
	}


	// Script Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool ScriptPropertyDrawer::operator()(const String & label, const_pointer & value) const
	{
		return asset_dropdown<Script>(label, value);
	}

	bool ScriptPropertyDrawer::operator()(const String & label, const_reference value) const
	{
		ImGui::Text("%s", label.c_str());
		return false;
	}

	bool ScriptPropertyDrawer::operator()(const String & label, reference value) const
	{
		return (*this)(label, (const_reference)value);
	}


	// Shader Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool ShaderPropertyDrawer::operator()(const String & label, const_pointer & value) const
	{
		return asset_dropdown<Shader>(label, value);
	}
	
	bool ShaderPropertyDrawer::operator()(const String & label, const_reference value) const
	{
		using TextEditor = ImGui::TextEditor;
		if (ImGui::BeginTabBar("SourceTabs"))
		{
			auto draw_source_tab = [](
				TextEditor & editor, 
				const String & name, 
				const String & type, 
				const String & source)
			{
				if (!source) return;
				if (ImGui::BeginTabItem((type + "##Shader##" + name).c_str()))
				{
					editor.SetLanguageDefinition(ImGui::TextEditor::LanguageDefinition::GLSL());
					editor.SetText(source);
					ImGui::EndTabItem();
				}
			};

			static TextEditor vert, frag, geom;
			static bool once = true;
			if (once || (once = false))
			{
				vert.SetLanguageDefinition(TextEditor::LanguageDefinition::GLSL());
				frag.SetLanguageDefinition(TextEditor::LanguageDefinition::GLSL());
				geom.SetLanguageDefinition(TextEditor::LanguageDefinition::GLSL());
			}
			draw_source_tab(vert, label, "Vertex", value.vertSrc());
			draw_source_tab(frag, label, "Fragment", value.fragSrc());
			draw_source_tab(geom, label, "Geometry", value.geomSrc());

			ImGui::EndTabBar();

			/* * * * * * * * * * * * * * * * * * * * */
		}
		return false;
	}

	bool ShaderPropertyDrawer::operator()(const String & label, reference value) const
	{
		return (*this)(label, (const Shader &)value);
	}


	// Sound Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool SoundPropertyDrawer::operator()(const String & label, const_pointer & value) const
	{
		return asset_dropdown<Sound>(label, value);
	}

	bool SoundPropertyDrawer::operator()(const String & label, const_reference value) const
	{
		return false;
	}

	bool SoundPropertyDrawer::operator()(const String & label, reference value) const
	{
		return false;
	}


	// Sprite Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool SpritePropertyDrawer::operator()(const String & label, const_pointer & value) const
	{
		return asset_dropdown<Sprite>(label, value);
	}

	bool SpritePropertyDrawer::operator()(const String & label, const_reference value) const
	{
		ImGui::PushID(label.c_str());

		vec4 color = value.color();
		if (ImGui::ColorEdit4("Color##Sprite", &color[0])) {}

		vec2 origin = value.origin();
		if (ImGui::InputFloat2("Origin##Sprite", &origin[0], 1)) {}

		vec2 position = value.position();
		if (ImGui::InputFloat2("Position##Sprite", &position[0], 1)) {}

		float_t rotation = value.rotation();
		if (ImGui::InputFloat("Rotation##Sprite", &rotation, 1)) {}

		vec2 scale = value.scale();
		if (ImGui::InputFloat2("Scale##Sprite", &scale[0], 1)) {}

		const Texture * tex = value.texture();
		TexturePropertyDrawer()("Texture##Sprite", tex);

		ImGui::PopID();

		return false;
	}

	bool SpritePropertyDrawer::operator()(const String & label, reference value) const
	{
		bool changed = false;

		ImGui::PushID(label.c_str());

		vec4 color = value.color();
		if (ImGui::ColorEdit4("Color##Sprite", &color[0]))
		{
			value.setColor(color);
			changed = true;
		}

		vec2 origin = value.origin();
		if (ImGui::InputFloat2("Origin##Sprite", &origin[0], 1))
		{
			value.setOrigin(origin);
			changed = true;
		}

		vec2 position = value.position();
		if (ImGui::InputFloat2("Position##Sprite", &position[0], 1))
		{
			value.setPosition(position);
			changed = true;
		}

		float_t rotation = value.rotation();
		if (ImGui::InputFloat("Rotation##Sprite", &rotation, 1))
		{
			value.setRotation(rotation);
			changed = true;
		}

		vec2 scale = value.scale();
		if (ImGui::InputFloat2("Scale##Sprite", &scale[0], 1))
		{
			value.setScale(scale);
			changed = true;
		}

		const Texture * tex = value.texture();
		if (TexturePropertyDrawer()("Texture##Sprite", tex))
		{
			value.setTexture(tex);
		}

		ImGui::PopID();

		return changed;
	}

	
	// Surface Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool SurfacePropertyDrawer::operator()(const String & label, const_pointer & value) const
	{
		return asset_dropdown<Surface>(label, value);
	}

	bool SurfacePropertyDrawer::operator()(const String & label, const_reference value) const
	{
		ImGui::PushID(label.c_str());

		const Shader * shader = value.shader();
		ShaderPropertyDrawer()("Shader##Surface", shader);

		if (ImGui::TreeNode("Preview"))
		{
			const ImGuiIO & io = ImGui::GetIO();

			auto scaleToFit = [](const vec2 & src, const vec2 & dst)
			{
				const vec2
					hs = { (dst[0] / src[0]), (dst[0] / src[0]) },
					vs = { (dst[1] / src[1]), (dst[1] / src[1]) };
				return (src * (((hs) < (vs)) ? (hs) : (vs)));
			};

			vec2 src = value.texture().size();
			vec2 pos = ML_EditorUtility.getCursorPos();
			vec2 dst = { 256, 256 };
			vec2 scl = scaleToFit(src, dst);

			ImGui::Text("%f %f", src[0], src[1]);

			ImGui::Image(
				value.texture().get_handle(),
				{ scl[0], scl[1] },
				{ 0, 1 },
				{ 1, 0 },
				{ 255, 255, 255, 255 },
				{ 255, 255, 255, 128 }
			);

			ImGui::TreePop();
		}

		ImGui::PopID();
		return false;
	}

	bool SurfacePropertyDrawer::operator()(const String & label, reference value) const
	{
		return false;
	}

	
	// Texture Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool TexturePropertyDrawer::operator()(const String & label, const_pointer & value) const
	{
		return asset_dropdown<Texture>(label, value);
	}

	bool TexturePropertyDrawer::operator()(const String & label, const_reference value) const
	{
		return false;
	}

	bool TexturePropertyDrawer::operator()(const String & label, reference value) const
	{
		/* * * * * * * * * * * * * * * * * * * * */

		const uint32_t handle = value;
		ImGui::Text("Handle: %u", handle);

		/* * * * * * * * * * * * * * * * * * * * */

		vec2u size = value.size();
		ImGui::Text("Size: %u x %u", size[0], size[1]);

		/* * * * * * * * * * * * * * * * * * * * */

		vec2u realSize = value.realSize();
		ImGui::Text("Real Size: %u x %u", realSize[0], realSize[1]);

		/* * * * * * * * * * * * * * * * * * * * */

		bool smooth = value.smooth();
		if (ImGui::Checkbox(("Smooth##" + label).c_str(), &smooth))
		{
			value.setSmooth(smooth);
		}

		/* * * * * * * * * * * * * * * * * * * * */

		bool repeated = value.repeated();
		if (ImGui::Checkbox(("Repeated##" + label).c_str(), &repeated))
		{
			value.setRepeated(repeated);
		}

		/* * * * * * * * * * * * * * * * * * * * */

		bool mipmapped = value.mipmapped();
		if (ImGui::Checkbox(("Mipmapped##" + label).c_str(), &mipmapped))
		{
			value.setMipmapped(mipmapped);
		}

		/* * * * * * * * * * * * * * * * * * * * */

		int32_t level = value.level();
		if (ImGui::InputInt(("Level##" + label).c_str(), &level))
		{
			value.setLevel(level);
		}
		ImGui::SameLine(); ML_EditorUtility.HelpMarker("WIP");

		/* * * * * * * * * * * * * * * * * * * * */

		int32_t target = GL::indexOf(value.target());
		if (ImGui::Combo(
			("Target##" + label).c_str(),
			&(target), 
			GL::Target_names, 
			3 //IM_ARRAYSIZE(GL::Target_names)
		))
		{
			GL::Target temp;
			if (GL::valueAt(target, temp))
				value.setTarget(temp);
		}
		ImGui::SameLine(); ML_EditorUtility.HelpMarker("WIP");

		/* * * * * * * * * * * * * * * * * * * * */

		int32_t colorFormat = GL::indexOf(value.colorFormat());
		if (ImGui::Combo(
			("Color Format##" + label).c_str(),
			&(colorFormat),
			GL::Format_names,
			IM_ARRAYSIZE(GL::Format_names)
		))
		{
			GL::Format temp;
			if (GL::valueAt(colorFormat, temp))
				value.setColorFormat(temp);
		}
		ImGui::SameLine(); ML_EditorUtility.HelpMarker("WIP");

		/* * * * * * * * * * * * * * * * * * * * */

		int32_t internalFormat = GL::indexOf(value.internalFormat());
		if (ImGui::Combo(
			("Internal Format##" + label).c_str(),
			&(internalFormat),
			GL::Format_names,
			IM_ARRAYSIZE(GL::Format_names)
		))
		{
			GL::Format temp;
			if (GL::valueAt(internalFormat, temp))
				value.setInternalFormat(temp);
		}
		ImGui::SameLine(); ML_EditorUtility.HelpMarker("WIP");

		/* * * * * * * * * * * * * * * * * * * * */

		int32_t pixelType = GL::indexOf(value.type());
		if (ImGui::Combo(
			("Pixel Type##" + label).c_str(),
			&(pixelType),
			GL::Type_names,
			IM_ARRAYSIZE(GL::Type_names)
		))
		{
			GL::Type temp;
			if (GL::valueAt(pixelType, temp))
				value.setType(temp);
		}
		ImGui::SameLine(); ML_EditorUtility.HelpMarker("WIP");

		/* * * * * * * * * * * * * * * * * * * * */

		if (value.target() == GL::Texture2D)
		{
			const vec2 previewSize = ([](const vec2 & src, const vec2 & dst)
			{
				const vec2
					hs = { (dst[0] / src[0]), (dst[0] / src[0]) },
					vs = { (dst[1] / src[1]), (dst[1] / src[1]) };
				return (src * (((hs) < (vs)) ? (hs) : (vs)));

			})(value.size(), { 256, 256 }); // <- target size

			ImGui::Image(
				value.get_handle(),
				{ previewSize[0], previewSize[1] },
				{ 0, 1 },
				{ 1, 0 },
				{ 255, 255, 255, 255 },
				{ 255, 255, 255, 128 }
			);
		}
		else
		{
			ML_EditorUtility.HelpMarker(
				(String)"No preview available for " + GL::nameOf(value.target()) + "s."
			);
		}

		/* * * * * * * * * * * * * * * * * * * * */

		return false;
	}


	// Uniform Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool UniformPropertyDrawer::operator()(const String & label, const_pointer & value) const
	{
		return asset_dropdown<Uniform>(label, value);
	}

	bool UniformPropertyDrawer::operator()(const String & label, const_reference value) const
	{
		return false;
	}

	bool UniformPropertyDrawer::operator()(const String & label, pointer & value) const
	{
		if (ImGui::Button(("New##" + label).c_str()))
		{
			ImGui::OpenPopup(("New Uniform##" + label).c_str());
		}
		
		if (ImGui::BeginPopupModal(
			("New Uniform##" + label).c_str(),
			nullptr,
			ImGuiWindowFlags_AlwaysAutoResize
		))
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			// Type Input
			static int32_t	type = 0;
			ImGui::Combo(
				"Type",
				&type,
				Uniform::TypeNames,
				Uniform::MAX_UNI_TYPES
			);

			// Name Input
			static char name[32] = "my_uniform\0";
			const bool enterPress = ImGui::InputText(
				"Name",
				name,
				IM_ARRAYSIZE(name),
				ImGuiInputTextFlags_EnterReturnsTrue
			);

			auto ResetPopup = []()
			{
				type = 0;
				std::strcpy(name, "my_uniform\0");
				ImGui::CloseCurrentPopup();
			};

			if ((ImGui::Button("Submit") || enterPress) && (name && !value))
			{
				switch (type)
				{
				case uni_flt1::ID: value = new uni_flt1(name, { 0 }); break;
				case uni_int1::ID: value = new uni_int1(name, { 0 }); break;
				case uni_vec2::ID: value = new uni_vec2(name, { 0 }); break;
				case uni_vec3::ID: value = new uni_vec3(name, { 0 }); break;
				case uni_vec4::ID: value = new uni_vec4(name, { 0 }); break;
				case uni_col4::ID: value = new uni_col4(name, { 0 }); break;
				case uni_mat3::ID: value = new uni_mat3(name, { 0 }); break;
				case uni_mat4::ID: value = new uni_mat4(name, { 0 }); break;
				case uni_tex2::ID: value = new uni_tex2(name, { 0 }); break;
				case uni_cube::ID: value = new uni_cube(name, { 0 }); break;
				}

				if (value)
				{
					ResetPopup();
				}
			}

			ImGui::SameLine();

			if (ImGui::Button("Cancel"))
			{
				ResetPopup();
			}

			ImGui::EndPopup();

			return value;
		}
		return false;
	}

	bool UniformPropertyDrawer::operator()(const String & label, reference value) const
	{
		switch (value.type)
		{
			// Flt1
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_flt1::ID:
			if (float_t * temp = detail::toFloat(&value))
			{
				const String name = "##" + label + "##Float##Uni" + value.name;
				ImGui::DragFloat(name.c_str(), temp, 0.0001f);
				if (auto u = value.as<uni_flt1>())
				{
					u->data = (*temp); 
					return true;
				}
			}
			break;

			// Int1
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_int1::ID:
			if (int32_t * temp = detail::toInt(&value))
			{
				const String name = "##" + label + "##Int##Uni" + value.name;
				ImGui::DragInt(name.c_str(), temp, 0.0001f);
				if (auto u = value.as<uni_int1>())
				{
					u->data = (*temp);
					return true;
				}
			}
			break;

			// Vec2
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_vec2::ID:
			if (vec2 * temp = detail::toVec2(&value))
			{
				const String name = "##" + label + "##Vec2##Uni" + value.name;
				ImGui::DragFloat2(name.c_str(), &(*temp)[0], 0.0001f);
				if (auto u = value.as<uni_vec2>())
				{
					u->data = (*temp); 
					return true;
				}
			}
			break;

			// Vec3
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_vec3::ID:
			if (vec3 * temp = detail::toVec3(&value))
			{
				const String name = "##" + label + "##Vec3##Uni" + value.name;
				ImGui::DragFloat3(name.c_str(), &(*temp)[0], 0.0001f);
				if (auto u = value.as<uni_vec3>())
				{
					u->data = (*temp);
					return true;
				}
			}
			break;

			// Vec4
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_vec4::ID:
			if (vec4 * temp = detail::toVec4(&value))
			{
				const String name = "##" + label + "##Vec4##Uni" + value.name;
				ImGui::DragFloat4(name.c_str(), &(*temp)[0], 0.0001f);
				if (auto u = value.as<uni_vec4>())
				{
					u->data = (*temp); 
					return true;
				}
			}
			break;

			// Col4
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_col4::ID:
			if (vec4 * temp = detail::toCol4(&value))
			{
				const String name = "##" + label + "##Col4##Uni" + value.name;
				ImGui::ColorEdit4(name.c_str(), &(*temp)[0]);
				if (auto u = value.as<uni_col4>())
				{
					u->data = (*temp); 
					return true;
				}
			}
			break;

			// Mat3
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_mat3::ID:
			if (mat3 * temp = detail::toMat3(&value))
			{
				const String name = "##" + label + "##Mat3##Uni" + value.name;
				ImGui::DragFloat4((name + "##00").c_str(), &(*temp)[0], 0.0001f);
				ImGui::DragFloat4((name + "##03").c_str(), &(*temp)[3], 0.0001f);
				ImGui::DragFloat4((name + "##06").c_str(), &(*temp)[6], 0.0001f);
				if (auto u = value.as<uni_mat3>())
				{
					u->data = (*temp); 
					return true;
				}
			}
			break;

			// Mat4
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_mat4::ID:
			if (mat4 * temp = detail::toMat4(&value))
			{
				const String name = "##" + label + "##Mat3##Uni" + value.name;
				ImGui::DragFloat4((name + "##00").c_str(), &(*temp)[0],  0.0001f);
				ImGui::DragFloat4((name + "##04").c_str(), &(*temp)[4],  0.0001f);
				ImGui::DragFloat4((name + "##08").c_str(), &(*temp)[8],  0.0001f);
				ImGui::DragFloat4((name + "##12").c_str(), &(*temp)[12], 0.0001f);
				if (auto u = value.as<uni_mat4>())
				{
					u->data = (*temp); 
					return true;
				}
			}
			break;

			// Tex2
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_tex2::ID:
			if (auto u = value.as<uni_tex2>())
			{
				const String name = "##" + label + "##Texture2D##Uni" + value.name;
				const Texture * temp = u->data;
				if (TexturePropertyDrawer()(name, temp)) { u->data = temp; }
				return true;
			}
			break;

			// Tex3
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_tex3::ID:
			if (auto u = value.as<uni_tex3>())
			{
				const String name = "##" + label + "##Texture3D##Uni" + value.name;
				const Texture * temp = u->data;
				if (TexturePropertyDrawer()(name, temp)) { u->data = temp; }
				return true;
			}
			break;

			// Cube
			/* * * * * * * * * * * * * * * * * * * * */
		case uni_cube::ID:
			if (auto u = value.as<uni_cube>())
			{
				const String name = "##" + label + "##TextureCubeMap##Uni" + value.name;
				const Texture * temp = u->data;
				if (TexturePropertyDrawer()(name, temp)) { u->data = temp; }
				return true;
			}
			break;
		}
		
		ImGui::SameLine();
		ML_EditorUtility.HelpMarker("This uniform cannot be modified.");
		return false;
	}
}