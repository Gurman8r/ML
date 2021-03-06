#include <ML/Editor/PropertyDrawer.hpp>
#include <ML/Engine/ContentManager.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Editor/ImGuiExt.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/Rect.hpp>
#include <ML/Engine/Engine.hpp>
#include <ML/Editor/FileBrowser.hpp>
#include <ML/Audio/Sound.hpp>
#include <ML/Engine/Entity.hpp>
#include <ML/Graphics/Font.hpp>
#include <ML/Graphics/Material.hpp>
#include <ML/Graphics/Model.hpp>
#include <ML/Graphics/Sprite.hpp>
#include <ML/Graphics/RenderTexture.hpp>
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
		template <class T, class Value> static inline void begin_prop(
			const PropertyDrawer<T> * prop, String const & label, Value const &
		)
		{
			if (prop)
			{
				ImGui::PushID(ML_ADDRESSOF(prop));
				ImGui::PushID((int32_t)prop->info.hash);
				ImGui::PushID(label.c_str());
				ImGui::PushID((int32_t)typeof<Value>::hash());
			}
		}

		template <class T> static inline bool end_prop(const PropertyDrawer<T> * prop, bool changed)
		{
			if (prop)
			{
				ImGui::PopID();
				ImGui::PopID();
				ImGui::PopID();
				ImGui::PopID();
			}
			return changed;
		}

		template <class T>
		static inline bool select_combo(String const & label, T const *& value)
		{
			int32_t index { ML_Engine.content().get_index_of<T>(value) };
			return (ImGuiExt::Combo(label.c_str(), &index, ML_Engine.content().get_keys<T>())
				? (value = ML_Engine.content().find_by_index<T>(index))
				: false
			);
		}

		template <class T>
		static inline bool inspect_button(String const & label, T const * value)
		{
			ImGui::PushID((int32_t)typeof<T>::hash());
			ImGui::PushID(label.c_str());
			ImGui::PushID(value);
			const bool out { ImGui::Button(("Inspect##{0}##{1}"_s.format(
				label, typeof<T>::name()
			)).c_str()) };
			if (out)
			{
				ML_Editor.inspector().setFocused(true);
				ML_Editor.content().select_item(
					typeof<T>::name(), ML_Engine.content().get_name(value), (void *)value
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
	bool PropertyDrawer<Entity>::operator()(String const & label, const_pointer & value) const
	{
		Layout::begin_prop(this, label, value);

		const bool changed { Layout::select_combo<value_type>(label, value) };
		if (value && ImGui::IsItemHovered())
		{
		}
		const String menu_label { "##SelectorMenu##{0}##{1}"_s.format(label, this->info.brief) };
		if (ImGui::BeginPopupContextItem(menu_label.c_str()))
		{
			if (Layout::inspect_button((label + menu_label), value))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}

		return Layout::end_prop(this, changed);
	}

	bool PropertyDrawer<Entity>::operator()(String const & label, pointer & value) const
	{
		Layout::begin_prop(this, label, value);

		// Popup
		const String button_label { ("{0}##NewButton##{1}"_s).format(label, this->info.brief) };
		const String popup_label { ("Create {1}##{0}##Popup"_s).format(label, this->info.brief) };
		if (ImGui::Button(button_label.c_str()))
		{
			ImGui::OpenPopup(popup_label.c_str());
		}
		if (ImGui::BeginPopupModal(popup_label.c_str(), 0, ImGuiWindowFlags_AlwaysAutoResize))
		{
			// State
			static bool popup_open { false };
			static char name[32] = "";

			// Popup Opened01245001
			if (!popup_open && (popup_open = true))
			{
				std::strcpy(name, "new_entity");
			}

			// Name
			ImGui::InputText(
				("Name##" + this->info.brief.str() + "##" + label).c_str(),
				name,
				ML_ARRAYSIZE(name)
			);

			// Submit / Cancel
			const bool submit { ImGui::Button("Submit") };
			ImGui::SameLine();
			const bool cancel { ImGui::Button("Cancel") };
			if (submit && !value)
			{
				value = ML_Engine.content().create<value_type>(name);
			}
			if (submit || cancel)
			{
				popup_open = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();

			return Layout::end_prop(this, submit || cancel);
		}
		return Layout::end_prop(this, false);
	}

	bool PropertyDrawer<Entity>::operator()(String const & label, reference value) const
	{
		Layout::begin_prop(this, label, value);

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
			for (auto const & [ name, func ] : ML_Registry.funcs())
			{
				// Skip
				if (!filter.PassFilter(name.c_str())) continue;
				String const * info { ML_Registry.get_info(name) };
				if (info && *info != "Component") continue;

				// Selectable
				if (ImGui::Selectable((name + "##AddComponentMenuButton").c_str()))
				{
					if (hash_t const * code { ML_Registry.get_code(name) })
					{
						Trackable * temp { ML_Registry.generate(name) };
						if (!value.add(*code, temp))
						{
							Debug::logError("Failed Creating \'{0}\'", name);
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
		if (auto t{ value.get<Transform>() })
		{
			ImGui::PushID(ML_ADDRESSOF(t));
			const bool header_open{ ImGui::CollapsingHeader("Transform") };
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
				PropertyDrawer<Transform>()(label, *t);
			}
			ImGui::PopID();
		}

		// Camera
		if (auto c{ value.get<Camera>() })
		{
			ImGui::PushID(ML_ADDRESSOF(c));
			const bool header_open{ ImGui::CollapsingHeader("Camera") };
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
				PropertyDrawer<Camera>()(label, *c);
			}
			ImGui::PopID();
		}

		// Light
		if (auto l{ value.get<Light>() })
		{
			ImGui::PushID(ML_ADDRESSOF(l));
			const bool header_open{ ImGui::CollapsingHeader("Light (WIP)") };
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
				PropertyDrawer<Light>()(label, *l);
			}
			ImGui::PopID();
		}

		// Renderer
		if (auto r{ value.get<Renderer>() })
		{
			ImGui::PushID(ML_ADDRESSOF(r));
			const bool header_open{ ImGui::CollapsingHeader("Renderer") };
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
				PropertyDrawer<Renderer>()(label, *r);
			}
			ImGui::PopID();
		}

		return Layout::end_prop(this, false);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	bool PropertyDrawer<Camera>::operator()(String const & label, reference value) const
	{
		constexpr float_t speed{ 0.005f };

		Layout::begin_prop(this, label, value);

		bool	enabled = value.enabled();
		int32_t clearFlags = value.clearFlags();
		int32_t projection = value.projection();
		vec3	position = value.position();
		vec3	forward = value.direction();
		vec4	background = value.background();
		float_t fieldOfView = value.fieldOfView();
		float_t clipNear = value.clipNear();
		float_t clipFar = value.clipFar();
		vec2	viewport = (vec2)value.viewport().size();

		/* * * * * * * * * * * * * * * * * * * * */

		ImGui::BeginChildFrame(
			ImGui::GetID(("##Camera##" + label).c_str()),
			{ 0, (ImGui::GetTextLineHeightWithSpacing() * 1.25f) * 10.0f },
			true
		);

		// Enabled
		if (ImGui::Checkbox(("Enabled##Camera##" + label).c_str(), &enabled))
		{
			value.setEnabled(enabled);
		}
		ImGuiExt::Tooltip("If enabled, the camera be applied.");

		// Clear Flags
		if (ImGuiExt::Combo(
			("Clear Flags##Camera##" + label).c_str(),
			&clearFlags,
			"Solid Color\0Depth Only\0Don't Clear"
		))
		{
			value.setClearFlags((Camera::ClearFlags)clearFlags);
		}
		ImGuiExt::Tooltip("Specify how the screen should be cleared.");

		// Projection
		if (ImGuiExt::Combo(
			("Projection##Camera##" + label).c_str(),
			&projection,
			"Perspective\0"
		))
		{
			value.setProjection((Camera::Projection)projection);
		}
		ImGuiExt::Tooltip("Specify which projection to use.");

		// Background
		if (ImGui::ColorEdit4(("Background##Camera##" + label).c_str(), &background[0]))
		{
			value.setBackground(background);
		}
		ImGuiExt::Tooltip("Specify the color to apply when using \'Solid Color\'.");

		ImGui::Separator();

		// Position
		if (ImGui::DragFloat3(("Position##Camera##" + label).c_str(), &position[0], speed))
		{
			value.setPosition(position);
		}
		ImGuiExt::Tooltip("Set the position of the camera.");

		// Direction
		if (ImGui::DragFloat3(("Direction##Camera##" + label).c_str(), &forward[0], speed))
		{
			value.setDirection(forward);
		}
		ImGuiExt::Tooltip("Set the direction the camera is facing.");

		ImGui::Separator();

		// Field of View
		if (ImGui::DragFloat(("Field of View##Camera##" + label).c_str(), &fieldOfView, speed))
		{
			value.setFieldOfView(fieldOfView);
		}
		ImGuiExt::Tooltip("Specify the field of view.");

		// Clip Near
		if (ImGui::DragFloat(("Clip Near##Camera##" + label).c_str(), &clipNear, speed))
		{
			value.setClipNear(clipNear);
		}
		ImGuiExt::Tooltip("Specify the near clipping plane.");

		// Clip Far
		if (ImGui::DragFloat(("Clip Far##Camera##" + label).c_str(), &clipFar, speed))
		{
			value.setClipFar(clipFar);
		}
		ImGuiExt::Tooltip("Specify the far clipping plane.");

		// Viewport
		if (ImGui::DragFloat2(("Viewport##Camera##" + label).c_str(), &viewport[0], speed))
		{
			if (viewport[0] <= 0.f) viewport[0] = FLT_MIN;
			if (viewport[1] <= 0.f) viewport[1] = FLT_MIN;
			value.setViewport((vec2i)viewport);
		}
		ImGuiExt::Tooltip("Specify the viewport size.");

		ImGui::EndChildFrame();
		return Layout::end_prop(this, false);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	bool PropertyDrawer<Light>::operator()(String const & label, reference value) const
	{
		Layout::begin_prop(this, label, value);

		bool enabled = value.enabled();
		vec4 color = value.color();
		float_t intensity = value.intensity();
		int32_t mode = value.mode();

		/* * * * * * * * * * * * * * * * * * * * */

		ImGui::BeginChildFrame(
			ImGui::GetID(("##Light##" + label).c_str()),
			{ 0, (ImGui::GetTextLineHeightWithSpacing() * 1.25f) * 4.0f },
			true
		);

		if (ImGui::Checkbox(("Enabled##Light##" + label).c_str(), &enabled))
		{
			value.setEnabled(enabled);
		}

		if (ImGui::ColorEdit4(("Color##Light##" + label).c_str(), &color[0]))
		{
			value.setColor(color);
		}

		if (ImGui::DragFloat(("Intensity##Light##" + label).c_str(), &intensity))
		{
			value.setIntensity(intensity);
		}

		if (ImGuiExt::Combo(("Mode##Light##" + label).c_str(), &mode, "Realtime"))
		{
			value.setMode((Light::Mode)mode);
		}

		ImGui::EndChildFrame();

		return Layout::end_prop(this, false);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	bool PropertyDrawer<Renderer>::operator()(String const & label, reference value) const
	{
		Layout::begin_prop(this, label, value);

		ImGui::BeginChildFrame(
			ImGui::GetID(("##Renderer##" + label).c_str()),
			{ 0, (ImGui::GetTextLineHeightWithSpacing() * 1.25f) * 8.f },
			true
		);

		/* * * * * * * * * * * * * * * * * * * * */

		bool enabled{ value.enabled() };
		if (ImGui::Checkbox(("Enabled##Renderer##" + label).c_str(), &enabled))
		{
			value.setEnabled(enabled);
		}

		Material const * material{ value.material() };
		if (PropertyDrawer<Material>()("Material##Renderer", material))
		{
			value.setMaterial(material);
		}

		Shader const * shader{ value.shader() };
		if (PropertyDrawer<Shader>()("Shader##Renderer", shader))
		{
			value.setShader(shader);
		}

		Model const * model{ value.model() };
		if (PropertyDrawer<Model>()("Model##Renderer", model))
		{
			value.setModel(model);
		}

		/* * * * * * * * * * * * * * * * * * * * */

		if (ImGui::TreeNode("Alpha"))
		{
			ImGui::Checkbox("Enabled##AlphaState", &value.states().alpha().enabled);

			int32_t index = GL::index_of(value.states().alpha().func);
			if (ImGuiExt::Combo(
				"Comparison##Alpha Testing",
				&index,
				GL::Predicate_names,
				ML_ARRAYSIZE(GL::Predicate_names)
			))
			{
				GL::value_at(index, value.states().alpha().func);
			}
			ImGui::DragFloat("Coeff##AlphaState", &value.states().alpha().coeff);

			ImGui::TreePop();
		}

		/* * * * * * * * * * * * * * * * * * * * */

		if (ImGui::TreeNode("Blend"))
		{
			ImGui::Checkbox("Enabled##BlendState", &value.states().blend().enabled);

			auto factor_combo = [](C_String label, int32_t & index)
			{
				return ImGuiExt::Combo(
					label,
					&index,
					GL::Factor_names,
					ML_ARRAYSIZE(GL::Factor_names)
				);
			};

			int32_t sfactorRGB = GL::index_of(value.states().blend().sfactorRGB);
			if (factor_combo("Src RGB##BlendState", sfactorRGB))
			{
				GL::value_at(sfactorRGB, value.states().blend().sfactorRGB);
			}

			int32_t sfactorAlpha = GL::index_of(value.states().blend().sfactorAlpha);
			if (factor_combo("Src Alpha##BlendState", sfactorAlpha))
			{
				GL::value_at(sfactorAlpha, value.states().blend().sfactorAlpha);
			}

			int32_t dfactorRGB = GL::index_of(value.states().blend().dfactorRGB);
			if (factor_combo("Dst RGB##BlendState", dfactorRGB))
			{
				GL::value_at(dfactorRGB, value.states().blend().dfactorRGB);
			}

			int32_t dfactorAlpha = GL::index_of(value.states().blend().dfactorAlpha);
			if (factor_combo("Dst Alpha##BlendState", dfactorAlpha))
			{
				GL::value_at(dfactorAlpha, value.states().blend().dfactorAlpha);
			}
			ImGui::TreePop();
		}

		/* * * * * * * * * * * * * * * * * * * * */

		if (ImGui::TreeNode("Cull"))
		{
			ImGui::Checkbox("Enabled##CullState", &value.states().cull().enabled);

			int32_t index = GL::index_of(value.states().cull().mode);
			if (ImGuiExt::Combo(
				"Face##Cull",
				&index,
				GL::Face_names,
				ML_ARRAYSIZE(GL::Face_names)
			))
			{
				GL::value_at(index, value.states().cull().mode);
			}

			ImGui::TreePop();
		}

		/* * * * * * * * * * * * * * * * * * * * */

		if (ImGui::TreeNode("Depth"))
		{
			ImGui::Checkbox("Enabled##DepthState", &value.states().depth().enabled);

			ImGui::Checkbox("Mask##DepthState", &value.states().depth().mask);

			int32_t index = GL::index_of(value.states().depth().func);
			if (ImGuiExt::Combo(
				"Comparison##Depth",
				&index,
				GL::Predicate_names,
				ML_ARRAYSIZE(GL::Predicate_names)
			))
			{
				GL::value_at(index, value.states().depth().func);
			}

			ImGui::TreePop();
		}

		ImGui::EndChildFrame();

		/* * * * * * * * * * * * * * * * * * * * */

		return Layout::end_prop(this, false);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	bool PropertyDrawer<Transform>::operator()(String const & label, reference value) const
	{
		Layout::begin_prop(this, label, value);

		/* * * * * * * * * * * * * * * * * * * * */

		constexpr float_t speed{ 0.005f };

		bool enabled = value.enabled();
		vec3 pos = value.position();
		vec3 scl = value.scale();
		vec4 rot = value.rotation();

		/* * * * * * * * * * * * * * * * * * * * */

		ImGui::BeginChildFrame(
			ImGui::GetID(("##Transform##" + label).c_str()),
			{ 0, (ImGui::GetTextLineHeightWithSpacing() * 1.25f) * 4.0f },
			true
		);

		if (ImGui::Checkbox(("Enabled##Transform##" + label).c_str(), &enabled))
		{
			value.setEnabled(enabled);
		}

		if (ImGui::DragFloat3(("Position##Transform##" + label).c_str(), &pos[0], speed))
		{
			value.setPosition(pos);
		}

		if (ImGui::DragFloat3(("Scale##Transform##" + label).c_str(), &scl[0], speed))
		{
			value.setScale(scl);
		}

		if (ImGui::DragFloat4(("Rotation##Transform##" + label).c_str(), &rot[0], speed))
		{
			value.setRotation(rot);
		}

		ImGui::EndChildFrame();

		/* * * * * * * * * * * * * * * * * * * * */

		return Layout::end_prop(this, false);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


	// Font Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool PropertyDrawer<Font>::operator()(String const & label, const_pointer & value) const
	{
		Layout::begin_prop(this, label, value);

		const bool changed { Layout::select_combo<value_type>(label, value) };
		if (value && ImGui::IsItemHovered())
		{
		}
		const String menu_label { "##SelectorMenu##{0}##{1}"_s.format(label, this->info.brief) };
		if (ImGui::BeginPopupContextItem(menu_label.c_str()))
		{
			if (Layout::inspect_button((label + menu_label), value))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}

		return Layout::end_prop(this, changed);
	}

	bool PropertyDrawer<Font>::operator()(String const & label, pointer & value) const
	{
		Layout::begin_prop(this, label, value);

		// Popup
		const String button_label { ("{0}##NewButton##{1}"_s).format(label, this->info.brief) };
		const String popup_label { ("Create {1}##{0}##Popup"_s).format(label, this->info.brief) };
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
				("Name##" + this->info.brief.str() + "##" + label).c_str(),
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
				("##Path##" + this->info.brief.str() + "##" + label).c_str(),
				asset_path,
				ML_MAX_PATH
			);
			ImGui::SameLine();
			static String open_path;
			if (ImGuiExt::OpenFile(("Browse##" + label), open_path, { 1280, 720 }) == 1)
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
					value = ML_Engine.content().create<value_type>(name, asset_path);
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

			return Layout::end_prop(this, submit || cancel);
		}
		return Layout::end_prop(this, false);
	}

	bool PropertyDrawer<Font>::operator()(String const & label, reference value) const
	{
		Layout::begin_prop(this, label, value);

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
		auto draw_glyph = [](Glyph const & g) 
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

		return Layout::end_prop(this, false);
	}


	// Image Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool PropertyDrawer<Image>::operator()(String const & label, const_pointer & value) const
	{
		Layout::begin_prop(this, label, value);

		const bool changed { Layout::select_combo<value_type>(label, value) };
		if (value && ImGui::IsItemHovered())
		{
		}
		const String menu_label { "##SelectorMenu##{0}##{1}"_s.format(label, this->info.brief) };
		if (ImGui::BeginPopupContextItem(menu_label.c_str()))
		{
			if (Layout::inspect_button((label + menu_label), value))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}

		return Layout::end_prop(this, changed);
	}

	bool PropertyDrawer<Image>::operator()(String const & label, pointer & value) const
	{
		Layout::begin_prop(this, label, value);

		// Popup
		const String button_label { ("{0}##NewButton##{1}"_s).format(label, this->info.brief) };
		const String popup_label { ("Create {1}##{0}##Popup"_s).format(label, this->info.brief) };
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
				("Name##" + this->info.brief.str() + "##" + label).c_str(),
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
				("##Path##" + this->info.brief.str() + "##" + label).c_str(),
				asset_path,
				ML_MAX_PATH
			);
			ImGui::SameLine();
			static String open_path;
			if (ImGuiExt::OpenFile(("Browse##" + label), open_path, { 1280, 720 }) == 1)
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
					value = ML_Engine.content().create<value_type>(name, (*copy));
				}
				else if (ML_FS.fileExists(asset_path))
				{
					value = ML_Engine.content().create<value_type>(name, asset_path);
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

			return Layout::end_prop(this, submit || cancel);
		}
		return Layout::end_prop(this, false);
	}

	bool PropertyDrawer<Image>::operator()(String const & label, reference value) const
	{
		Layout::begin_prop(this, label, value);

		bool changed { false };

		ImGui::Text("Size: %i x %i", value.width(), value.height());
		
		ImGui::Text("Channels: %i", value.channels());

		ML_Editor.previews().drawPreview(value, ImGuiExt::GetContentRegionAvail(), nullptr);

		return Layout::end_prop(this, changed);
	}


	// Material Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool PropertyDrawer<Material>::operator()(String const & label, const_pointer & value) const
	{
		Layout::begin_prop(this, label, value);

		const bool changed { Layout::select_combo<value_type>(label, value) };
		if (value && ImGui::IsItemHovered())
		{
		}
		const String menu_label { "##SelectorMenu##{0}##{1}"_s.format(label, this->info.brief) };
		if (ImGui::BeginPopupContextItem(menu_label.c_str()))
		{
			if (Layout::inspect_button((label + menu_label), value))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}

		return Layout::end_prop(this, changed);
	}

	bool PropertyDrawer<Material>::operator()(String const & label, pointer & value) const
	{
		Layout::begin_prop(this, label, value);

		// Popup
		const String button_label { ("{0}##NewButton##{1}"_s).format(label, this->info.brief) };
		const String popup_label { ("Create {1}##{0}##Popup"_s).format(label, this->info.brief) };
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
			static Material const * copy { nullptr };
			static char asset_path[ML_MAX_PATH] = "";

			// Popup Opened
			if (!popup_open && (popup_open = true))
			{
				std::strcpy(name, "new_material");
				std::strcpy(asset_path, "");
				globals = false;
				copy = nullptr;
			}

			// Name
			ImGui::InputText(
				("Name##" + this->info.brief.str() + "##"+ label).c_str(),
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
			PropertyDrawer<Material>()(("Copy Uniforms##" + label), (Material const *&)copy);
			ImGuiExt::Tooltip("Select an existing material to copy uniforms from");

			// Path
			ImGui::InputText(
				("##Path##" + this->info.brief.str() + "##" + label).c_str(),
				asset_path,
				ML_MAX_PATH
			);
			ImGui::SameLine();
			static String open_path;
			if (ImGuiExt::OpenFile(("Browse##" + label), open_path, { 1280, 720 }) == 1)
			{
				if (ML_FS.fileExists(open_path))
				{
					std::strcpy(asset_path, open_path.c_str());
					open_path.clear();
				}
			}
			ImGuiExt::Tooltip("Select a file to read uniforms from");

			// Submit
			const bool submit { ImGui::Button("Submit") };
			if (submit && !value)
			{
				if (value = ML_Engine.content().create<Material>(name))
				{
					if (globals)
					{
						for (auto const & pair : ML_Engine.content().data<Uniform>())
						{
							if (auto u { (Uniform const *)pair.second })
							{
								value->insert(u->clone());
							}
						}
					}
					if (copy)
					{
						for (auto const & u : (*copy))
						{
							if (!value->get(u->name())) { value->insert(u->clone()); }
						}
					}
					if (ML_FS.fileExists(asset_path))
					{
						value->loadFromFile(
							asset_path, 
							(const std::map<String, Texture *> *) & ML_Engine.content().data<Texture>()
						);
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

			return Layout::end_prop(this, submit || cancel);
		}
		return Layout::end_prop(this, false);
	}

	bool PropertyDrawer<Material>::operator()(String const & label, reference value) const
	{
		Layout::begin_prop(this, label, value);

		ImGui::PushID(label.c_str());

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
		if (!value.empty()) { ImGui::Separator(); }

		// to remove
		Uniform * to_remove{ nullptr };
		for (auto & u : value)
		{
			if (!u) continue;

			// name
			const String name("##Uni##" + u->name() + "##Material##" + label);

			// Uniform Header
			ImGui::PushStyleColor(
				ImGuiCol_Header,
				{ 0.367f, 0.258f, 0.489f, 0.580f }
			);

			if (ImGui::TreeNode((u->name() + name).c_str()))
			{
				ImGui::PopStyleColor();
				if (u)
				{
					ImGui::PushID(name.c_str());
					const bool canEdit{ PropertyDrawer<Uniform>()(name, (Uniform &)(*u)) };
					ImGui::SameLine();
					if (canEdit)
					{
						if (ImGui::Button(("Delete##" + name).c_str()))
						{ 
							to_remove = u;
						}
					}
					else
					{
						ImGuiExt::HelpMarker("This uniform cannot be modified.");
					}
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
		value.erase(to_remove);
		ImGui::PopID();
		return Layout::end_prop(this, false);
	}


	// Model Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool PropertyDrawer<Model>::operator()(String const & label, const_pointer & value) const
	{
		Layout::begin_prop(this, label, value);

		const bool changed { Layout::select_combo<value_type>(label, value) };
		if (value && ImGui::IsItemHovered())
		{
		}
		const String menu_label { "##SelectorMenu##{0}##{1}"_s.format(label, this->info.brief) };
		if (ImGui::BeginPopupContextItem(menu_label.c_str()))
		{
			if (Layout::inspect_button((label + menu_label), value))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}

		return Layout::end_prop(this, changed);
	}

	bool PropertyDrawer<Model>::operator()(String const & label, pointer & value) const
	{
		Layout::begin_prop(this, label, value);

		// Popup
		const String button_label { ("{0}##NewButton##{1}"_s).format(label, this->info.brief) };
		const String popup_label { ("Create {1}##{0}##Popup"_s).format(label, this->info.brief) };
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
				("Name##" + this->info.brief.str() + "##" + label).c_str(),
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
				("##Path##" + this->info.brief.str() + "##" + label).c_str(),
				asset_path,
				ML_MAX_PATH
			);
			ImGui::SameLine();
			static String open_path;
			if (ImGuiExt::OpenFile(("Browse##" + label), open_path, { 1280, 720 }) == 1)
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
					value = ML_Engine.content().create<value_type>(name, (*copy));
				}
				else if (ML_FS.fileExists(asset_path))
				{
					value = ML_Engine.content().create<value_type>(name, asset_path);
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

			return Layout::end_prop(this, submit || cancel);
		}
		return Layout::end_prop(this, false);
	}

	bool PropertyDrawer<Model>::operator()(String const & label, reference value) const
	{
		Layout::begin_prop(this, label, value);

		ImGui::PushID(ML_ADDRESSOF(&value));
		ImGui::Text("Meshes: %u", value.meshes().size());
		for (size_t i = 0; i < value.meshes().size(); i++)
		{
			Mesh const * mesh { value.meshes()[i] };
			ImGui::PushID(ML_ADDRESSOF(mesh));

			const String meshID { String("Mesh [" + std::to_string(i) + "]##" + label) };
			if (ImGui::TreeNode(meshID.c_str()))
			{
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

				if (mesh->layout())
				{
					ImGui::PushID(("Layout" + meshID).c_str());
					ImGui::Text("Layout");
					ImGui::BeginChildFrame(ImGui::GetID("vertexattrib"),
						{ 0, ImGui::GetTextLineHeightWithSpacing() * mesh->layout().size() },
						ImGuiWindowFlags_NoScrollbar |
						ImGuiWindowFlags_NoScrollWithMouse
					);
					for (auto const & e : mesh->layout())
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
		return Layout::end_prop(this, false);
	}


	// RenderTexture Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool PropertyDrawer<RenderTexture>::operator()(String const & label, const_pointer & value) const
	{
		Layout::begin_prop(this, label, value);

		const bool changed{ Layout::select_combo<value_type>(label, value) };
		if (value && ImGui::IsItemHovered())
		{
		}
		const String menu_label{ "##SelectorMenu##{0}##{1}"_s.format(label, this->info.brief) };
		if (ImGui::BeginPopupContextItem(menu_label.c_str()))
		{
			if (Layout::inspect_button((label + menu_label), value))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}

		return Layout::end_prop(this, changed);
	}

	bool PropertyDrawer<RenderTexture>::operator()(String const & label, pointer & value) const
	{
		Layout::begin_prop(this, label, value);

		// Popup
		const String button_label{ ("{0}##NewButton##{1}"_s).format(label, this->info.brief) };
		const String popup_label{ ("Create {1}##{0}##Popup"_s).format(label, this->info.brief) };
		if (ImGui::Button(button_label.c_str()))
		{
			ImGui::OpenPopup(popup_label.c_str());
		}
		if (ImGui::BeginPopupModal(popup_label.c_str(), 0, ImGuiWindowFlags_AlwaysAutoResize))
		{
			// State
			static bool popup_open{ false };
			static char name[32] = "";
			static const_pointer copy{ nullptr };

			// Popup Opened
			if (!popup_open && (popup_open = true))
			{
				std::strcpy(name, "new_surface");
				copy = nullptr;
			}

			// Name
			ImGui::InputText(
				("Name##" + this->info.brief.str() + "##" + label).c_str(),
				name,
				ML_ARRAYSIZE(name)
			);

			// Copy
			if (self_type()(("Copy From##" + label), (const_pointer &)copy))
			{
			}

			// Submit
			const bool submit{ ImGui::Button("Submit") };
			if (submit && !value)
			{
				if (copy)
				{
					value = ML_Engine.content().create<value_type>(name, (*copy));
				}
				else
				{
					value = ML_Engine.content().create<value_type>(name);
				}
			}
			ImGui::SameLine();

			// Cancel / Popup Closed
			const bool cancel{ ImGui::Button("Cancel") };
			if (submit || cancel)
			{
				popup_open = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();

			return Layout::end_prop(this, submit || cancel);
		}
		return Layout::end_prop(this, false);
	}

	bool PropertyDrawer<RenderTexture>::operator()(String const & label, reference value) const
	{
		Layout::begin_prop(this, label, value);

		bool changed{ false };

		ImGui::Columns(2);

		ImGui::Text("Handle: %u", (uint32_t)value.texture());

		ImGui::Text("Size: %i x %i", value.width(), value.height());

		int32_t colorID{ GL::index_of(value.colorID()) };
		if (ImGuiExt::Combo(("Color Attachment##RenderTexture##" + label).c_str(),
			&colorID, GL::ColorID_names, ML_ARRAYSIZE(GL::ColorID_names)))
		{
			value.setColorID(GL::value_at<GL::ColorID>(colorID));
		}

		int32_t frameID{ GL::index_of(value.frameID()) };
		if (ImGuiExt::Combo(("Framebuffer Attachment##RenderTexture##" + label).c_str(),
			&frameID, GL::FrameID_names, ML_ARRAYSIZE(GL::FrameID_names)))
		{
			value.setFrameID(GL::value_at<GL::FrameID>(frameID));
		}

		Material const * material{ value.material() };
		if (PropertyDrawer<Material>()(("Material##RenderTexture##" + label), material))
		{
			value.setMaterial(material); changed = true;
		}

		Model const * model{ value.model() };
		if (PropertyDrawer<Model>()(("Model##RenderTexture##" + label), model))
		{
			value.setModel(model); changed = true;
		}

		Shader const * shader{ value.shader() };
		if (PropertyDrawer<Shader>()(("Shader##RenderTexture##" + label), shader))
		{
			value.setShader(shader); changed = true;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		ImGui::NewLine();

		/* * * * * * * * * * * * * * * * * * * * */

		bool smooth{ value.texture().smooth() };
		if (ImGui::Checkbox(("Smooth##" + label).c_str(), &smooth))
		{
		}

		/* * * * * * * * * * * * * * * * * * * * */

		bool repeated{ value.texture().repeated() };
		if (ImGui::Checkbox(("Repeated##" + label).c_str(), &repeated))
		{
		}

		/* * * * * * * * * * * * * * * * * * * * */

		bool mipmapped{ value.texture().mipmapped() };
		if (ImGui::Button(((mipmapped
			? "Invalidate Mipmap##" : "Generate Mipmap##") + label).c_str()
		))
		{
		}

		/* * * * * * * * * * * * * * * * * * * * */

		int32_t level{ value.texture().level() };
		if (ImGui::InputInt(("Level##" + label).c_str(), &level))
		{
		}

		/* * * * * * * * * * * * * * * * * * * * */

		int32_t target{ GL::index_of(value.texture().sampler()) };
		if (ImGuiExt::Combo(
			("Sampler##" + label).c_str(),
			&(target),
			GL::Sampler_names,
			ML_ARRAYSIZE(GL::Sampler_names)
		))
		{
		}

		/* * * * * * * * * * * * * * * * * * * * */

		int32_t colorFormat{ GL::index_of(value.texture().color_fmt()) };
		if (ImGuiExt::Combo(
			("Color Format##" + label).c_str(),
			&(colorFormat),
			GL::Format_names,
			ML_ARRAYSIZE(GL::Format_names)
		))
		{
		}

		/* * * * * * * * * * * * * * * * * * * * */

		int32_t internalFormat{ GL::index_of(value.texture().internal_fmt()) };
		if (ImGuiExt::Combo(
			("Internal Format##" + label).c_str(),
			&(internalFormat),
			GL::Format_names,
			ML_ARRAYSIZE(GL::Format_names)
		))
		{
		}

		/* * * * * * * * * * * * * * * * * * * * */

		int32_t pixelType{ GL::index_of(value.texture().pixel_type()) };
		if (ImGuiExt::Combo(
			("Pixel Type##" + label).c_str(),
			&(pixelType),
			GL::Type_names,
			ML_ARRAYSIZE(GL::Type_names)
		))
		{
		}

		ImGui::NextColumn();

		/* * * * * * * * * * * * * * * * * * * * */

		ML_Editor.previews().drawPreview(value, ImGuiExt::GetContentRegionAvail(), nullptr);

		/* * * * * * * * * * * * * * * * * * * * */

		ImGui::Columns(1);
		return Layout::end_prop(this, changed);
	}


	// Script Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool PropertyDrawer<Script>::operator()(String const & label, const_pointer & value) const
	{
		Layout::begin_prop(this, label, value);

		const bool changed { Layout::select_combo<value_type>(label, value) };
		if (value && ImGui::IsItemHovered())
		{
		}
		const String menu_label { "##SelectorMenu##{0}##{1}"_s.format(label, this->info.brief) };
		if (ImGui::BeginPopupContextItem(menu_label.c_str()))
		{
			if (Layout::inspect_button((label + menu_label), value))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}

		return Layout::end_prop(this, changed);
	}

	bool PropertyDrawer<Script>::operator()(String const & label, pointer & value) const
	{
		Layout::begin_prop(this, label, value);

		// Popup
		const String button_label { ("{0}##NewButton##{1}"_s).format(label, this->info.brief) };
		const String popup_label { ("Create {1}##{0}##Popup"_s).format(label, this->info.brief) };
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
				("Name##" + this->info.brief.str() + "##" + label).c_str(),
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
				("##Path##" + this->info.brief.str() + "##" + label).c_str(),
				asset_path,
				ML_MAX_PATH
			);
			ImGui::SameLine();
			static String open_path;
			if (ImGuiExt::OpenFile(("Browse##" + label), open_path, { 1280, 720 }) == 1)
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
					value = ML_Engine.content().create<value_type>(name, (*copy));
				}
				else if (ML_FS.fileExists(asset_path))
				{
					value = ML_Engine.content().create<value_type>(name, asset_path);
				}
				else
				{
					value = ML_Engine.content().create<value_type>(name);
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

			return Layout::end_prop(this, submit || cancel);
		}
		return Layout::end_prop(this, false);
	}

	bool PropertyDrawer<Script>::operator()(String const & label, reference value) const
	{
		Layout::begin_prop(this, label, value);

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
		static std::unordered_map<const_pointer, TextEditor> editors;
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
		return Layout::end_prop(this, changed);
	}


	// Shader Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool PropertyDrawer<Shader>::operator()(String const & label, const_pointer & value) const
	{
		Layout::begin_prop(this, label, value);

		const bool changed { Layout::select_combo<value_type>(label, value) };
		if (value && ImGui::IsItemHovered())
		{
		}
		const String menu_label { "##SelectorMenu##{0}##{1}"_s.format(label, this->info.brief) };
		if (ImGui::BeginPopupContextItem(menu_label.c_str()))
		{
			if (Layout::inspect_button((label + menu_label), value))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}

		return Layout::end_prop(this, changed);
	}
	
	bool PropertyDrawer<Shader>::operator()(String const & label, pointer & value) const
	{
		Layout::begin_prop(this, label, value);

		// Popup
		const String button_label { ("{0}##NewButton##{1}"_s).format(label, this->info.brief) };
		const String popup_label { ("Create {1}##{0}##Popup"_s).format(label, this->info.brief) };
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
			static const_pointer copy[3] { nullptr };

			// Popup Opened
			if (!popup_open && (popup_open = true))
			{
				std::strcpy(name, "new_shader");
				std::strcpy(asset_path, "");
				copy[0] = copy[1] = copy[2] = nullptr;
			}

			// Name
			ImGui::InputText(
				("Name##" + this->info.brief.str() + "##" + label).c_str(),
				name,
				ML_ARRAYSIZE(name)
			);

			// Copy
			ImGui::Separator();
			if (self_type()(("Copy Vertex##" + label), (const_pointer &)copy[0]))
			{
				std::strcpy(asset_path, "");
			}
			if (self_type()(("Copy Fragment##" + label), (const_pointer &)copy[2]))
			{
				std::strcpy(asset_path, "");
			}
			if (self_type()(("Copy Geometry##" + label), (const_pointer &)copy[1]))
			{
				std::strcpy(asset_path, "");
			}
			ImGui::Separator();

			// Path
			ImGui::InputText(
				("##Path##" + this->info.brief.str() + "##" + label).c_str(),
				asset_path,
				ML_MAX_PATH
			);
			ImGui::SameLine();
			static String open_path;
			if (ImGuiExt::OpenFile(("Browse##" + label), open_path, { 1280, 720 }) == 1)
			{
				if (ML_FS.fileExists(open_path))
				{
					std::strcpy(asset_path, open_path.c_str());
					open_path.clear();
					copy[0] = copy[1] = copy[2] = nullptr;
				}
			}
			ImGui::Separator();

			// Submit
			const bool submit { ImGui::Button("Submit") };
			if (submit && !value)
			{
				if (ML_FS.fileExists(asset_path))
				{
					value = ML_Engine.content().create<value_type>(name, asset_path);
				}
				else if (copy[0] || copy[1] || copy[2])
				{
					(value = ML_Engine.content().create<value_type>(name))->loadFromMemory(
						(copy[0] ? copy[0]->vertSrc() : String{}),
						(copy[1] ? copy[1]->geomSrc() : String{}),
						(copy[2] ? copy[2]->fragSrc() : String{})
					);
				}
				else
				{
					value = ML_Engine.content().create<value_type>(name);
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

			return Layout::end_prop(this, submit || cancel);
		}
		return Layout::end_prop(this, false);
	}

	bool PropertyDrawer<Shader>::operator()(String const & label, reference value) const
	{
		Layout::begin_prop(this, label, value);

		/* * * * * * * * * * * * * * * * * * * * */

		bool changed { false };

		ImGui::Text("Handle: %u", (uint32_t)value);

		if (ImGui::BeginTabBar(("SourceTabs##" + label).c_str()))
		{
			auto draw_source_tab = [](
				String const & name, 
				String const & type, 
				String const & source)
			{
				if (!source) return;
				
				/* * * * * * * * * * * * * * * * * * * * */

				const bool tab_open {
					ImGui::BeginTabItem((type + "##ShaderTab##" + name).c_str()) 
				};

				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::Button("Copy to Clipboard"))
					{
						ML_Engine.window().setClipboardString(source);
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}

				if (tab_open)
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

		/* * * * * * * * * * * * * * * * * * * * */

		return Layout::end_prop(this, changed);
	}


	// Sound Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool PropertyDrawer<Sound>::operator()(String const & label, const_pointer & value) const
	{
		Layout::begin_prop(this, label, value);

		const bool changed { Layout::select_combo<value_type>(label, value) };
		if (value && ImGui::IsItemHovered())
		{
		}
		const String menu_label { "##SelectorMenu##{0}##{1}"_s.format(label, this->info.brief) };
		if (ImGui::BeginPopupContextItem(menu_label.c_str()))
		{
			if (Layout::inspect_button((label + menu_label), value))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}

		return Layout::end_prop(this, changed);
	}

	bool PropertyDrawer<Sound>::operator()(String const & label, pointer & value) const
	{
		Layout::begin_prop(this, label, value);

		// Popup
		const String button_label { ("{0}##NewButton##{1}"_s).format(label, this->info.brief) };
		const String popup_label { ("Create {1}##{0}##Popup"_s).format(label, this->info.brief) };
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
				("Name##" + this->info.brief.str() + "##" + label).c_str(),
				name,
				ML_ARRAYSIZE(name)
			);

			// Path
			ImGui::InputText(
				("##Path##" + this->info.brief.str() + "##" + label).c_str(),
				asset_path,
				ML_MAX_PATH
			);
			ImGui::SameLine();
			static String open_path;
			if (ImGuiExt::OpenFile(("Browse##" + label), open_path, { 1280, 720 }) == 1)
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
					value = ML_Engine.content().create<value_type>(name, asset_path);
				}
				else
				{
					value = ML_Engine.content().create<value_type>(name);
				}
			}
			if (submit || cancel)
			{
				popup_open = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();

			return Layout::end_prop(this, submit || cancel);
		}
		return Layout::end_prop(this, false);
	}

	bool PropertyDrawer<Sound>::operator()(String const & label, reference value) const
	{
		Layout::begin_prop(this, label, value);

		return Layout::end_prop(this, false);
	}


	// Sprite Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool PropertyDrawer<Sprite>::operator()(String const & label, const_pointer & value) const
	{
		Layout::begin_prop(this, label, value);

		const bool changed { Layout::select_combo<value_type>(label, value) };
		if (value && ImGui::IsItemHovered())
		{
		}
		const String menu_label { "##SelectorMenu##{0}##{1}"_s.format(label, this->info.brief) };
		if (ImGui::BeginPopupContextItem(menu_label.c_str()))
		{
			if (Layout::inspect_button((label + menu_label), value))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}

		return Layout::end_prop(this, changed);
	}

	bool PropertyDrawer<Sprite>::operator()(String const & label, pointer & value) const
	{
		Layout::begin_prop(this, label, value);

		// Popup
		const String button_label { ("{0}##NewButton##{1}"_s).format(label, this->info.brief) };
		const String popup_label { ("Create {1}##{0}##Popup"_s).format(label, this->info.brief) };
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
				("Name##" + this->info.brief.str() + "##" + label).c_str(),
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
				("##Path##" + this->info.brief.str() + "##" + label).c_str(),
				asset_path,
				ML_MAX_PATH
			);
			ImGui::SameLine();
			static String open_path;
			if (ImGuiExt::OpenFile(("Browse##" + label), open_path, { 1280, 720 }) == 1)
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
					value = ML_Engine.content().create<value_type>(name, (*copy));
				}
				else if (ML_FS.fileExists(asset_path))
				{
					value = ML_Engine.content().create<value_type>(name, asset_path);
				}
				else
				{
					value = ML_Engine.content().create<value_type>(name);
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

			return Layout::end_prop(this, submit || cancel);
		}
		return Layout::end_prop(this, false);
	}

	bool PropertyDrawer<Sprite>::operator()(String const & label, reference value) const
	{
		Layout::begin_prop(this, label, value);

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

		Texture const * tex = value.texture();
		if (PropertyDrawer<Texture>()("Texture##Sprite", tex))
		{
			value.setTexture(tex);
		}

		ImGui::PopID();

		return Layout::end_prop(this, changed);
	}

	
	// Texture Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool PropertyDrawer<Texture>::operator()(String const & label, const_pointer & value) const
	{
		Layout::begin_prop(this, label, value);
		
		const bool changed { Layout::select_combo<value_type>(label, value) };
		
		if (value && ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			switch (value->sampler())
			{
			case GL::Texture2D:
				if (auto preview { ML_Editor.previews().getPreview(value) })
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
				ImGui::Text("Texture3D previews are currently disabled.");
				break;
			
			case GL::TextureCubeMap:
				ImGui::Text("Cubemap previews are currently disabled.");
				break;
			}
			ImGui::EndTooltip();
		}
		
		const String menu_label { "##SelectorMenu##{0}##{1}"_s.format(label, this->info.brief) };
		
		if (ImGui::BeginPopupContextItem(menu_label.c_str()))
		{
			if (Layout::inspect_button((label + menu_label), value))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
		
		return Layout::end_prop(this, changed);
	}

	bool PropertyDrawer<Texture>::operator()(String const & label, pointer & value) const
	{
		Layout::begin_prop(this, label, value);
		
		// Popup
		const String button_label { ("{0}##NewButton##{1}"_s).format(label, this->info.brief) };
		const String popup_label { ("Create {1}##{0}##Popup"_s).format(label, this->info.brief) };
		if (ImGui::Button(button_label.c_str()))
		{
			ImGui::OpenPopup(popup_label.c_str());
		}
		if (ImGui::BeginPopupModal(popup_label.c_str(), 0, ImGuiWindowFlags_AlwaysAutoResize))
		{
			// State
			static bool popup_open { false };
			static char name[32] = "";
			static Array<Image const *, 6> image { nullptr };
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
				("Name##" + this->info.brief.str() + "##"+ label).c_str(),
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
				if (PropertyDrawer<Image>()(("Image##" + label), (Image const *&)image[0]))
				{
					std::strcpy(asset_path, "");
				}

				// Path
				ImGui::InputText(
					("##Path##" + this->info.brief.str() + "##" + label).c_str(),
					asset_path,
					ML_MAX_PATH
				);
				ImGui::SameLine();
				if (ImGuiExt::OpenFile(("Browse##" + label), open_path, { 1280, 720 }) == 1)
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
				PropertyDrawer<Image>()(("Right##Image##" + label), (Image const *&)image[0]);
				PropertyDrawer<Image>()(("Left##Image##" + label), (Image const *&)image[1]);
				PropertyDrawer<Image>()(("Top##Image##" + label), (Image const *&)image[2]);
				PropertyDrawer<Image>()(("Bottom##Image##" + label), (Image const *&)image[3]);
				PropertyDrawer<Image>()(("Front##Image##" + label), (Image const *&)image[4]);
				PropertyDrawer<Image>()(("Back##Image##" + label), (Image const *&)image[5]);
			}

			// Submit
			const bool submit { ImGui::Button("Submit") };
			if (submit && !value)
			{
				if (sampler_type == 0)
				{
					if (image[0])
					{
						if (value = ML_Engine.content().create<Texture>(name))
						{
							if (!value->loadFromImage(*image[0]))
							{
								Debug::logError("Failed loading texture from image: {0}", name);
							}
						}
					}
					else if (asset_path)
					{
						if (value = ML_Engine.content().create<Texture>(name))
						{
							if (Image temp_img{ asset_path })
							{
								value->loadFromImage(temp_img);
							}
							else
							{
								Debug::logError("Failed loading texture from file: {0}", name);
							}
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
						ML_Engine.content().insert<Texture>(name, value);
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

			return Layout::end_prop(this, submit || cancel);
		}
		return Layout::end_prop(this, false);
	}

	bool PropertyDrawer<Texture>::operator()(String const & label, reference value) const
	{
		Layout::begin_prop(this, label, value);
		
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
			value.setSmooth(smooth); 
			changed = true;
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
		if (ImGui::Button(((mipmapped 
			? "Invalidate Mipmap##" : "Generate Mipmap##") + label).c_str()
		))
		{
			value.setMipmapped(!mipmapped);
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
			ML_Editor.previews().drawPreview(value, ImGuiExt::GetContentRegionAvail(), nullptr);
		}
		break;
		case GL::Texture3D:
		{
			ImGui::TextWrapped("Texture3D previews are currently disabled.");
		}
		break;
		case GL::TextureCubeMap:
		{
			ImGui::TextWrapped("Cubemap previews are currently disabled.");
		}
		break;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		ImGui::Columns(1);

		return Layout::end_prop(this, changed);
	}


	// Uniform Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool PropertyDrawer<Uniform>::operator()(String const & label, const_pointer & value) const
	{
		Layout::begin_prop(this, label, value);

		const bool changed { Layout::select_combo<value_type>(label, value) };
		
		if (value && ImGui::IsItemHovered())
		{
		}
		
		const String menu_label { "##SelectorMenu##{0}##{1}"_s.format(label, this->info.brief) };
		
		if (ImGui::BeginPopupContextItem(menu_label.c_str()))
		{
			if (Layout::inspect_button((label + menu_label), value))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}

		return Layout::end_prop(this, changed);
	}

	bool PropertyDrawer<Uniform>::operator()(String const & label, pointer & value) const
	{
		Layout::begin_prop(this, label, value);

		// Popup
		const String button_label { ("{0}##NewButton##{1}"_s).format(label, this->info.brief) };
		const String popup_label { ("Create {1}##{0}##Popup"_s).format(label, this->info.brief) };
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
				("Name##" + this->info.brief.str() + "##"+ label).c_str(),
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
					case Uniform::category_of<bool>():
						return (Uniform *)new uni_bool{ name, {} };

					case Uniform::category_of<int32_t>():
						return (Uniform *)new uni_int{ name, {} };

					case Uniform::category_of<float_t>():
						return (Uniform *)new uni_float{ name, {} };

					case Uniform::category_of<vec2f>():
						return (Uniform *)new uni_vec2{ name, {} };

					case Uniform::category_of<vec3f>():
						return (Uniform *)new uni_vec3{ name, {} };

					case Uniform::category_of<vec4f>():
						return (Uniform *)new uni_vec4{ name, {} };

					case Uniform::category_of<Color>():
						return (Uniform *)new uni_color{ name, {} };

					case Uniform::category_of<mat2f>():
						return (Uniform *)new uni_mat2{ name, {} };

					case Uniform::category_of<mat3f>():
						return (Uniform *)new uni_mat3{ name, {} };

					case Uniform::category_of<mat4f>():
						return (Uniform *)new uni_mat4{ name, {} };

					case Uniform::category_of<Texture>():
						return (Uniform *)new uni_sampler{ name, {} };
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

			return Layout::end_prop(this, submit || cancel);
		}
		return Layout::end_prop(this, false);
	}

	bool PropertyDrawer<Uniform>::operator()(String const & label, reference value) const
	{
		constexpr float_t spd{ 0.005f };
		constexpr C_String fmt{ "%.4f" };
		
		Layout::begin_prop(this, label, value);
		
		switch (value.get_root_id())
		{
		case typeof<bool>::hash(): if (auto const * v{ uniform_cast<bool>(&value) })
		{
			auto copy{ *v };
			const String name{ "##" + label + "##Bool##Uni##" + value.name() };
			const bool changed{ ImGui::Checkbox(name.c_str(), &copy) };
			if (value.get_self_id() == typeof<uni_bool>::hash())
			{
				static_cast<uni_bool *>(&value)->set(copy);
				return Layout::end_prop(this, true);
			}
		} break;
		case typeof<int32_t>::hash(): if (auto const * v{ uniform_cast<int32_t>(&value) })
		{
			auto copy{ *v };
			const String name{ "##" + label + "##Int##Uni##" + value.name() };
			const bool changed{ (!value.is_modifiable())
				? ImGui::DragInt(name.c_str(), &copy)
				: ImGui::InputInt(name.c_str(), &copy)
			};
			if (value.get_self_id() == typeof<uni_int>::hash())
			{
				static_cast<uni_int *>(&value)->set(copy);
				return Layout::end_prop(this, true);
			}
		} break;
		case typeof<float_t>::hash(): if (auto const * v{ uniform_cast<float_t>(&value) })
		{
			auto copy{ *v };
			const String name{ "##" + label + "##Float##Uni##" + value.name() };
			const bool changed{ ImGui::DragFloat(name.c_str(), &copy, spd, 0, 0, fmt) };
			if (value.get_self_id() == typeof<uni_float>::hash())
			{
				static_cast<uni_float *>(&value)->set(copy);
				return Layout::end_prop(this, true);
			}
		} break;
		case typeof<vec2>::hash(): if (auto const * v{ uniform_cast<vec2>(&value) })
		{
			auto copy{ *v };
			const String name{ "##" + label + "##Vec2##Uni##" + value.name() };
			const bool changed{ ImGui::DragFloat2(name.c_str(), &copy[0], spd, 0, 0, fmt) };
			if (value.get_self_id() == typeof<uni_vec2>::hash())
			{
				static_cast<uni_vec2 *>(&value)->set(copy);
				return Layout::end_prop(this, true);
			}
		} break;
		case typeof<vec3>::hash(): if (auto const * v{ uniform_cast<vec3>(&value) })
		{
			auto copy{ *v };
			const String name{ "##" + label + "##Vec3##Uni##" + value.name() };
			const bool changed{ ImGui::DragFloat3(name.c_str(), &copy[0], spd, 0, 0, fmt) };
			if (value.get_self_id() == typeof<uni_vec3>::hash())
			{
				static_cast<uni_vec3 *>(&value)->set(copy);
				return Layout::end_prop(this, true);
			}
		} break;
		case typeof<vec4>::hash(): if (auto const * v{ uniform_cast<vec4>(&value) })
		{
			auto copy{ *v };
			const String name{ "##" + label + "##Vec4##Uni##" + value.name() };
			const bool changed{ ImGui::DragFloat4(name.c_str(), &copy[0], spd, 0, 0, fmt) };
			if (value.get_self_id() == typeof<uni_vec4>::hash())
			{
				static_cast<uni_vec4 *>(&value)->set(copy);
				return Layout::end_prop(this, true);
			}
		} break;
		case typeof<Color>::hash(): if (auto const * v{ uniform_cast<Color>(&value) })
		{
			auto copy{ *v };
			const String name{ "##" + label + "##Color##Uni##" + value.name() };
			const bool changed{ ImGui::ColorEdit4(name.c_str(), &copy[0], ImGuiColorEditFlags_Float) };
			if (value.get_self_id() == typeof<uni_color>::hash())
			{
				static_cast<uni_color *>(&value)->set(copy);
				return Layout::end_prop(this, true);
			}
		} break;
		case typeof<mat2>::hash(): if (auto const * v{ uniform_cast<mat2>(&value) })
		{
			auto copy{ *v };
			const String name{ "##" + label + "##Mat2##Uni##" + value.name() };
			const bool changed{ ImGuiExt::DragMat2(name.c_str(), copy, spd, 0, 0, fmt) };
			if (value.get_self_id() == typeof<uni_mat2>::hash())
			{
				static_cast<uni_mat2 *>(&value)->set(copy);
				return Layout::end_prop(this, true);
			}
		} break;
		case typeof<mat3>::hash(): if (auto const * v{ uniform_cast<mat3>(&value) })
		{
			auto copy{ *v };
			const String name{ "##" + label + "##Mat3##Uni##" + value.name() };
			const bool changed{ ImGuiExt::DragMat3(name.c_str(), copy, spd, 0, 0, fmt) };
			if (value.get_self_id() == typeof<uni_mat3>::hash())
			{
				static_cast<uni_mat3 *>(&value)->set(copy);
				return Layout::end_prop(this, true);
			}
		} break;
		case typeof<mat4>::hash(): if (auto const * v{ uniform_cast<mat4>(&value) })
		{
			auto copy{ *v };
			const String name{ "##" + label + "##Mat4##Uni##" + value.name() };
			const bool changed{ ImGuiExt::DragMat4(name.c_str(), copy, spd, 0, 0, fmt) };
			if (value.get_self_id() == typeof<uni_mat4>::hash())
			{
				static_cast<uni_mat4 *>(&value)->set(copy);
				return Layout::end_prop(this, true);
			}
		} break;
		case typeof<Texture>::hash(): if (auto const * v{ uniform_cast<Texture const *>(&value) })
		{
			auto copy{ *v };
			const String name{ "##" + label + "##Sampler##Uni##" + value.name() };
			const bool changed{ PropertyDrawer<Texture>()(name, copy) };
			if (value.get_self_id() == typeof<uni_sampler>::hash())
			{
				static_cast<uni_sampler *>(&value)->set(copy);
				return Layout::end_prop(this, true);
			}
		} break;
		}
		return Layout::end_prop(this, false);
	}
}