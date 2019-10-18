#include <ML/Editor/PropertyDrawer.hpp>
#include <ML/Engine/ContentManager.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Editor/ImGuiExt.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/Rect.hpp>
#include <ML/Engine/Ref.hpp>
#include <ML/Editor/FileBrowser.hpp>
#include <ML/Editor/AssetPreview.hpp>
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
#include <ML/Graphics/Light.hpp>
#include <ML/Graphics/Camera.hpp>
#include <ML/Graphics/Transform.hpp>
#include <ImGuiColorTextEdit/TextEditor.h>
#include <ML/Editor/Editor.hpp>
#include <ML/Engine/Engine.hpp>

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	struct PropertyDrawer<>::Layout
	{
		template <class T, class ... Args> static inline void begin_prop(
			const PropertyDrawer<T> * prop, 
			const String & label, 
			Args && ... value
		)
		{
			if (prop)
			{
				ImGui::PushID(prop);
				ImGui::PushID(prop->type_name.c_str());
				ImGui::PushID(label.c_str());
				ImGui::PushID(std::forward<Args>(value)...);
			}
		}

		template <class T> static inline void end_prop(const PropertyDrawer<T> * prop)
		{
			if (prop)
			{
				ImGui::PopID();
				ImGui::PopID();
				ImGui::PopID();
				ImGui::PopID();
			}
		}

		template <class T>
		static inline bool select_combo(const String & label, const T *& value)
		{
			int32_t index = ML_Content.get_index_of<T>(value);
			return (ImGuiExt::Combo(label.c_str(), &index, ML_Content.get_keys<T>())
				? (value = ML_Content.find_by_index<T>(index))
				: false
			);
		}

		template <class T>
		static inline bool inspect_button(const String & label, const T * value)
		{
			ImGui::PushID(typeof<T>::name.c_str());
			ImGui::PushID(label.c_str());
			ImGui::PushID(value);
			const bool out { ImGui::Button(("Inspect##{0}##{1}"_s.format(
				label, typeof<T>::name
			)).c_str()) };
			if (out)
			{
				ML_Editor.inspector().Focus(true);
				ML_Editor.content().select_item(
					typeof<T>::name, ML_Content.get_name(value), (void *)value
				);
			}
			ImGui::PopID();
			ImGui::PopID();
			ImGui::PopID();
			return out;
		}
	};
}

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	// Entity Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool PropertyDrawer<Entity>::operator()(const String & label, const_pointer & value) const
	{
		Layout::begin_prop(this, label, value);
		const bool changed { Layout::select_combo<value_type>(label, value) };
		if (ImGui::IsItemHovered())
		{
		}
		const String menu_label { ("##SelectorMenu##{0}##{1}"_s).format(label, typeof<value_type>::name) };
		if (ImGui::BeginPopupContextItem(menu_label.c_str()))
		{
			if (Layout::inspect_button((label + menu_label), value))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
		Layout::end_prop(this);
		return changed;
	}

	bool PropertyDrawer<Entity>::operator()(const String & label, pointer & value) const
	{
		// Popup
		const String button_label { ("{0}##NewButton##{1}"_s).format(label, typeof<value_type>::name.str()) };
		const String popup_label { ("Create {1}##{0}##Popup"_s).format(label, typeof<value_type>::name.str()) };
		if (ImGui::Button(button_label.c_str()))
		{
			ImGui::OpenPopup(popup_label.c_str());
		}
		if (ImGui::BeginPopupModal(popup_label.c_str(), 0, ImGuiWindowFlags_AlwaysAutoResize))
		{
			// State
			static bool popup_open { false };
			static char name[32] = "";

			// Popup Opened
			if (!popup_open && (popup_open = true))
			{
				std::strcpy(name, "new_entity");
			}

			// Name
			ImGui::InputText(
				("Name##" + typeof<value_type>::name.str() + "##" + label).c_str(),
				name,
				ML_ARRAYSIZE(name)
			);

			// Submit / Cancel
			const bool submit { ImGui::Button("Submit") };
			ImGui::SameLine();
			const bool cancel { ImGui::Button("Cancel") };
			if (submit && !value)
			{
				value = ML_Content.create<value_type>(name);
			}
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

	bool PropertyDrawer<Entity>::operator()(const String & label, reference value) const
	{
		ImGui::PushID(ML_ADDRESSOF(&value));

		// Add Component
		if (ImGui::Button("Add Component##Button"))
		{
			ImGui::OpenPopup("Add Component##PopupModal");
		}
		if (ImGui::BeginPopupModal(
			"Add Component##PopupModal",
			nullptr, 
			ImGuiWindowFlags_AlwaysAutoResize
		))
		{
			// Filter
			static ImGuiTextFilter filter {};
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0, 0 });
			filter.Draw("Filter", 180);
			ImGui::PopStyleVar();

			// Component List
			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
			ImGui::BeginChildFrame(
				ImGui::GetID("AddComponentMenuContent"),
				{ 380, ImGui::GetTextLineHeightWithSpacing() * 10 },
				ImGuiWindowFlags_NoMove
			);
			ImGui::PopStyleVar();
			for (const auto & pair : ML_Registry.funcs())
			{
				// Skip
				if (!filter.PassFilter(pair.first.c_str())) continue;
				const String * info { ML_Registry.get_info(pair.first) };
				if (info && *info != "/Component") continue;

				// Selectable
				if (ImGui::Selectable((pair.first + "##AddComponentMenuButton").c_str()))
				{
					if (const hash_t * code { ML_Registry.get_code(pair.first) })
					{
						void * temp { ML_Registry.generate(pair.first) };
						if (!value.addByCode(*code, temp))
						{
							Debug::logError("Failed Creating \'{0}\'", pair.first);
							delete temp;
						}
					}
					ImGui::CloseCurrentPopup();
				}
			}
			ImGui::EndChildFrame();

			// Cancel
			if (ImGui::Button("Cancel"))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		// Transform
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		if (Transform * t { value.get<Transform>() })
		{
			ImGui::PushID(ML_ADDRESSOF(t));
			const bool header_open { ImGui::CollapsingHeader("Transform") };
			if (ImGui::BeginPopupContextItem(("##ContextMenu##Transform##" + label).c_str()))
			{
				if (ImGui::Button(("Remove##Transform#Button##" + label).c_str()))
				{
					value.remove<Transform>();
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			if (header_open)
			{
				/* * * * * * * * * * * * * * * * * * * * */

				constexpr float_t speed { 0.005f };

				bool enabled = t->enabled();
				vec3 pos = t->position();
				vec3 scl = t->scale();
				vec3 rot = t->rotation();

				/* * * * * * * * * * * * * * * * * * * * */

				ImGui::BeginChildFrame(
					ImGui::GetID(("##Transform##" + label).c_str()),
					{ 0, (ImGui::GetTextLineHeightWithSpacing() * 1.25f) * 4.0f },
					true
				);

				if (ImGui::Checkbox(("Enabled##Transform##" + label).c_str(), &enabled))
				{
					t->setEnabled(enabled);
				}

				if (ImGui::DragFloat3(("Position##Transform##" + label).c_str(), &pos[0], speed))
				{
					t->setPosition(pos);
				}

				if (ImGui::DragFloat3(("Scale##Transform##" + label).c_str(), &scl[0], speed))
				{
					t->setScale(scl);
				}

				if (ImGui::DragFloat3(("Rotation##Transform##" + label).c_str(), &rot[0], speed))
				{
					t->setRotation(rot);
				}

				ImGui::EndChildFrame();

				/* * * * * * * * * * * * * * * * * * * * */
			}
			ImGui::PopID();
		}

		// Camera
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		if (Camera * c = value.get<Camera>())
		{
			ImGui::PushID(ML_ADDRESSOF(c));
			const bool header_open { ImGui::CollapsingHeader("Camera") };
			if (ImGui::BeginPopupContextItem(("##ContextMenu##Camera##" + label).c_str()))
			{
				if (ImGui::Button(("Remove##Camera#Button##" + label).c_str()))
				{
					value.remove<Camera>();
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			if (header_open)
			{
				constexpr float_t speed { 0.005f };

				bool	enabled		= c->enabled();
				int32_t clearFlags	= c->clearFlags();
				int32_t projection	= c->projection();
				vec3	position	= c->position();
				vec3	forward		= c->direction();
				vec4	background	= c->background();
				float_t fieldOfView = c->fieldOfView();
				float_t clipNear	= c->clipNear();
				float_t clipFar		= c->clipFar();
				IntRect viewport	= c->viewport();

				/* * * * * * * * * * * * * * * * * * * * */

				ImGui::BeginChildFrame(
					ImGui::GetID(("##Camera##" + label).c_str()),
					{ 0, (ImGui::GetTextLineHeightWithSpacing() * 1.25f) * 10.0f },
					true
				);

				// Enabled
				if (ImGui::Checkbox(("Enabled##Camera##" + label).c_str(), &enabled))
				{
					c->setEnabled(enabled);
				}
				ImGuiExt::Tooltip("If enabled, the camera be applied.");

				// Clear Flags
				if (ImGuiExt::Combo(
					("Clear Flags##Camera##" + label).c_str(),
					&clearFlags, 
					"Solid Color\0Depth Only\0Don't Clear"
				))
				{
					c->setClearFlags((Camera::ClearFlags)clearFlags);
				}
				ImGuiExt::Tooltip("Specify how the screen should be cleared.");

				// Projection
				if (ImGuiExt::Combo(
					("Projection##Camera##" + label).c_str(),
					&projection, 
					"Perspective\0"
				))
				{
					c->setProjection((Camera::Projection)projection);
				}
				ImGuiExt::Tooltip("Specify which projection to use.");

				// Background
				if (ImGui::ColorEdit4(("Background##Camera##" + label).c_str(), &background[0]))
				{
					c->setBackground(background);
				}
				ImGuiExt::Tooltip("Specify the color to apply when using \'Solid Color\'.");

				ImGui::Separator();

				// Position
				if (ImGui::DragFloat3(("Position##Camera##" + label).c_str(), &position[0], speed))
				{
					c->setPosition(position);
				}
				ImGuiExt::Tooltip("Set the position of the camera.");

				// Direction
				if (ImGui::DragFloat3(("Direction##Camera##" + label).c_str(), &forward[0], speed))
				{
					c->setDirection(forward);
				}
				ImGuiExt::Tooltip("Set the direction the camera is facing.");

				ImGui::Separator();

				// Field of View
				if (ImGui::DragFloat(("Field of View##Camera##" + label).c_str(), &fieldOfView, speed))
				{
					c->setFieldOfView(fieldOfView);
				}
				ImGuiExt::Tooltip("Specify the field of view.");

				// Clip Near
				if (ImGui::DragFloat(("Clip Near##Camera##" + label).c_str(), &clipNear, speed))
				{
					c->setClipNear(clipNear);
				}
				ImGuiExt::Tooltip("Specify the near clipping plane.");

				// Clip Far
				if (ImGui::DragFloat(("Clip Far##Camera##" + label).c_str(), &clipFar, speed))
				{
					c->setClipFar(clipFar);
				}
				ImGuiExt::Tooltip("Specify the far clipping plane.");

				// Viewport
				if (ImGui::DragInt4(("Viewport##Camera##" + label).c_str(), &viewport[0], speed))
				{
					c->setViewport(viewport);
				}
				ImGuiExt::Tooltip("Specify the viewport.");

				ImGui::EndChildFrame();

				/* * * * * * * * * * * * * * * * * * * * */
			}
			ImGui::PopID();
		}

		// Light
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		if (Light * l = value.get<Light>())
		{
			ImGui::PushID(ML_ADDRESSOF(l));
			const bool header_open { ImGui::CollapsingHeader("Light (WIP)") };
			if (ImGui::BeginPopupContextItem(("##ContextMenu##Light##" + label).c_str()))
			{
				if (ImGui::Button(("Remove##Light#Button##" + label).c_str()))
				{
					value.remove<Light>();
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			if (header_open)
			{
				bool enabled = l->enabled();
				vec4 color = l->color();
				float_t intensity = l->intensity();
				int32_t mode = l->mode();

				/* * * * * * * * * * * * * * * * * * * * */

				ImGui::BeginChildFrame(
					ImGui::GetID(("##Light##" + label).c_str()),
					{ 0, (ImGui::GetTextLineHeightWithSpacing() * 1.25f) * 4.0f },
					true
				);

				if (ImGui::Checkbox(("Enabled##Light##" + label).c_str(), &enabled))
				{
					l->setEnabled(enabled);
				}
				
				if (ImGui::ColorEdit4(("Color##Light##" + label).c_str(), &color[0]))
				{
					l->setColor(color);
				}

				if (ImGui::DragFloat(("Intensity##Light##" + label).c_str(), &intensity))
				{
					l->setIntensity(intensity);
				}

				if (ImGuiExt::Combo(("Mode##Light##" + label).c_str(), &mode, "Realtime"))
				{
					l->setMode((Light::Mode)mode);
				}

				ImGui::EndChildFrame();

				/* * * * * * * * * * * * * * * * * * * * */
			}
			ImGui::PopID();
		}

		// Renderer
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		if (Renderer * r = value.get<Renderer>())
		{
			ImGui::PushID(ML_ADDRESSOF(r));
			const bool header_open { ImGui::CollapsingHeader("Renderer") };
			if (ImGui::BeginPopupContextItem(("##ContextMenu##Renderer##" + label).c_str()))
			{
				if (ImGui::Button(("Remove##Renderer#Button##" + label).c_str()))
				{
					value.remove<Renderer>();
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			if (header_open)
			{
				ImGui::BeginChildFrame(
					ImGui::GetID(("##Renderer##" + label).c_str()),
					{ 0, (ImGui::GetTextLineHeightWithSpacing() * 1.25f) * 6.5f },
					true
				);

				/* * * * * * * * * * * * * * * * * * * * */

				bool enabled { r->enabled() };
				if (ImGui::Checkbox(("Enabled##Renderer##" + label).c_str(), &enabled))
				{
					r->setEnabled(enabled);
				}

				const Model * model { r->model() };
				if (PropertyDrawer<Model>()("Model##Renderer", model))
				{
					r->setModel(model);
				}

				const Material * material { r->material() };
				if (PropertyDrawer<Material>()("Material##Renderer", material))
				{
					r->setMaterial(material);
				}

				/* * * * * * * * * * * * * * * * * * * * */

				if (ImGui::TreeNode("Alpha"))
				{
					ImGui::Checkbox("Enabled##AlphaState", &r->states().alpha().enabled);

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

				ImGui::EndChildFrame();
			}
			ImGui::PopID();
		}

		ImGui::PopID();
		return false;
	}


	// Font Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool PropertyDrawer<Font>::operator()(const String & label, const_pointer & value) const
	{
		Layout::begin_prop(this, label, value);
		const bool changed { Layout::select_combo<value_type>(label, value) };
		if (ImGui::IsItemHovered())
		{
		}
		const String menu_label { ("##SelectorMenu##{0}##{1}"_s).format(label, typeof<value_type>::name) };
		if (ImGui::BeginPopupContextItem(menu_label.c_str()))
		{
			if (Layout::inspect_button((label + menu_label), value))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
		Layout::end_prop(this);
		return changed;
	}

	bool PropertyDrawer<Font>::operator()(const String & label, pointer & value) const
	{
		// Popup
		const String button_label { ("{0}##NewButton##{1}"_s).format(label, typeof<value_type>::name.str()) };
		const String popup_label { ("Create {1}##{0}##Popup"_s).format(label, typeof<value_type>::name.str()) };
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
				std::strcpy(name, "new_font");
				std::strcpy(asset_path, "");
				copy = nullptr;
			}

			// Name
			ImGui::InputText(
				("Name##" + typeof<value_type>::name.str() + "##" + label).c_str(),
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
				("##Path##" + typeof<value_type>::name.str() + "##" + label).c_str(),
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
					if (copy->info().filename)
					{
						std::strcpy(asset_path, copy->info().filename.c_str());
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

	bool PropertyDrawer<Font>::operator()(const String & label, reference value) const
	{
		const uint32_t font_size { 40 };
		Font::Page & page { value.getPage(font_size) };

		ImGui::BeginChild(
			("##PropertyDrawer##Font##" + label).c_str(),
			{ ImGuiExt::GetContentRegionAvail()[0], font_size * 4 },
			true,
			ImGuiWindowFlags_NoScrollbar
		);
		ImGui::Text("%s", value.info().family.c_str());
		ImGui::BeginGroup();
		auto draw_glyph = [](const Glyph & g) 
		{
			ImGui::Image(
				g.texture.get_address(),
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
	bool PropertyDrawer<Image>::operator()(const String & label, const_pointer & value) const
	{
		Layout::begin_prop(this, label, value);
		const bool changed { Layout::select_combo<value_type>(label, value) };
		if (ImGui::IsItemHovered())
		{
		}
		const String menu_label { ("##SelectorMenu##{0}##{1}"_s).format(label, typeof<value_type>::name) };
		if (ImGui::BeginPopupContextItem(menu_label.c_str()))
		{
			if (Layout::inspect_button((label + menu_label), value))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
		Layout::end_prop(this);
		return changed;
	}

	bool PropertyDrawer<Image>::operator()(const String & label, pointer & value) const
	{
		// Popup
		const String button_label { ("{0}##NewButton##{1}"_s).format(label, typeof<value_type>::name.str()) };
		const String popup_label { ("Create {1}##{0}##Popup"_s).format(label, typeof<value_type>::name.str()) };
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
				std::strcpy(name, "new_image");
				std::strcpy(asset_path, "");
				copy = nullptr;
			}

			// Name
			ImGui::InputText(
				("Name##" + typeof<value_type>::name.str() + "##" + label).c_str(),
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
				("##Path##" + typeof<value_type>::name.str() + "##" + label).c_str(),
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

	bool PropertyDrawer<Image>::operator()(const String & label, reference value) const
	{
		bool changed { false };

		ImGui::Text("Size: %i x %i", value.width(), value.height());
		
		ImGui::Text("Channels: %i", value.channels());

		ML_AssetPreview.drawPreview(value, ImGuiExt::GetContentRegionAvail(), nullptr);

		return changed;
	}


	// Material Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool PropertyDrawer<Material>::operator()(const String & label, const_pointer & value) const
	{
		Layout::begin_prop(this, label, value);
		const bool changed { Layout::select_combo<value_type>(label, value) };
		if (ImGui::IsItemHovered())
		{
		}
		const String menu_label { ("##SelectorMenu##{0}##{1}"_s).format(label, typeof<value_type>::name) };
		if (ImGui::BeginPopupContextItem(menu_label.c_str()))
		{
			if (Layout::inspect_button((label + menu_label), value))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
		Layout::end_prop(this);
		return changed;
	}

	bool PropertyDrawer<Material>::operator()(const String & label, pointer & value) const
	{
		// Popup
		const String button_label { ("{0}##NewButton##{1}"_s).format(label, typeof<value_type>::name.str()) };
		const String popup_label { ("Create {1}##{0}##Popup"_s).format(label, typeof<value_type>::name.str()) };
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
				std::strcpy(name, "new_material");
				globals = false;
				copy = nullptr;
				shader = nullptr;
			}

			// Name
			ImGui::InputText(
				("Name##" + typeof<value_type>::name.str() + "##"+ label).c_str(),
				name,
				ML_ARRAYSIZE(name)
			);

			// Defaults
			ImGui::Checkbox("Load Global Uniforms", &globals);
			ImGuiExt::Tooltip(
				"If true, copy all global uniforms into this material.\n"
				"**Note: Once added, constants cannot be removed."
			);

			// Copy
			PropertyDrawer<Material>()(("Copy Uniforms##" + label), (const Material *&)copy);
			ImGuiExt::Tooltip(
				"Select an existing material to copy uniforms from"
			);

			// Shader
			PropertyDrawer<Shader>()("Shader", (const Shader *&)shader);
			ImGuiExt::Tooltip(
				"Select the shader to use"
			);

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
							if (auto u { (const Uniform *)pair.second })
							{
								value->insert(u->clone());
							}
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
							if (!value->get(u->name)) { value->insert(u->clone()); }
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

	bool PropertyDrawer<Material>::operator()(const String & label, reference value) const
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
				if (u && !value.insert(u))
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
	bool PropertyDrawer<Model>::operator()(const String & label, const_pointer & value) const
	{
		Layout::begin_prop(this, label, value);
		const bool changed { Layout::select_combo<value_type>(label, value) };
		if (ImGui::IsItemHovered())
		{
		}
		const String menu_label { ("##SelectorMenu##{0}##{1}"_s).format(label, typeof<value_type>::name) };
		if (ImGui::BeginPopupContextItem(menu_label.c_str()))
		{
			if (Layout::inspect_button((label + menu_label), value))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
		Layout::end_prop(this);
		return changed;
	}

	bool PropertyDrawer<Model>::operator()(const String & label, pointer & value) const
	{
		// Popup
		const String button_label { ("{0}##NewButton##{1}"_s).format(label, typeof<value_type>::name.str()) };
		const String popup_label { ("Create {1}##{0}##Popup"_s).format(label, typeof<value_type>::name.str()) };
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
				std::strcpy(name, "new_model");
				std::strcpy(asset_path, "");
				copy = nullptr;
			}

			// Name
			ImGui::InputText(
				("Name##" + typeof<value_type>::name.str() + "##" + label).c_str(),
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
				("##Path##" + typeof<value_type>::name.str() + "##" + label).c_str(),
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

	bool PropertyDrawer<Model>::operator()(const String & label, reference value) const
	{
		ImGui::PushID(ML_ADDRESSOF(&value));
		ImGui::Text("Meshes: %u", value.meshes().size());
		for (size_t i = 0; i < value.meshes().size(); i++)
		{
			const Mesh * mesh { value.meshes()[i] };
			ImGui::PushID(ML_ADDRESSOF(mesh));

			const String meshID { String("Mesh [" + std::to_string(i) + "]##" + label) };
			if (ImGui::TreeNode(meshID.c_str()))
			{
				ImGui::Text("Vertex Count:"); ImGui::SameLine(); ImGui::Text("%u", mesh->vertices().size());
				ImGui::Text("Index Count:"); ImGui::SameLine(); ImGui::Text("%u", mesh->indices().size());

				ImGui::Separator();

				if (mesh->vao())
				{
					ImGui::PushID(("VAO##" + meshID).c_str());
					ImGui::Text("VAO");
					ImGui::BeginChildFrame(ImGui::GetID("vao"),
						{ 0, ImGui::GetTextLineHeightWithSpacing() * 2.5f },
						ImGuiWindowFlags_NoScrollbar |
						ImGuiWindowFlags_NoScrollWithMouse
					);
					{
						int32_t vao_handle { (int32_t)(uint32_t)mesh->vao() };
						ImGui::DragInt(("Handle##VAO##" + meshID).c_str(), &vao_handle);

						int32_t vao_mode { GL::index_of(mesh->vao().mode()) };
						ImGuiExt::Combo(("Mode##VAO##" + meshID).c_str(), &vao_mode, GL::Mode_names, ML_ARRAYSIZE(GL::Mode_names));
					}
					ImGui::EndChildFrame();
					ImGui::Separator();
					ImGui::PopID();
				}

				if (mesh->vbo())
				{
					ImGui::PushID(("VBO##" + meshID).c_str());
					ImGui::Text("VBO");
					ImGui::BeginChildFrame(ImGui::GetID("vbo"),
						{ 0, ImGui::GetTextLineHeightWithSpacing() * 5.0f },
						ImGuiWindowFlags_NoScrollbar |
						ImGuiWindowFlags_NoScrollWithMouse
					);
					{
						int32_t vbo_handle { (int32_t)(uint32_t)mesh->vbo() };
						ImGui::DragInt(("Handle##VBO##" + meshID).c_str(), &vbo_handle);

						int32_t vbo_size { (int32_t)mesh->vbo().size() };
						ImGui::DragInt(("Size##VBO##" + meshID).c_str(), &vbo_size);
						ImGuiExt::Tooltip("Total length of contiguous data");

						int32_t vbo_count { (int32_t)mesh->vbo().count() };
						ImGui::DragInt(("Count##VBO##" + meshID).c_str(), &vbo_count);
						ImGuiExt::Tooltip("Number of vertices");

						int32_t vbo_usage { GL::index_of(mesh->vbo().usage()) };
						ImGuiExt::Combo(("Usage##VBO##" + meshID).c_str(), &vbo_usage, GL::Usage_names, ML_ARRAYSIZE(GL::Usage_names));
					}
					ImGui::EndChildFrame();
					ImGui::Separator();
					ImGui::PopID();
				}

				if (mesh->ibo())
				{
					ImGui::PushID(("IBO##" + meshID).c_str());
					ImGui::Text("IBO");
					ImGui::BeginChildFrame(ImGui::GetID("ibo"),
						{ 0, ImGui::GetTextLineHeightWithSpacing() * 5.0f },
						ImGuiWindowFlags_NoScrollbar |
						ImGuiWindowFlags_NoScrollWithMouse
					);
					{
						int32_t ibo_handle { (int32_t)(uint32_t)mesh->ibo() };
						ImGui::DragInt(("Handle##IBO##" + meshID).c_str(), &ibo_handle);

						int32_t ibo_count { (int32_t)mesh->ibo().count() };
						ImGui::DragInt(("Count##IBO##" + meshID).c_str(), &ibo_count);

						int32_t ibo_usage { GL::index_of(mesh->ibo().usage()) };
						ImGuiExt::Combo(("Usage##IBO##" + meshID).c_str(), &ibo_usage, GL::Usage_names, ML_ARRAYSIZE(GL::Usage_names));

						int32_t ibo_type { GL::index_of(mesh->ibo().type()) };
						ImGuiExt::Combo(("Type##IBO##" + meshID).c_str(), &ibo_usage, GL::Type_names, ML_ARRAYSIZE(GL::Type_names));
					}
					ImGui::EndChildFrame();
					ImGui::Separator();
					ImGui::PopID();
				}

				if (!mesh->layout().elements().empty())
				{
					ImGui::PushID(("Layout" + meshID).c_str());
					ImGui::Text("Layout");
					ImGui::BeginChildFrame(ImGui::GetID("vertexattrib"),
						{ 0, ImGui::GetTextLineHeightWithSpacing() * mesh->layout().elements().size() },
						ImGuiWindowFlags_NoScrollbar |
						ImGuiWindowFlags_NoScrollWithMouse
					);
					for (const BufferLayout::Element & e : mesh->layout().elements())
					{
						ImGui::Text(
							"Index: %u | "
							"Size: %u | "
							"Type: %s | "
							"Normalize: %u | "
							"Stride: %u | "
							"Offset: %u | "
							"Width: %u",
							e.index, e.size, GL::name_of(e.type), e.normalize, e.stride, e.offset, e.width
						);
					}
					ImGui::EndChildFrame();
					ImGui::PopID();
				}
				ImGui::TreePop();
			}
			ImGui::PopID();
		}
		ImGui::PopID();
		return false;
	}


	// Script Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool PropertyDrawer<Script>::operator()(const String & label, const_pointer & value) const
	{
		Layout::begin_prop(this, label, value);
		const bool changed { Layout::select_combo<value_type>(label, value) };
		if (ImGui::IsItemHovered())
		{
		}
		const String menu_label { ("##SelectorMenu##{0}##{1}"_s).format(label, typeof<value_type>::name) };
		if (ImGui::BeginPopupContextItem(menu_label.c_str()))
		{
			if (Layout::inspect_button((label + menu_label), value))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
		Layout::end_prop(this);
		return changed;
	}

	bool PropertyDrawer<Script>::operator()(const String & label, pointer & value) const
	{
		// Popup
		const String button_label { ("{0}##NewButton##{1}"_s).format(label, typeof<value_type>::name.str()) };
		const String popup_label { ("Create {1}##{0}##Popup"_s).format(label, typeof<value_type>::name.str()) };
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
				std::strcpy(name, "new_script");
				std::strcpy(asset_path, "");
				copy = nullptr;
			}

			// Name
			ImGui::InputText(
				("Name##" + typeof<value_type>::name.str() + "##" + label).c_str(),
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
				("##Path##" + typeof<value_type>::name.str() + "##" + label).c_str(),
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

	bool PropertyDrawer<Script>::operator()(const String & label, reference value) const
	{
		bool changed { false };
		ImGui::PushID(ML_ADDRESSOF(&value));

		// Filename
		if (value.path())
		{
			ImGui::Text("%s", value.path().c_str());
		}

		// Execute
		if (ImGui::Button(("Execute##" + label).c_str()))
		{
			value.execute();
		}

		// Language
		int32_t lang { (int32_t)value.language() - 1 };
		bool lang_changed;
		if (lang_changed = ImGui::Combo(("Language##" + label).c_str(), &lang, "Lua\0Python"))
		{
			value.setLanguage((Script::Language)(lang + 1));
		}

		// Text
		using TextEditor = ImGui::TextEditor;
		using LanguageDefinition = TextEditor::LanguageDefinition;
		static HashMap<const_pointer, TextEditor> editors;
		auto edit { editors.find(&value) };
		if (edit == editors.end())
		{
			edit = editors.insert({ &value, TextEditor() }).first;
			edit->second.SetText(value.text());
			lang_changed = true;
			edit->second.SetShowWhitespaces(true);
		}
		if (lang_changed)
		{
			edit->second.SetLanguageDefinition((lang == 0
				? LanguageDefinition::Lua()
				: (lang == 1
					? LanguageDefinition::Python()
					: LanguageDefinition::CPlusPlus()
					)));
		}
		edit->second.Render(
			("ScriptEditor##" + label).c_str(), { 0, 0, }, true
		);
		if (edit->second.IsTextChanged())
		{
			value.setText(edit->second.GetText());
			changed = true;
		}
		ImGui::PopID();
		return changed;
	}


	// Shader Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool PropertyDrawer<Shader>::operator()(const String & label, const_pointer & value) const
	{
		Layout::begin_prop(this, label, value);
		const bool changed { Layout::select_combo<value_type>(label, value) };
		if (ImGui::IsItemHovered())
		{
		}
		const String menu_label { ("##SelectorMenu##{0}##{1}"_s).format(label, typeof<value_type>::name) };
		if (ImGui::BeginPopupContextItem(menu_label.c_str()))
		{
			if (Layout::inspect_button((label + menu_label), value))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
		Layout::end_prop(this);
		return changed;
	}
	
	bool PropertyDrawer<Shader>::operator()(const String & label, pointer & value) const
	{
		// Popup
		const String button_label { ("{0}##NewButton##{1}"_s).format(label, typeof<value_type>::name.str()) };
		const String popup_label { ("Create {1}##{0}##Popup"_s).format(label, typeof<value_type>::name.str()) };
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
				std::strcpy(name, "new_shader");
				std::strcpy(asset_path, "");
				copy = nullptr;
			}

			// Name
			ImGui::InputText(
				("Name##" + typeof<value_type>::name.str() + "##" + label).c_str(),
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
				("##Path##" + typeof<value_type>::name.str() + "##" + label).c_str(),
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

	bool PropertyDrawer<Shader>::operator()(const String & label, reference value) const
	{
		bool changed { false };
		ImGui::PushID(ML_ADDRESSOF(&value));
		if (ImGui::BeginTabBar("SourceTabs"))
		{
			auto draw_source_tab = [](
				const String & name, 
				const String & type, 
				const String & source)
			{
				if (!source) return;
				if (ImGui::BeginTabItem((type + "##ShaderTab##" + name).c_str()))
				{
					ImGui::TextUnformatted(&source[0], &source[source.size() - 1]);
					if (ImGui::BeginPopupContextItem((type + "##ShaderMenu##" + name).c_str()))
					{
						if (ImGui::Button("Copy to Clipboard"))
						{
							ML_Engine.window().setClipboardString(source);
							ImGui::CloseCurrentPopup();
						}
						ImGui::EndPopup();
					}
					ImGui::EndTabItem();
				}
			};

			draw_source_tab(label, "Vertex", value.sources().vs);
			draw_source_tab(label, "Fragment", value.sources().fs);
			draw_source_tab(label, "Geometry", value.sources().gs);

			ImGui::EndTabBar();

			/* * * * * * * * * * * * * * * * * * * * */
		}
		ImGui::PopID();
		return changed;
	}


	// Sound Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool PropertyDrawer<Sound>::operator()(const String & label, const_pointer & value) const
	{
		Layout::begin_prop(this, label, value);
		const bool changed { Layout::select_combo<value_type>(label, value) };
		if (ImGui::IsItemHovered())
		{
		}
		const String menu_label { ("##SelectorMenu##{0}##{1}"_s).format(label, typeof<value_type>::name) };
		if (ImGui::BeginPopupContextItem(menu_label.c_str()))
		{
			if (Layout::inspect_button((label + menu_label), value))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
		Layout::end_prop(this);
		return changed;
	}

	bool PropertyDrawer<Sound>::operator()(const String & label, pointer & value) const
	{
		// Popup
		const String button_label { ("{0}##NewButton##{1}"_s).format(label, typeof<value_type>::name.str()) };
		const String popup_label { ("Create {1}##{0}##Popup"_s).format(label, typeof<value_type>::name.str()) };
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

			// Popup Opened
			if (!popup_open && (popup_open = true))
			{
				std::strcpy(name, "new_sprite");
				std::strcpy(asset_path, "");
			}

			// Name
			ImGui::InputText(
				("Name##" + typeof<value_type>::name.str() + "##" + label).c_str(),
				name,
				ML_ARRAYSIZE(name)
			);

			// Path
			ImGui::InputText(
				("##Path##" + typeof<value_type>::name.str() + "##" + label).c_str(),
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
				}
			}

			// Submit / Cancel
			const bool submit { ImGui::Button("Submit") };
			ImGui::SameLine();
			const bool cancel { ImGui::Button("Cancel") };
			if (submit && !value)
			{
				if (ML_FS.fileExists(asset_path))
				{
					value = ML_Content.create<value_type>(name, asset_path);
				}
				else
				{
					value = ML_Content.create<value_type>(name);
				}
			}
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

	bool PropertyDrawer<Sound>::operator()(const String & label, reference value) const
	{
		return false;
	}


	// Sprite Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool PropertyDrawer<Sprite>::operator()(const String & label, const_pointer & value) const
	{
		Layout::begin_prop(this, label, value);
		const bool changed { Layout::select_combo<value_type>(label, value) };
		if (ImGui::IsItemHovered())
		{
		}
		const String menu_label { ("##SelectorMenu##{0}##{1}"_s).format(label, typeof<value_type>::name) };
		if (ImGui::BeginPopupContextItem(menu_label.c_str()))
		{
			if (Layout::inspect_button((label + menu_label), value))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
		Layout::end_prop(this);
		return changed;
	}

	bool PropertyDrawer<Sprite>::operator()(const String & label, pointer & value) const
	{
		// Popup
		const String button_label { ("{0}##NewButton##{1}"_s).format(label, typeof<value_type>::name.str()) };
		const String popup_label { ("Create {1}##{0}##Popup"_s).format(label, typeof<value_type>::name.str()) };
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
				std::strcpy(name, "new_sprite");
				std::strcpy(asset_path, "");
				copy = nullptr;
			}

			// Name
			ImGui::InputText(
				("Name##" + typeof<value_type>::name.str() + "##" + label).c_str(),
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
				("##Path##" + typeof<value_type>::name.str() + "##" + label).c_str(),
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

	bool PropertyDrawer<Sprite>::operator()(const String & label, reference value) const
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
	bool PropertyDrawer<Surface>::operator()(const String & label, const_pointer & value) const
	{
		Layout::begin_prop(this, label, value);
		const bool changed { Layout::select_combo<value_type>(label, value) };
		if (ImGui::IsItemHovered())
		{
		}
		const String menu_label { ("##SelectorMenu##{0}##{1}"_s).format(label, typeof<value_type>::name) };
		if (ImGui::BeginPopupContextItem(menu_label.c_str()))
		{
			if (Layout::inspect_button((label + menu_label), value))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
		Layout::end_prop(this);
		return changed;
	}

	bool PropertyDrawer<Surface>::operator()(const String & label, pointer & value) const
	{
		// Popup
		const String button_label { ("{0}##NewButton##{1}"_s).format(label, typeof<value_type>::name.str()) };
		const String popup_label { ("Create {1}##{0}##Popup"_s).format(label, typeof<value_type>::name.str()) };
		if (ImGui::Button(button_label.c_str()))
		{
			ImGui::OpenPopup(popup_label.c_str());
		}
		if (ImGui::BeginPopupModal(popup_label.c_str(), 0, ImGuiWindowFlags_AlwaysAutoResize))
		{
			// State
			static bool popup_open { false };
			static char name[32] = "";
			static const_pointer copy { nullptr };

			// Popup Opened
			if (!popup_open && (popup_open = true))
			{
				std::strcpy(name, "new_surface");
				copy = nullptr;
			}

			// Name
			ImGui::InputText(
				("Name##" + typeof<value_type>::name.str() + "##" + label).c_str(),
				name,
				ML_ARRAYSIZE(name)
			);

			// Copy
			if (self_type()(("Copy From##" + label), (const_pointer &)copy))
			{
			}

			// Submit
			const bool submit { ImGui::Button("Submit") };
			if (submit && !value)
			{
				if (copy)
				{
					value = ML_Content.create<value_type>(name, (*copy));
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

	bool PropertyDrawer<Surface>::operator()(const String & label, reference value) const
	{
		bool changed { false };

		ImGui::Columns(2);

		ImGui::Text("Handle: %u", (uint32_t)value.texture());

		ImGui::Text("Size: %i x %i", value.width(), value.height());

		int32_t colorID { GL::index_of(value.colorID()) };
		if (ImGuiExt::Combo(("Color Attachment##Surface##" + label).c_str(), 
			&colorID, GL::ColorID_names, ML_ARRAYSIZE(GL::ColorID_names)))
		{
			value.setColorID(GL::value_at<GL::ColorID>(colorID));
		}

		int32_t frameID { GL::index_of(value.frameID()) };
		if (ImGuiExt::Combo(("FB Attachment##Surface##" + label).c_str(),
			&frameID, GL::FrameID_names, ML_ARRAYSIZE(GL::FrameID_names)))
		{
			value.setFrameID(GL::value_at<GL::FrameID>(frameID));
		}

		const Material * material { value.material() };
		if (PropertyDrawer<Material>()(("Material##Surface##" + label), material))
		{
			value.setMaterial(material); changed = true;
		}

		const Model * model { value.model() };
		if (PropertyDrawer<Model>()(("Model##Surface##" + label), model))
		{
			value.setModel(model); changed = true;
		}

		ImGui::NextColumn();

		/* * * * * * * * * * * * * * * * * * * * */

		ML_AssetPreview.drawPreview(value, ImGuiExt::GetContentRegionAvail(), nullptr);

		/* * * * * * * * * * * * * * * * * * * * */

		ImGui::Columns(1);
		return changed;
	}

	
	// Texture Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool PropertyDrawer<Texture>::operator()(const String & label, const_pointer & value) const
	{
		Layout::begin_prop(this, label, value);
		const bool changed { Layout::select_combo<value_type>(label, value) };
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			switch (value->sampler())
			{
			case GL::Texture2D:
				if (auto preview { ML_AssetPreview.getPreview(value) })
				{
					const vec2 dst { 128, 128 };
					const vec2 scl { alg::scale_to_fit((vec2)preview->size(), dst) * 0.975f };
					const vec2 pos { ((dst - scl) * 0.5f) };
					ImGui::BeginChild(
						("##PropertyDrawer##Texture2D##Selector##Preview" + label).c_str(),
						{ dst[0], dst[1] }, true, ImGuiWindowFlags_NoScrollbar
					);
					ImGui::SetCursorPos({ pos[0], pos[1] });
					ImGui::Image(preview->get_address(), { scl[0], scl[1] }, { 0, 1 }, { 1, 0 });
					ImGui::EndChild();
				}
				break;
			
			case GL::Texture3D:
				ImGui::Text("Texture-3D previews are currently disabled.");
				break;
			
			case GL::TextureCubeMap:
				ImGui::Text("Texture-CubeMap previews are currently disabled.");
				break;
			}
			ImGui::EndTooltip();
		}
		const String menu_label { ("##SelectorMenu##{0}##{1}"_s).format(label, typeof<value_type>::name) };
		if (ImGui::BeginPopupContextItem(menu_label.c_str()))
		{
			if (Layout::inspect_button((label + menu_label), value))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
		Layout::end_prop(this);
		return changed;
	}

	bool PropertyDrawer<Texture>::operator()(const String & label, pointer & value) const
	{
		// Popup
		const String button_label { ("{0}##NewButton##{1}"_s).format(label, typeof<value_type>::name.str()) };
		const String popup_label { ("Create {1}##{0}##Popup"_s).format(label, typeof<value_type>::name.str()) };
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
				std::strcpy(name, "new_texture");
				std::strcpy(asset_path, "");
				for (auto *& e : image) e = nullptr;
				open_path.clear();
			}

			// Name
			ImGui::InputText(
				("Name##" + typeof<value_type>::name.str() + "##"+ label).c_str(),
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
					("##Path##" + typeof<value_type>::name.str() + "##" + label).c_str(),
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

	bool PropertyDrawer<Texture>::operator()(const String & label, reference value) const
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

		vec2u real_size { value.real_size() };
		ImGui::Text("Real Size: %u x %u", real_size[0], real_size[1]);

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

		/* * * * * * * * * * * * * * * * * * * * */

		ImGui::NextColumn();

		/* * * * * * * * * * * * * * * * * * * * */

		switch (value.sampler())
		{
		case GL::Texture2D:
		{
			ML_AssetPreview.drawPreview(value, ImGuiExt::GetContentRegionAvail(), nullptr);
		}
		break;
		case GL::Texture3D:
		{
			ImGui::TextWrapped("Texture-3D previews are currently disabled.");
		}
		break;
		case GL::TextureCubeMap:
		{
			ImGui::TextWrapped("Texture-CubeMap previews are currently disabled.");
		}
		break;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		ImGui::Columns(1);

		return changed;
	}


	// Uniform Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool PropertyDrawer<Uniform>::operator()(const String & label, const_pointer & value) const
	{
		Layout::begin_prop(this, label, value);
		const bool changed { Layout::select_combo<value_type>(label, value) };
		if (ImGui::IsItemHovered())
		{
		}
		const String menu_label { ("##SelectorMenu##{0}##{1}"_s).format(label, typeof<value_type>::name) };
		if (ImGui::BeginPopupContextItem(menu_label.c_str()))
		{
			if (Layout::inspect_button((label + menu_label), value))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
		Layout::end_prop(this);
		return changed;
	}

	bool PropertyDrawer<Uniform>::operator()(const String & label, pointer & value) const
	{
		// Popup
		const String button_label { ("{0}##NewButton##{1}"_s).format(label, typeof<value_type>::name.str()) };
		const String popup_label { ("Create {1}##{0}##Popup"_s).format(label, typeof<value_type>::name.str()) };
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
				("Name##" + typeof<value_type>::name.str() + "##"+ label).c_str(),
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
					case uni_bool	::ID: return (Uniform *)new uni_bool	{ name, {} };
					case uni_float	::ID: return (Uniform *)new uni_float	{ name, {} };
					case uni_int	::ID: return (Uniform *)new uni_int		{ name, {} };
					case uni_vec2	::ID: return (Uniform *)new uni_vec2	{ name, {} };
					case uni_vec3	::ID: return (Uniform *)new uni_vec3	{ name, {} };
					case uni_vec4	::ID: return (Uniform *)new uni_vec4	{ name, {} };
					case uni_color	::ID: return (Uniform *)new uni_color	{ name, {} };
					case uni_mat2	::ID: return (Uniform *)new uni_mat2	{ name, {} };
					case uni_mat3	::ID: return (Uniform *)new uni_mat3	{ name, {} };
					case uni_mat4	::ID: return (Uniform *)new uni_mat4	{ name, {} };
					case uni_sampler::ID: return (Uniform *)new uni_sampler	{ name, {} };
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

			//if (value && ML_BITREAD(flags, 0)) 
			//{ 
			//	ML_Content.insert<Uniform>(value->name, value); 
			//}

			return (submit || cancel);
		}
		return false;
	}

	bool PropertyDrawer<Uniform>::operator()(const String & label, reference value) const
	{
		constexpr float_t speed = 0.005f;
		switch (value.id)
		{
		case uni_bool::ID:
			if (auto u = detail::as_bool(&value))
			{
				const String name = "##" + label + "##Bool##Uni" + value.name;
				ImGui::Checkbox(name.c_str(), u);
				if (auto temp = value.as<uni_bool>())
				{
					temp->data = (*u);
					return true;
				}
			}
			break;

		case uni_float::ID:
			if (auto u = detail::as_float(&value))
			{
				const String name = "##" + label + "##Float##Uni" + value.name;
				ImGui::DragFloat(name.c_str(), u, speed);
				if (auto temp = value.as<uni_float>())
				{
					temp->data = (*u); 
					return true;
				}
			}
			break;

		case uni_int::ID:
			if (auto u = detail::as_int(&value))
			{
				const String name = "##" + label + "##Int##Uni" + value.name;
				
				if (value.isModifiable()) ImGui::InputInt(name.c_str(), u);
				else ImGui::DragInt(name.c_str(), u);
				
				if (auto temp = value.as<uni_int>())
				{
					temp->data = (*u);
					return true;
				}
			}
			break;

		case uni_vec2::ID:
			if (auto u = detail::as_vec2(&value))
			{
				const String name = "##" + label + "##Vec2##Uni" + value.name;
				ImGui::DragFloat2(name.c_str(), &(*u)[0], speed);
				if (auto temp = value.as<uni_vec2>())
				{
					temp->data = (*u); 
					return true;
				}
			}
			break;

		case uni_vec3::ID:
			if (auto u = detail::as_vec3(&value))
			{
				const String name = "##" + label + "##Vec3##Uni" + value.name;
				ImGui::DragFloat3(name.c_str(), &(*u)[0], speed);
				if (auto temp = value.as<uni_vec3>())
				{
					temp->data = (*u);
					return true;
				}
			}
			break;

		case uni_vec4::ID:
			if (auto u = detail::as_vec4(&value))
			{
				const String name = "##" + label + "##Vec4##Uni" + value.name;
				ImGui::DragFloat4(name.c_str(), &(*u)[0], speed);
				if (auto temp = value.as<uni_vec4>())
				{
					temp->data = (*u); 
					return true;
				}
			}
			break;

		case uni_color::ID:
			if (auto u = detail::as_color(&value))
			{
				const String name = "##" + label + "##Color##Uni" + value.name;
				ImGui::ColorEdit4(name.c_str(), &(*u)[0], ImGuiColorEditFlags_Float);
				if (auto temp = value.as<uni_color>())
				{
					temp->data = (*u); 
					return true;
				}
			}
			break;

		case uni_mat2::ID:
			if (mat2 * u = detail::as_mat2(&value))
			{
				const String name = "##" + label + "##Mat2##Uni" + value.name;
				ImGui::DragFloat2((name + "##00").c_str(), &(*u)[0], speed);
				ImGui::DragFloat2((name + "##02").c_str(), &(*u)[2], speed);
				if (auto temp = value.as<uni_mat2>())
				{
					temp->data = (*u);
					return true;
				}
			}
			break;

		case uni_mat3::ID:
			if (auto u = detail::as_mat3(&value))
			{
				const String name = "##" + label + "##Mat3##Uni" + value.name;
				ImGui::DragFloat3((name + "##00").c_str(), &(*u)[0], speed);
				ImGui::DragFloat3((name + "##03").c_str(), &(*u)[3], speed);
				ImGui::DragFloat3((name + "##06").c_str(), &(*u)[6], speed);
				if (auto temp = value.as<uni_mat3>())
				{
					temp->data = (*u); 
					return true;
				}
			}
			break;

		case uni_mat4::ID:
			if (auto u = detail::as_mat4(&value))
			{
				const String name = "##" + label + "##Mat4##Uni" + value.name;
				ImGui::DragFloat4((name + "##00").c_str(), &(*u)[0],  speed);
				ImGui::DragFloat4((name + "##04").c_str(), &(*u)[4],  speed);
				ImGui::DragFloat4((name + "##08").c_str(), &(*u)[8],  speed);
				ImGui::DragFloat4((name + "##12").c_str(), &(*u)[12], speed);
				if (auto temp = value.as<uni_mat4>())
				{
					temp->data = (*u); 
					return true;
				}
			}
			break;

		case uni_sampler::ID:
			if (auto u = value.as<uni_sampler>())
			{
				const String name = "##" + label + "##Sampler##Uni" + value.name;
				const Texture * temp { u->data };
				if (PropertyDrawer<Texture>()(name, temp)) 
				{
					u->data = temp; 
				}
				return true;
			}
			break;
		}
		ImGui::SameLine();
		ImGuiExt::HelpMarker("This uniform cannot be modified.");
		return false;
	}
}