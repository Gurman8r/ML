#include <ML/Editor/PropertyDrawer.hpp>
#include <ML/Engine/ContentManager.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Editor/ImGuiExt.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/Rect.hpp>
#include <ML/Engine/Ref.hpp>
#include <ML/Editor/FileBrowser.hpp>

#include <ML/Audio/Sound.hpp>
#include <ML/Engine/Entity.hpp>
#include <ML/Graphics/Font.hpp>
#include <ML/Graphics/Material.hpp>
#include <ML/Graphics/Model.hpp>
#include <ML/Graphics/Sprite.hpp>
#include <ML/Graphics/Surface.hpp>
#include <ML/Engine/Script.hpp>
#include <ML/Graphics/Renderer.hpp>
#include <ML/Core/FileSystem.hpp>

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	struct PropertyDrawer<>::Layout
	{
		template <class T>
		static inline bool dropdown(const String & label, const T *& value)
		{
			int32_t index = ML_Content.get_index_of<T>(value);
			return (ImGuiExt::Combo(label.c_str(), &index, ML_Content.get_keys<T>())
				? (value = ML_Content.find_by_index<T>(index))
				: false
			);
		}
	};
}

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	// Entity Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool PropertyDrawer<Entity>::operator()(const String & label, const_pointer & value, int32_t flags) const
	{
		const bool changed { PropertyDrawer<>::Layout::dropdown<value_type>(label, value) };

		return changed;
	}

	bool PropertyDrawer<Entity>::operator()(const String & label, pointer & value, int32_t flags) const
	{
		return false;
	}

	bool PropertyDrawer<Entity>::operator()(const String & label, reference value, int32_t flags) const
	{
		ImGui::PushID(ML_ADDRESSOF(&value));
		if (Renderer * r = value.get<Renderer>())
		{
			ImGui::PushID(ML_ADDRESSOF(r));
			if (ImGui::CollapsingHeader("Renderer"))
			{
				ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
				ImGui::BeginChild(("##Renderer##" + label).c_str(), { 0, 0 }, true);
				ImGui::PopStyleVar();

				/* * * * * * * * * * * * * * * * * * * * */

				bool enabled { r->enabled() };
				if (ImGui::Checkbox(("Enabled##Renderer##" + label).c_str(), &enabled))
				{
					r->setEnabled(enabled);
				}

				const Model * model = (const Model *)r->drawable();
				if (PropertyDrawer<Model>()("Model##Renderer", model))
				{
					r->setDrawable(model);
				}

				const Material * material = r->material();
				if (PropertyDrawer<Material>()("Material##Renderer", material))
				{
					r->setMaterial(material);
				}

				/* * * * * * * * * * * * * * * * * * * * */

				if (ImGui::TreeNode("Alpha"))
				{
					ImGui::Checkbox("##Enabled##AlphaState", &r->states().alpha().enabled);
					
					int32_t index = GL::index_of(r->states().alpha().predicate);
					if (ImGuiExt::Combo(
						"Comparison##Alpha Testing",
						&index,
						GL::Predicate_names,
						ML_ARRAYSIZE(GL::Predicate_names)
					))
					{
						GL::value_at(index, r->states().alpha().predicate);
					}
					ImGui::DragFloat("Coeff##AlphaState", &r->states().alpha().coeff);
					
					ImGui::TreePop();
				}

				/* * * * * * * * * * * * * * * * * * * * */

				if (ImGui::TreeNode("Blend"))
				{
					ImGui::Checkbox("Enabled##BlendState", &r->states().blend().enabled);

					auto factor_combo = [](C_String label, int32_t & index)
					{
						return ImGuiExt::Combo(
							label,
							&index,
							GL::Factor_names,
							ML_ARRAYSIZE(GL::Factor_names)
						);
					};

					int32_t srcRGB = GL::index_of(r->states().blend().srcRGB);
					if (factor_combo("Src RGB##BlendState", srcRGB))
					{
						GL::value_at(srcRGB, r->states().blend().srcRGB);
					}

					int32_t srcAlpha = GL::index_of(r->states().blend().srcAlpha);
					if (factor_combo("Src Alpha##BlendState", srcAlpha))
					{
						GL::value_at(srcAlpha, r->states().blend().srcAlpha);
					}

					int32_t dstRGB = GL::index_of(r->states().blend().dstRGB);
					if (factor_combo("Dst RGB##BlendState", dstRGB))
					{
						GL::value_at(dstRGB, r->states().blend().dstRGB);
					}

					int32_t dstAlpha = GL::index_of(r->states().blend().dstAlpha);
					if (factor_combo("Dst Alpha##BlendState", dstAlpha))
					{
						GL::value_at(dstAlpha, r->states().blend().dstAlpha);
					}
					ImGui::TreePop();
				}

				/* * * * * * * * * * * * * * * * * * * * */

				if (ImGui::TreeNode("Cull"))
				{
					ImGui::Checkbox("Enabled##CullState", &r->states().cull().enabled);

					int32_t index = GL::index_of(r->states().cull().face);
					if (ImGuiExt::Combo(
						"Face##Cull",
						&index,
						GL::Face_names,
						ML_ARRAYSIZE(GL::Face_names)
					))
					{
						GL::value_at(index, r->states().cull().face);
					}

					ImGui::TreePop();
				}

				/* * * * * * * * * * * * * * * * * * * * */

				if (ImGui::TreeNode("Depth"))
				{
					ImGui::Checkbox("Enabled##DepthState", &r->states().depth().enabled);

					ImGui::Checkbox("Mask##DepthState", &r->states().depth().mask);

					int32_t index = GL::index_of(r->states().depth().predicate);
					if (ImGuiExt::Combo(
						"Comparison##Depth",
						&index,
						GL::Predicate_names,
						ML_ARRAYSIZE(GL::Predicate_names)
					))
					{
						GL::value_at(index, r->states().depth().predicate);
					}

					ImGui::TreePop();
				}

				/* * * * * * * * * * * * * * * * * * * * */

				ImGui::EndChild();
			}
			ImGui::PopID();
		}
		ImGui::PopID();
		return false;
	}


	// Font Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool PropertyDrawer<Font>::operator()(const String & label, const_pointer & value, int32_t flags) const
	{
		const bool changed { PropertyDrawer<>::Layout::dropdown<value_type>(label, value) };

		return changed;
	}

	bool PropertyDrawer<Font>::operator()(const String & label, pointer & value, int32_t flags) const
	{
		// Popup
		const String button_label { String("{0}##NewButton##{1}").format(label, type_name().str()) };
		const String popup_label { String("Create {1}##{0}##Popup").format(label, type_name().str()) };
		if (ImGui::Button(button_label.c_str()))
		{
			ImGui::OpenPopup(popup_label.c_str());
		}
		if (ImGui::BeginPopupModal(popup_label.c_str(), 0, ImGuiWindowFlags_AlwaysAutoResize))
		{
			// State
			static bool popup_open { false };
			static char name[32] = "";
			static char asset_path[ML_MAX_PATH] = "";
			static const_pointer copy { nullptr };

			// Popup Opened
			if (!popup_open && (popup_open = true))
			{
				std::strcpy(name, ("new_" + alg::to_lower(type_name().str())).c_str());
				std::strcpy(asset_path, "");
				copy = nullptr;
			}

			// Name
			ImGui::InputText(
				("Name##" + type_name().str() + "##" + label).c_str(),
				name,
				ML_ARRAYSIZE(name)
			);

			// Copy
			if (self_type()(("Copy From##" + label), (const_pointer &)copy))
			{
				std::strcpy(asset_path, "");
			}

			// Path
			ImGui::InputText(
				("##Path##" + type_name().str() + "##" + label).c_str(),
				asset_path,
				ML_MAX_PATH
			);
			ImGui::SameLine();
			static String open_path;
			if (ImGuiExt::OpenFile(("Browse##" + label), open_path, { 1280, 720 }))
			{
				if (ML_FS.fileExists(open_path))
				{
					std::strcpy(asset_path, open_path.c_str());
					open_path.clear();
					copy = nullptr;
				}
			}

			// Submit
			const bool submit { ImGui::Button("Submit") };
			if (submit && !value)
			{
				if (copy)
				{
					if (copy->getInfo().filename)
					{
						std::strcpy(asset_path, copy->getInfo().filename.c_str());
					}
				}
				if (ML_FS.fileExists(asset_path))
				{
					value = ML_Content.create<value_type>(name, asset_path);
				}
			}
			ImGui::SameLine();

			// Cancel / Popup Closed
			const bool cancel { ImGui::Button("Cancel") };
			if (submit || cancel)
			{
				popup_open = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();

			return (submit || cancel);
		}
		return false;
	}

	bool PropertyDrawer<Font>::operator()(const String & label, reference value, int32_t flags) const
	{
		const uint32_t font_size { 40 };
		Font::Page & page { value.getPage(font_size) };

		ImGui::BeginChild(
			("##PropertyDrawer##Font##" + label).c_str(),
			{ ImGuiExt::GetContentRegionAvail()[0], font_size * 4 },
			true,
			ImGuiWindowFlags_NoScrollbar
		);
		ImGui::Text("%s", value.getInfo().family.c_str());
		ImGui::BeginGroup();
		auto draw_glyph = [](const Glyph & g) 
		{
			ImGui::Image(
				g.texture.get_handle(),
				{ g.width(), g.height() }, 
				{ 0, 0 }, 
				{ 1, 1 }
			);
		};
		for (char i = 'A'; i <= 'Z'; i++)
		{
			draw_glyph(value.getGlyph(i, font_size));
			ImGui::SameLine();
		}
		ImGui::NewLine();
		for (char i = 'a'; i <= 'z'; i++)
		{
			draw_glyph(value.getGlyph(i, font_size));
			ImGui::SameLine();
		}
		ImGui::NewLine();
		for (char i = '0'; i <= '9'; i++)
		{
			draw_glyph(value.getGlyph(i, font_size));
			ImGui::SameLine();
		}
		ImGui::NewLine();
		ImGui::EndGroup();
		ImGui::EndChild();

		return false;
	}


	// Image Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool PropertyDrawer<Image>::operator()(const String & label, const_pointer & value, int32_t flags) const
	{
		const bool changed { PropertyDrawer<>::Layout::dropdown<value_type>(label, value) };

		return changed;
	}

	bool PropertyDrawer<Image>::operator()(const String & label, pointer & value, int32_t flags) const
	{
		// Popup
		const String button_label { String("{0}##NewButton##{1}").format(label, type_name().str()) };
		const String popup_label { String("Create {1}##{0}##Popup").format(label, type_name().str()) };
		if (ImGui::Button(button_label.c_str()))
		{
			ImGui::OpenPopup(popup_label.c_str());
		}
		if (ImGui::BeginPopupModal(popup_label.c_str(), 0, ImGuiWindowFlags_AlwaysAutoResize))
		{
			// State
			static bool popup_open { false };
			static char name[32] = "";
			static char asset_path[ML_MAX_PATH] = "";
			static const_pointer copy { nullptr };

			// Popup Opened
			if (!popup_open && (popup_open = true))
			{
				std::strcpy(name, ("new_" + alg::to_lower(type_name().str())).c_str());
				std::strcpy(asset_path, "");
				copy = nullptr;
			}

			// Name
			ImGui::InputText(
				("Name##" + type_name().str() + "##" + label).c_str(),
				name,
				ML_ARRAYSIZE(name)
			);

			// Copy
			if (self_type()(("Copy From##" + label), (const_pointer &)copy))
			{
				std::strcpy(asset_path, "");
			}

			// Path
			ImGui::InputText(
				("##Path##" + type_name().str() + "##" + label).c_str(),
				asset_path,
				ML_MAX_PATH
			);
			ImGui::SameLine();
			static String open_path;
			if (ImGuiExt::OpenFile(("Browse##" + label), open_path, { 1280, 720 }))
			{
				if (ML_FS.fileExists(open_path))
				{
					std::strcpy(asset_path, open_path.c_str());
					open_path.clear();
					copy = nullptr;
				}
			}

			// Submit
			const bool submit { ImGui::Button("Submit") };
			if (submit && !value)
			{
				if (copy)
				{
					value = ML_Content.create<value_type>(name, (*copy));
				}
				else if (ML_FS.fileExists(asset_path))
				{
					value = ML_Content.create<value_type>(name, asset_path);
				}
			}
			ImGui::SameLine();

			// Cancel / Popup Closed
			const bool cancel { ImGui::Button("Cancel") };
			if (submit || cancel)
			{
				popup_open = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();

			return (submit || cancel);
		}
		return false;
	}

	bool PropertyDrawer<Image>::operator()(const String & label, reference value, int32_t flags) const
	{
		bool changed { false };

		ImGui::Columns(2, label.c_str());

		// Settings
		ImGui::Text("Size: %i x %i", value.width(), value.height());
		ImGui::Text("Channels: %i", value.channels());
		if (ImGui::Button("Flip Vertically"))
		{
			value.flipVertically();
			changed = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Flip Horizontally"))
		{
			value.flipHorizontally();
			changed = true;
		}

		ImGui::NextColumn();
#if 1
		ImGui::Text("Image previews are currently disabled.");
#else
		// Temporary Texture
		Ref<Texture> preview { "##PropertyDrawer##Image##Preview##" + label };
		if (!preview && !preview.create())
		{
			return changed;
		}

		// Lock
		static Image::Pixels pixels {};
		if (!pixels ||
			pixels.size() != value.pixels().size() ||
			pixels.front() != value.pixels().front() ||
			pixels.back() != value.pixels().back() ||
			pixels != value.pixels())
		{
			pixels = value.pixels();
			preview->loadFromImage(value);
			Debug::log("Here");
		}

		// Preview
		if (preview && (*preview))
		{
			const vec2 dst { ImGuiExt::GetContentRegionAvail() };
			const vec2 scl { alg::scale_to_fit((vec2)value.size(), dst) * 0.975f };
			const vec2 pos { ((dst - scl) * 0.5f) };

			ImGui::BeginChild(
				preview.name().c_str(),
				{ dst[0], dst[1] },
				true,
				ImGuiWindowFlags_NoScrollbar
			);
			ImGui::SetCursorPos({ pos[0], pos[1] });
			ImGui::Image(preview->get_handle(), { scl[0], scl[1] }, { 0, 1 }, { 1, 0 });
			ImGui::EndChild();
		}
#endif

		ImGui::Columns(1);
		return changed;
	}


	// Material Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool PropertyDrawer<Material>::operator()(const String & label, const_pointer & value, int32_t flags) const
	{
		const bool changed { PropertyDrawer<>::Layout::dropdown<value_type>(label, value) };

		return changed;
	}

	bool PropertyDrawer<Material>::operator()(const String & label, pointer & value, int32_t flags) const
	{
		// Popup
		const String button_label { String("{0}##NewButton##{1}").format(label, type_name().str()) };
		const String popup_label { String("Create {1}##{0}##Popup").format(label, type_name().str()) };
		if (ImGui::Button(button_label.c_str()))
		{
			ImGui::OpenPopup(popup_label.c_str());
		}
		if (ImGui::BeginPopupModal(popup_label.c_str(), 0, ImGuiWindowFlags_AlwaysAutoResize))
		{
			// State
			static bool popup_open { false };
			static char name[32] = "";
			static bool globals { false };
			static const Shader * shader { nullptr };
			static const Material * copy { nullptr };

			// Popup Opened
			if (!popup_open && (popup_open = true))
			{
				std::strcpy(name, ("new_" + alg::to_lower(type_name().str())).c_str());
				globals = false;
				copy = nullptr;
				shader = nullptr;
			}

			// Name
			ImGui::InputText(
				("Name##" + type_name().str() + "##"+ label).c_str(),
				name,
				ML_ARRAYSIZE(name)
			);

			// Defaults
			ImGui::Checkbox("Load Globals", &globals);

			// Shader
			PropertyDrawer<Shader>()("Shader", (const Shader *&)shader);

			// Copy From
			PropertyDrawer<Material>()(("Copy From##" + label), (const Material *&)copy);

			// Submit
			const bool submit { ImGui::Button("Submit") };
			if (submit && !value)
			{
				if (value = ML_Content.create<Material>(name))
				{
					if (globals)
					{
						for (const auto & pair : ML_Content.data<Uniform>())
						{
							value->add(static_cast<const Uniform *>(pair.second)->clone());
						}
					}
					if (shader)
					{
						value->setShader(shader);
					}
					if (copy)
					{
						for (const auto * u : (*copy))
						{
							if (!value->get(u->name)) { value->add(u->clone()); }
						}
					}
				}
			}
			ImGui::SameLine();

			// Cancel / Popup Closed
			const bool cancel { ImGui::Button("Cancel") };
			if (submit || cancel)
			{
				popup_open = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();

			return (submit || cancel);
		}
		return false;
	}

	bool PropertyDrawer<Material>::operator()(const String & label, reference value, int32_t flags) const
	{
		ImGui::PushID(label.c_str());

		/* * * * * * * * * * * * * * * * * * * * */

		const Shader * shader = value.shader();
		if (PropertyDrawer<Shader>()("Shader##Material", shader))
		{
			value.shader() = shader;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		if (ImGui::TreeNode(("Uniforms##" + label).c_str()))
		{
			// new uniform editor
			Uniform * u = nullptr;
			if (PropertyDrawer<Uniform>()(
				("New Uniform##Material##" + label).c_str(), 
				(Uniform *&)u
			))
			{
				if (u && !value.add(u))
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
						if ((*it)->id == uni_mat3::ID) { height = 3; }
						else if ((*it)->id == uni_mat4::ID) { height = 4; }

						ImGui::PushID(name.c_str());
						ImGui::BeginChild(
							("UniformChild##" + name).c_str(),
							{ -1, (35 * height) + 8 },
							true,
							ImGuiWindowFlags_NoScrollbar |
							ImGuiWindowFlags_NoScrollWithMouse
						);
						if (PropertyDrawer<Uniform>()(name, (Uniform &)**it))
						{
							ImGui::SameLine();
							if (ImGui::Button(("Remove##" + name).c_str()))
							{
								toRemove.push_back(it);
							}
						}
						ImGui::EndChild();
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


	// Model Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool PropertyDrawer<Model>::operator()(const String & label, const_pointer & value, int32_t flags) const
	{
		const bool changed { PropertyDrawer<>::Layout::dropdown<value_type>(label, value) };

		return changed;
	}

	bool PropertyDrawer<Model>::operator()(const String & label, pointer & value, int32_t flags) const
	{
		// Popup
		const String button_label { String("{0}##NewButton##{1}").format(label, type_name().str()) };
		const String popup_label { String("Create {1}##{0}##Popup").format(label, type_name().str()) };
		if (ImGui::Button(button_label.c_str()))
		{
			ImGui::OpenPopup(popup_label.c_str());
		}
		if (ImGui::BeginPopupModal(popup_label.c_str(), 0, ImGuiWindowFlags_AlwaysAutoResize))
		{
			// State
			static bool popup_open { false };
			static char name[32] = "";
			static char asset_path[ML_MAX_PATH] = "";
			static const_pointer copy { nullptr };

			// Popup Opened
			if (!popup_open && (popup_open = true))
			{
				std::strcpy(name, ("new_" + alg::to_lower(type_name().str())).c_str());
				std::strcpy(asset_path, "");
				copy = nullptr;
			}

			// Name
			ImGui::InputText(
				("Name##" + type_name().str() + "##" + label).c_str(),
				name,
				ML_ARRAYSIZE(name)
			);

			// Copy
			if (self_type()(("Copy From##" + label), (const_pointer &)copy))
			{
				std::strcpy(asset_path, "");
			}

			// Path
			ImGui::InputText(
				("##Path##" + type_name().str() + "##" + label).c_str(),
				asset_path,
				ML_MAX_PATH
			);
			ImGui::SameLine();
			static String open_path;
			if (ImGuiExt::OpenFile(("Browse##" + label), open_path, { 1280, 720 }))
			{
				if (ML_FS.fileExists(open_path))
				{
					std::strcpy(asset_path, open_path.c_str());
					open_path.clear();
					copy = nullptr;
				}
			}

			// Submit
			const bool submit { ImGui::Button("Submit") };
			if (submit && !value)
			{
				if (copy)
				{
					value = ML_Content.create<value_type>(name, (*copy));
				}
				else if (ML_FS.fileExists(asset_path))
				{
					value = ML_Content.create<value_type>(name, asset_path);
				}
			}
			ImGui::SameLine();

			// Cancel / Popup Closed
			const bool cancel { ImGui::Button("Cancel") };
			if (submit || cancel)
			{
				popup_open = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();

			return (submit || cancel);
		}
		return false;
	}

	bool PropertyDrawer<Model>::operator()(const String & label, reference value, int32_t flags) const
	{
		ImGui::PushID(ML_ADDRESSOF(&value));
		ImGui::Text("Meshes: %u", value.meshes().size());
		for (size_t i = 0; i < value.meshes().size(); i++)
		{
			const Mesh * mesh { value.meshes()[i] };
			ImGui::PushID(ML_ADDRESSOF(mesh));

			if (ImGui::TreeNode(String("Mesh [" + std::to_string(i) + "]##" + label).c_str()))
			{
				ImGui::Text("Vertices: %u", mesh->vertices().size());
				ImGui::Text("Indices: %u", mesh->indices().size());
				ImGui::TreePop();
			}
			ImGui::PopID();
		}
		ImGui::PopID();
		return false;
	}


	// Script Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool PropertyDrawer<Script>::operator()(const String & label, const_pointer & value, int32_t flags) const
	{
		const bool changed { PropertyDrawer<>::Layout::dropdown<value_type>(label, value) };

		return changed;
	}

	bool PropertyDrawer<Script>::operator()(const String & label, pointer & value, int32_t flags) const
	{
		// Popup
		const String button_label { String("{0}##NewButton##{1}").format(label, type_name().str()) };
		const String popup_label { String("Create {1}##{0}##Popup").format(label, type_name().str()) };
		if (ImGui::Button(button_label.c_str()))
		{
			ImGui::OpenPopup(popup_label.c_str());
		}
		if (ImGui::BeginPopupModal(popup_label.c_str(), 0, ImGuiWindowFlags_AlwaysAutoResize))
		{
			// State
			static bool popup_open { false };
			static char name[32] = "";
			static char asset_path[ML_MAX_PATH] = "";
			static const_pointer copy { nullptr };

			// Popup Opened
			if (!popup_open && (popup_open = true))
			{
				std::strcpy(name, ("new_" + alg::to_lower(type_name().str())).c_str());
				std::strcpy(asset_path, "");
				copy = nullptr;
			}

			// Name
			ImGui::InputText(
				("Name##" + type_name().str() + "##" + label).c_str(),
				name,
				ML_ARRAYSIZE(name)
			);

			// Copy
			if (self_type()(("Copy From##" + label), (const_pointer &)copy))
			{
				std::strcpy(asset_path, "");
			}

			// Path
			ImGui::InputText(
				("##Path##" + type_name().str() + "##" + label).c_str(),
				asset_path,
				ML_MAX_PATH
			);
			ImGui::SameLine();
			static String open_path;
			if (ImGuiExt::OpenFile(("Browse##" + label), open_path, { 1280, 720 }))
			{
				if (ML_FS.fileExists(open_path))
				{
					std::strcpy(asset_path, open_path.c_str());
					open_path.clear();
					copy = nullptr;
				}
			}

			// Submit
			const bool submit { ImGui::Button("Submit") };
			if (submit && !value)
			{
				if (copy)
				{
					value = ML_Content.create<value_type>(name, (*copy));
				}
				else if (ML_FS.fileExists(asset_path))
				{
					value = ML_Content.create<value_type>(name, asset_path);
				}
				else
				{
					value = ML_Content.create<value_type>(name);
				}
			}
			ImGui::SameLine();

			// Cancel / Popup Closed
			const bool cancel { ImGui::Button("Cancel") };
			if (submit || cancel)
			{
				popup_open = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();

			return (submit || cancel);
		}
		return false;
	}

	bool PropertyDrawer<Script>::operator()(const String & label, reference value, int32_t flags) const
	{
		ImGui::Text("%s", label.c_str());
		return false;
	}


	// Shader Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool PropertyDrawer<Shader>::operator()(const String & label, const_pointer & value, int32_t flags) const
	{
		const bool changed { PropertyDrawer<>::Layout::dropdown<value_type>(label, value) };

		return changed;
	}
	
	bool PropertyDrawer<Shader>::operator()(const String & label, pointer & value, int32_t flags) const
	{
		// Popup
		const String button_label { String("{0}##NewButton##{1}").format(label, type_name().str()) };
		const String popup_label { String("Create {1}##{0}##Popup").format(label, type_name().str()) };
		if (ImGui::Button(button_label.c_str()))
		{
			ImGui::OpenPopup(popup_label.c_str());
		}
		if (ImGui::BeginPopupModal(popup_label.c_str(), 0, ImGuiWindowFlags_AlwaysAutoResize))
		{
			// State
			static bool popup_open { false };
			static char name[32] = "";
			static char asset_path[ML_MAX_PATH] = "";
			static const_pointer copy { nullptr };

			// Popup Opened
			if (!popup_open && (popup_open = true))
			{
				std::strcpy(name, ("new_" + alg::to_lower(type_name().str())).c_str());
				std::strcpy(asset_path, "");
				copy = nullptr;
			}

			// Name
			ImGui::InputText(
				("Name##" + type_name().str() + "##" + label).c_str(),
				name,
				ML_ARRAYSIZE(name)
			);

			// Copy
			if (self_type()(("Copy From##" + label), (const_pointer &)copy))
			{
				std::strcpy(asset_path, "");
			}

			// Path
			ImGui::InputText(
				("##Path##" + type_name().str() + "##" + label).c_str(),
				asset_path,
				ML_MAX_PATH
			);
			ImGui::SameLine();
			static String open_path;
			if (ImGuiExt::OpenFile(("Browse##" + label), open_path, { 1280, 720 }))
			{
				if (ML_FS.fileExists(open_path))
				{
					std::strcpy(asset_path, open_path.c_str());
					open_path.clear();
					copy = nullptr;
				}
			}

			// Submit
			const bool submit { ImGui::Button("Submit") };
			if (submit && !value)
			{
				if (copy)
				{
					value = ML_Content.create<value_type>(name, (*copy));
				}
				else if (ML_FS.fileExists(asset_path))
				{
					value = ML_Content.create<value_type>(name, asset_path);
				}
				else
				{
					value = ML_Content.create<value_type>(name);
				}
			}
			ImGui::SameLine();

			// Cancel / Popup Closed
			const bool cancel { ImGui::Button("Cancel") };
			if (submit || cancel)
			{
				popup_open = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();

			return (submit || cancel);
		}
		return false;
	}

	bool PropertyDrawer<Shader>::operator()(const String & label, reference value, int32_t flags) const
	{
		if (ImGui::BeginTabBar("SourceTabs"))
		{
			auto draw_source_tab = [](
				const String & name, 
				const String & type, 
				const String & source)
			{
				if (!source) return;
				if (ImGui::BeginTabItem((type + "##Shader##" + name).c_str()))
				{
					ImGui::TextUnformatted(&source[0], &source[source.size() - 1]);
					ImGui::EndTabItem();
				}
			};

			draw_source_tab(label, "Vertex", value.sources().vs);
			draw_source_tab(label, "Fragment", value.sources().fs);
			draw_source_tab(label, "Geometry", value.sources().gs);

			ImGui::EndTabBar();

			/* * * * * * * * * * * * * * * * * * * * */
		}
		return false;
	}


	// Sound Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool PropertyDrawer<Sound>::operator()(const String & label, const_pointer & value, int32_t flags) const
	{
		const bool changed { PropertyDrawer<>::Layout::dropdown<value_type>(label, value) };

		return changed;
	}

	bool PropertyDrawer<Sound>::operator()(const String & label, pointer & value, int32_t flags) const
	{
		return false;
	}

	bool PropertyDrawer<Sound>::operator()(const String & label, reference value, int32_t flags) const
	{
		return false;
	}


	// Sprite Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool PropertyDrawer<Sprite>::operator()(const String & label, const_pointer & value, int32_t flags) const
	{
		const bool changed { PropertyDrawer<>::Layout::dropdown<value_type>(label, value) };

		return changed;
	}

	bool PropertyDrawer<Sprite>::operator()(const String & label, pointer & value, int32_t flags) const
	{
		return false;
	}

	bool PropertyDrawer<Sprite>::operator()(const String & label, reference value, int32_t flags) const
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
		if (PropertyDrawer<Texture>()("Texture##Sprite", tex))
		{
			value.setTexture(tex);
		}

		ImGui::PopID();

		return changed;
	}

	
	// Surface Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool PropertyDrawer<Surface>::operator()(const String & label, const_pointer & value, int32_t flags) const
	{
		const bool changed { PropertyDrawer<>::Layout::dropdown<value_type>(label, value) };

		return changed;
	}

	bool PropertyDrawer<Surface>::operator()(const String & label, pointer & value, int32_t flags) const
	{
		return false;
	}

	bool PropertyDrawer<Surface>::operator()(const String & label, reference value, int32_t flags) const
	{
		bool changed { false };

		ImGui::Columns(2);

		ImGui::Text("Texture Handle: %u", (uint32_t)value.texture());

		const Material * material { value.material() };
		if (PropertyDrawer<Material>()(("Material##Surface##" + label), material))
		{
			//value.setMaterial(material); changed = true;
		}

		const Model * model { value.model() };
		if (PropertyDrawer<Model>()(("Model##Surface##" + label), model))
		{
			//value.setModel(model); changed = true;
		}

		ImGui::NextColumn();

		/* * * * * * * * * * * * * * * * * * * * */

		if (value)
		{
			const vec2 dst { ImGuiExt::GetContentRegionAvail() };
			const vec2 scl { alg::scale_to_fit((vec2)value.size(), dst) * 0.975f };
			const vec2 pos { ((dst - scl) * 0.5f) };

			ImGui::BeginChild(
				("##PropertyDrawer##Texture##Preview" + label).c_str(),
				{ dst[0], dst[1] },
				true,
				ImGuiWindowFlags_NoScrollbar
			);
			ImGui::SetCursorPos({ pos[0], pos[1] });
			ImGui::Image(value.get_handle(), { scl[0], scl[1] }, { 0, 1 }, { 1, 0 });
			ImGui::EndChild();
		}

		/* * * * * * * * * * * * * * * * * * * * */

		ImGui::Columns(1);
		return changed;
	}

	
	// Texture Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool PropertyDrawer<Texture>::operator()(const String & label, const_pointer & value, int32_t flags) const
	{
		const bool changed { PropertyDrawer<>::Layout::dropdown<value_type>(label, value) };

		return changed;
	}

	bool PropertyDrawer<Texture>::operator()(const String & label, pointer & value, int32_t flags) const
	{
		// Popup
		const String button_label { String("{0}##NewButton##{1}").format(label, type_name().str()) };
		const String popup_label { String("Create {1}##{0}##Popup").format(label, type_name().str()) };
		if (ImGui::Button(button_label.c_str()))
		{
			ImGui::OpenPopup(popup_label.c_str());
		}
		if (ImGui::BeginPopupModal(popup_label.c_str(), 0, ImGuiWindowFlags_AlwaysAutoResize))
		{
			// State
			static bool popup_open { false };
			static char name[32] = "";
			static Array<const Image *, 6> image { nullptr };
			static int32_t sampler_type { 0 };
			static char asset_path[ML_MAX_PATH] = "";
			static String open_path;

			// Popup Opened
			if (!popup_open && (popup_open = true))
			{
				std::strcpy(name, ("new_" + alg::to_lower(type_name().str())).c_str());
				std::strcpy(asset_path, "");
				for (auto *& e : image) e = nullptr;
				open_path.clear();
			}

			// Name
			ImGui::InputText(
				("Name##" + type_name().str() + "##"+ label).c_str(),
				name,
				ML_ARRAYSIZE(name)
			);

			// Sampler Type
			ImGuiExt::Combo(
				("Type##Sampler##" + label).c_str(), 
				&sampler_type,
				GL::Sampler_names, 
				ML_ARRAYSIZE(GL::Sampler_names)
			);

			if (sampler_type == 0)
			{
				// Copy
				if (PropertyDrawer<Image>()(("Image##" + label), (const Image *&)image[0]))
				{
					std::strcpy(asset_path, "");
				}

				// Path
				ImGui::InputText(
					("##Path##" + type_name().str() + "##" + label).c_str(),
					asset_path,
					ML_MAX_PATH
				);
				ImGui::SameLine();
				if (ImGuiExt::OpenFile(("Browse##" + label), open_path, { 1280, 720 }))
				{
					if (ML_FS.fileExists(open_path))
					{
						std::strcpy(asset_path, open_path.c_str());
						open_path.clear();
						image[0] = nullptr;
					}
				}
			}
			else if (sampler_type == 1)
			{
				ImGui::TextDisabled("Texture3D is currently not supported");
			}
			else if (sampler_type == 2)
			{
				PropertyDrawer<Image>()(("Right##Image##" + label), (const Image *&)image[0]);
				PropertyDrawer<Image>()(("Left##Image##" + label), (const Image *&)image[1]);
				PropertyDrawer<Image>()(("Top##Image##" + label), (const Image *&)image[2]);
				PropertyDrawer<Image>()(("Bottom##Image##" + label), (const Image *&)image[3]);
				PropertyDrawer<Image>()(("Front##Image##" + label), (const Image *&)image[4]);
				PropertyDrawer<Image>()(("Back##Image##" + label), (const Image *&)image[5]);
			}

			// Submit
			const bool submit { ImGui::Button("Submit") };
			if (submit && !value)
			{
				if (sampler_type == 0)
				{
					if (image[0] && (value = ML_Content.create<Texture>(name, image[0]->getFormat(), true, false)))
					{
						value->loadFromImage(*image[0]);
					}
					else if (ML_FS.fileExists(asset_path))
					{
						if (value = ML_Content.create<value_type>(name))
						{
							value->loadFromFile(asset_path);
						}
					}
				}
				else if (sampler_type == 1)
				{
					Debug::logError("Texture3D is currently not supported");
				}
				else if (sampler_type == 2)
				{
					value = new Texture { GL::TextureCubeMap, true, false };
					if (value->loadFromFaces(image))
					{
						ML_Content.insert<Texture>(name, value);
					}
					else { delete value; }
				}
			}
			ImGui::SameLine();

			// Cancel / Popup Closed
			const bool cancel { ImGui::Button("Cancel") };
			if (submit || cancel)
			{
				popup_open = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();

			return (submit || cancel);
		}
		return false;
	}

	bool PropertyDrawer<Texture>::operator()(const String & label, reference value, int32_t flags) const
	{
		bool changed = false;

		ImGui::Columns(2);

		/* * * * * * * * * * * * * * * * * * * * */

		const uint32_t handle { value };
		ImGui::Text("Handle: %u", handle);

		/* * * * * * * * * * * * * * * * * * * * */

		vec2u size { value.size() };
		ImGui::Text("Size: %u x %u", size[0], size[1]);

		/* * * * * * * * * * * * * * * * * * * * */

		vec2u realSize { value.realSize() };
		ImGui::Text("Real Size: %u x %u", realSize[0], realSize[1]);

		/* * * * * * * * * * * * * * * * * * * * */

		bool smooth { value.smooth() };
		if (ImGui::Checkbox(("Smooth##" + label).c_str(), &smooth))
		{
			value.setSmooth(smooth); changed = true;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		bool repeated { value.repeated() };
		if (ImGui::Checkbox(("Repeated##" + label).c_str(), &repeated))
		{
			value.setRepeated(repeated); 
			changed = true;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		bool mipmapped { value.mipmapped() };
		if (ImGui::Checkbox(("Mipmapped##" + label).c_str(), &mipmapped))
		{
			value.setMipmapped(mipmapped);
			changed = true;
		}
		ImGui::SameLine(); ImGuiExt::HelpMarker("Work In Progress");

		/* * * * * * * * * * * * * * * * * * * * */

		if (ImGui::Button(("Flip Horizontally##" + label).c_str()))
		{
			value.update(Image { value.copyToImage() }.flipHorizontally());
		}
		ImGui::SameLine();
		if (ImGui::Button(("Flip Vertically##" + label).c_str()))
		{
			value.update(Image { value.copyToImage() }.flipVertically());
		}

		/* * * * * * * * * * * * * * * * * * * * */

		int32_t level { value.level() };
		if (ImGui::InputInt(("Level##" + label).c_str(), &level))
		{
		}
		ImGui::SameLine(); ImGuiExt::HelpMarker("WIP");

		/* * * * * * * * * * * * * * * * * * * * */

		int32_t target { GL::index_of(value.sampler()) };
		if (ImGuiExt::Combo(
			("Sampler##" + label).c_str(),
			&(target), 
			GL::Sampler_names, 
			ML_ARRAYSIZE(GL::Sampler_names)
		))
		{
		}
		ImGui::SameLine(); ImGuiExt::HelpMarker("WIP");

		/* * * * * * * * * * * * * * * * * * * * */

		int32_t colorFormat { GL::index_of(value.color_fmt()) };
		if (ImGuiExt::Combo(
			("Color Format##" + label).c_str(),
			&(colorFormat),
			GL::Format_names,
			ML_ARRAYSIZE(GL::Format_names)
		))
		{
		}
		ImGui::SameLine(); ImGuiExt::HelpMarker("WIP");

		/* * * * * * * * * * * * * * * * * * * * */

		int32_t internalFormat { GL::index_of(value.internal_fmt()) };
		if (ImGuiExt::Combo(
			("Internal Format##" + label).c_str(),
			&(internalFormat),
			GL::Format_names,
			ML_ARRAYSIZE(GL::Format_names)
		))
		{
		}
		ImGui::SameLine(); ImGuiExt::HelpMarker("WIP");

		/* * * * * * * * * * * * * * * * * * * * */

		int32_t pixelType { GL::index_of(value.pixel_type()) };
		if (ImGuiExt::Combo(
			("Pixel Type##" + label).c_str(),
			&(pixelType),
			GL::Type_names,
			ML_ARRAYSIZE(GL::Type_names)
		))
		{
		}
		ImGui::SameLine(); ImGuiExt::HelpMarker("WIP");

		/* * * * * * * * * * * * * * * * * * * * */

		ImGui::NextColumn();

		/* * * * * * * * * * * * * * * * * * * * */

		switch (value.sampler())
		{
		case GL::Texture2D:
		{
			if (value)
			{
				const vec2 dst { ImGuiExt::GetContentRegionAvail() };
				const vec2 scl { alg::scale_to_fit((vec2)value.size(), dst) * 0.975f };
				const vec2 pos { ((dst - scl) * 0.5f) };

				ImGui::BeginChild(
					("##PropertyDrawer##Texture##Preview" + label).c_str(),
					{ dst[0], dst[1] },
					true,
					ImGuiWindowFlags_NoScrollbar
				);
				ImGui::SetCursorPos({ pos[0], pos[1] });
				ImGui::Image(value.get_handle(), { scl[0], scl[1] }, { 0, 1 }, { 1, 0 });
				ImGui::EndChild();
			}
		}
		break;
		case GL::TextureCubeMap:
		{

		}
		break;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		ImGui::Columns(1);

		return changed;
	}


	// Uniform Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool PropertyDrawer<Uniform>::operator()(const String & label, const_pointer & value, int32_t flags) const
	{
		const bool changed { PropertyDrawer<>::Layout::dropdown<value_type>(label, value) };

		return changed;
	}

	bool PropertyDrawer<Uniform>::operator()(const String & label, pointer & value, int32_t flags) const
	{
		// Popup
		const String button_label { String("{0}##NewButton##{1}").format(label, type_name().str()) };
		const String popup_label { String("Create {1}##{0}##Popup").format(label, type_name().str()) };
		if (ImGui::Button(button_label.c_str()))
		{
			ImGui::OpenPopup(popup_label.c_str());
		}
		if (ImGui::BeginPopupModal(popup_label.c_str(), 0, ImGuiWindowFlags_AlwaysAutoResize))
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			// Name Input
			static char name[32] = "new_uniform\0";
			ImGui::InputText(
				("Name##" + type_name().str() + "##"+ label).c_str(),
				name,
				ML_ARRAYSIZE(name)
			);

			// Type Input
			static int32_t	type = 0;
			ImGuiExt::Combo(
				"Type",
				&type,
				Uniform::Type_names,
				ML_ARRAYSIZE(Uniform::Type_names)
			);

			auto ResetPopup = []()
			{
				type = 0;
				std::strcpy(name, "new_uniform\0");
				ImGui::CloseCurrentPopup();
			};

			const bool submit { ImGui::Button("Submit") };
			if (submit && (name && !value))
			{
				value = ([&]()
				{
					switch (type)
					{
					case uni_bool	::ID: return (Uniform *)new uni_bool	{ name, { NULL } };
					case uni_float	::ID: return (Uniform *)new uni_float	{ name, { NULL } };
					case uni_int	::ID: return (Uniform *)new uni_int		{ name, { NULL } };
					case uni_vec2	::ID: return (Uniform *)new uni_vec2	{ name, { NULL } };
					case uni_vec3	::ID: return (Uniform *)new uni_vec3	{ name, { NULL } };
					case uni_vec4	::ID: return (Uniform *)new uni_vec4	{ name, { NULL } };
					case uni_color	::ID: return (Uniform *)new uni_color	{ name, { NULL } };
					case uni_mat3	::ID: return (Uniform *)new uni_mat3	{ name, { NULL } };
					case uni_mat4	::ID: return (Uniform *)new uni_mat4	{ name, { NULL } };
					case uni_sampler::ID: return (Uniform *)new uni_sampler	{ name, { NULL } };
					}
					return (Uniform *)nullptr;
				})();

				if (value)
				{
					ResetPopup();
				}
			}

			ImGui::SameLine();

			const bool cancel { ImGui::Button("Cancel") };
			if (cancel)
			{
				ResetPopup();
			}

			ImGui::EndPopup();

			if (value && bitRead(flags, 0)) 
			{ 
				ML_Content.insert<Uniform>(value->name, value); 
			}

			return (submit || cancel);
		}
		return false;
	}

	bool PropertyDrawer<Uniform>::operator()(const String & label, reference value, int32_t flags) const
	{
		constexpr float speed = 0.001f;
		switch (value.id)
		{
		case uni_bool::ID:
			if (bool * temp = detail::as_bool(&value))
			{
				const String name = "##" + label + "##Bool##Uni" + value.name;
				ImGui::Checkbox(name.c_str(), temp);
				if (auto u = value.as<uni_bool>())
				{
					u->data = (*temp);
					return true;
				}
			}
			break;

		case uni_float::ID:
			if (float_t * temp = detail::as_float(&value))
			{
				const String name = "##" + label + "##Float##Uni" + value.name;
				ImGui::DragFloat(name.c_str(), temp, speed);
				if (auto u = value.as<uni_float>())
				{
					u->data = (*temp); 
					return true;
				}
			}
			break;

		case uni_int::ID:
			if (int32_t * temp = detail::as_int(&value))
			{
				const String name = "##" + label + "##Int##Uni" + value.name;
				
				if (value.isValue()) ImGui::InputInt(name.c_str(), temp);
				else ImGui::DragInt(name.c_str(), temp);
				
				if (auto u = value.as<uni_int>())
				{
					u->data = (*temp);
					return true;
				}
			}
			break;

		case uni_vec2::ID:
			if (vec2 * temp = detail::as_vec2(&value))
			{
				const String name = "##" + label + "##Vec2##Uni" + value.name;
				ImGui::DragFloat2(name.c_str(), &(*temp)[0], speed);
				if (auto u = value.as<uni_vec2>())
				{
					u->data = (*temp); 
					return true;
				}
			}
			break;

		case uni_vec3::ID:
			if (vec3 * temp = detail::as_vec3(&value))
			{
				const String name = "##" + label + "##Vec3##Uni" + value.name;
				ImGui::DragFloat3(name.c_str(), &(*temp)[0], speed);
				if (auto u = value.as<uni_vec3>())
				{
					u->data = (*temp);
					return true;
				}
			}
			break;

		case uni_vec4::ID:
			if (vec4 * temp = detail::as_vec4(&value))
			{
				const String name = "##" + label + "##Vec4##Uni" + value.name;
				ImGui::DragFloat4(name.c_str(), &(*temp)[0], speed);
				if (auto u = value.as<uni_vec4>())
				{
					u->data = (*temp); 
					return true;
				}
			}
			break;

		case uni_color::ID:
			if (vec4 * temp = detail::as_color(&value))
			{
				const String name = "##" + label + "##Color##Uni" + value.name;
				ImGui::ColorEdit4(name.c_str(), &(*temp)[0]);
				if (auto u = value.as<uni_color>())
				{
					u->data = (*temp); 
					return true;
				}
			}
			break;

		case uni_mat3::ID:
			if (mat3 * temp = detail::as_mat3(&value))
			{
				const String name = "##" + label + "##Mat3##Uni" + value.name;
				ImGui::DragFloat3((name + "##00").c_str(), &(*temp)[0], speed);
				ImGui::DragFloat3((name + "##03").c_str(), &(*temp)[3], speed);
				ImGui::DragFloat3((name + "##06").c_str(), &(*temp)[6], speed);
				if (auto u = value.as<uni_mat3>())
				{
					u->data = (*temp); 
					return true;
				}
			}
			break;

		case uni_mat4::ID:
			if (mat4 * temp = detail::as_mat4(&value))
			{
				const String name = "##" + label + "##Mat4##Uni" + value.name;
				ImGui::DragFloat4((name + "##00").c_str(), &(*temp)[0],  speed);
				ImGui::DragFloat4((name + "##04").c_str(), &(*temp)[4],  speed);
				ImGui::DragFloat4((name + "##08").c_str(), &(*temp)[8],  speed);
				ImGui::DragFloat4((name + "##12").c_str(), &(*temp)[12], speed);
				if (auto u = value.as<uni_mat4>())
				{
					u->data = (*temp); 
					return true;
				}
			}
			break;

		case uni_sampler::ID:
			if (auto u = value.as<uni_sampler>())
			{
				const String name = "##" + label + "##Sampler##Uni" + value.name;
				const Texture * temp = u->data;
				if (PropertyDrawer<Texture>()(name, temp, 1)) { u->data = temp; }
				return true;
			}
			break;
		}
		ImGui::SameLine();
		ImGuiExt::HelpMarker("This uniform cannot be modified.");
		return false;
	}
}