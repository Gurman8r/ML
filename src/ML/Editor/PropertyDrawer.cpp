#include <ML/Editor/PropertyDrawer.hpp>
#include <ML/Engine/ContentDatabase.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Editor/ImGuiExt.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/Rect.hpp>
#include <ML/Engine/Ref.hpp>

#include <ML/Audio/Sound.hpp>
#include <ML/Engine/Entity.hpp>
#include <ML/Graphics/Font.hpp>
#include <ML/Graphics/Material.hpp>
#include <ML/Graphics/Model.hpp>
#include <ML/Graphics/Sprite.hpp>
#include <ML/Graphics/Surface.hpp>
#include <ML/Engine/Script.hpp>
#include <ML/Graphics/Renderer.hpp>


/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	template <
		class T
	> static inline bool asset_dropdown(const String & label, const T *& value)
	{
		int32_t index = ML_Content.get_index_of<T>(value);
		return (ImGuiExt::Combo(label.c_str(), &index, ML_Content.get_keys<T>())
			? (value = ML_Content.find_by_index<T>(index))
			: false
		);
	}
}

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	// Entity Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool EntityPropertyDrawer::operator()(const String & label, const_pointer & value, int32_t flags) const
	{
		return asset_dropdown<Entity>(label, value);
	}

	bool EntityPropertyDrawer::operator()(const String & label, const_reference value, int32_t flags) const
	{
		return false;
	}

	bool EntityPropertyDrawer::operator()(const String & label, reference value, int32_t flags) const
	{
		if (Renderer * r = value.get<Renderer>())
		{
			RenderStates & states = r->states();
			if (ImGui::CollapsingHeader("Renderer"))
			{
				/* * * * * * * * * * * * * * * * * * * * */

				const Model * model = (const Model *)r->drawable();
				if (ModelPropertyDrawer()("Model##Renderer", model))
				{
					r->setDrawable(model);
				}

				const Material * material = r->material();
				if (MaterialPropertyDrawer()("Material##Renderer", material))
				{
					r->setMaterial(material);
				}

				/* * * * * * * * * * * * * * * * * * * * */

				if (ImGui::TreeNode("Alpha Testing"))
				{
					if (AlphaState * alphaTest = states.get<AlphaState>())
					{
						ImGui::Checkbox("##Enabled##Alpha Testing", &alphaTest->enabled);

						int32_t index = GL::index_of(alphaTest->predicate);
						if (ImGui::Combo(
							"Comparison##Alpha Testing",
							&index,
							GL::Predicate_names,
							ML_ARRAYSIZE(GL::Predicate_names)
						))
						{
							GL::value_at(index, alphaTest->predicate);
						}

						ImGui::DragFloat("Coeff##Alpha Testing", &alphaTest->coeff);
					}
					ImGui::TreePop();
				}

				/* * * * * * * * * * * * * * * * * * * * */

				if (ImGui::TreeNode("Blend Function"))
				{
					if (BlendState * blendFunc = states.get<BlendState>())
					{
						ImGui::Checkbox("Enabled##Blending", &blendFunc->enabled);

						auto factor_combo = [](C_String label, int32_t & index)
						{
							return ImGui::Combo(
								label,
								&index,
								GL::Factor_names,
								ML_ARRAYSIZE(GL::Factor_names)
							);
						};

						int32_t srcRGB = GL::index_of(blendFunc->srcRGB);
						if (factor_combo("Src RGB##Blending", srcRGB))
						{
							GL::value_at(srcRGB, blendFunc->srcRGB);
						}

						int32_t srcAlpha = GL::index_of(blendFunc->srcAlpha);
						if (factor_combo("Src Alpha##Blending", srcAlpha))
						{
							GL::value_at(srcAlpha, blendFunc->srcAlpha);
						}

						int32_t dstRGB = GL::index_of(blendFunc->dstRGB);
						if (factor_combo("Dst RGB##Blending", dstRGB))
						{
							GL::value_at(dstRGB, blendFunc->dstRGB);
						}

						int32_t dstAlpha = GL::index_of(blendFunc->dstAlpha);
						if (factor_combo("Dst Alpha##Blending", dstAlpha))
						{
							GL::value_at(dstAlpha, blendFunc->dstAlpha);
						}
					}
					ImGui::TreePop();
				}

				/* * * * * * * * * * * * * * * * * * * * */

				if (ImGui::TreeNode("Cull Face"))
				{
					if (CullState * cullFace = states.get<CullState>())
					{
						ImGui::Checkbox("Enabled##Culling", &cullFace->enabled);

						int32_t index = GL::index_of(cullFace->face);
						if (ImGui::Combo(
							"Face##Culling",
							&index,
							GL::Face_names,
							ML_ARRAYSIZE(GL::Face_names)
						))
						{
							GL::value_at(index, cullFace->face);
						}
					}
					ImGui::TreePop();
				}

				/* * * * * * * * * * * * * * * * * * * * */

				if (ImGui::TreeNode("Depth Testing"))
				{
					if (DepthState * depthTest = states.get<DepthState>())
					{
						ImGui::Checkbox("Enabled##Depth Testing", &depthTest->enabled);

						int32_t index = GL::index_of(depthTest->predicate);
						if (ImGui::Combo(
							"Comparison##Depth Testing",
							&index,
							GL::Predicate_names,
							ML_ARRAYSIZE(GL::Predicate_names)
						))
						{
							GL::value_at(index, depthTest->predicate);
						}
					}
					ImGui::TreePop();
				}

				/* * * * * * * * * * * * * * * * * * * * */
			}
		}
		return false;
	}


	// Font Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool FontPropertyDrawer::operator()(const String & label, const_pointer & value, int32_t flags) const
	{
		return asset_dropdown<Font>(label, value);
	}

	bool FontPropertyDrawer::operator()(const String & label, const_reference value, int32_t flags) const
	{
		ImGui::Text("%s", value.getInfo().family.c_str());
		return false;
	}

	bool FontPropertyDrawer::operator()(const String & label, reference value, int32_t flags) const
	{
		return (*this)(label, (const_reference)value, flags);
	}


	// Image Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool ImagePropertyDrawer::operator()(const String & label, const_pointer & value, int32_t flags) const
	{
		return asset_dropdown<Image>(label, value);
	}

	bool ImagePropertyDrawer::operator()(const String & label, const_reference value, int32_t flags) const
	{
		ImGui::Text("Size: %i x %i", value.width(), value.height());
		ImGui::Text("Channels: %i", value.channels());
		return false;
	}

	bool ImagePropertyDrawer::operator()(const String & label, reference value, int32_t flags) const
	{
		bool changed { false };

		ImGui::Columns(2);

		// Settings
		ImGui::Text("Size: %i x %i", value.width(), value.height());
		ImGui::Text("Channels: %i", value.channels());
		if (ImGui::Button("Flip Vertically"))
		{
			value.flipVertically();
			changed = true;
		}
		if (ImGui::Button("Flip Horizontally"))
		{
			value.flipHorizontally();
			changed = true;
		}

		ImGui::NextColumn();

		// Temporary Texture
		Ref<Texture> preview { "##PropertyDrawer##Image##Preview##" + label };
		if (!preview && !preview.create())
		{
			return changed;
		}

		// Lock
		static hash_t img_lock { 0 };
		if (const hash_t img_hash { Hash { value.pixels() } })
		{
			if ((img_lock != img_hash) && (img_lock = img_hash))
			{
				preview->loadFromImage(value);
			}
		}

		// Preview
		if (preview && (*preview))
		{
			const vec2 dst { ImGuiExt::GetContentRegionAvail()[0], 256 };
			const vec2 scl { alg::scale_to_fit((vec2)value.size(), dst) };
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

		ImGui::Columns(1);
		return changed;
	}


	// Material Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool MaterialPropertyDrawer::operator()(const String & label, const_pointer & value, int32_t flags) const
	{
		return asset_dropdown<Material>(label, value);
	}

	bool MaterialPropertyDrawer::operator()(const String & label, const_reference value, int32_t flags) const
	{
		
		return false;
	}

	bool MaterialPropertyDrawer::operator()(const String & label, reference value, int32_t flags) const
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
			Uni * u = nullptr;
			if (UniformPropertyDrawer()(("##NewUniform##Material" + label).c_str(), u))
			{
				if (!value.add(u))
				{
					delete u;
					Debug::logError("A uniform with that name already exists");
				}
			}

			// do nothing if empty
			if (!value.uniforms().empty())
				ImGui::Separator();

			// to remove
			List<List<Uni *>::iterator> toRemove;

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
						ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
						ImGui::BeginChild(
							("UniformChild##" + name).c_str(),
							{ -1, (32 * height) + (height == 1 ? 8 : -8) },
							true,
							ImGuiWindowFlags_NoScrollWithMouse
						);

						if (UniformPropertyDrawer()(name, (Uni &)(*(*it))))
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


	// Model Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool ModelPropertyDrawer::operator()(const String & label, const_pointer & value, int32_t flags) const
	{
		return asset_dropdown<Model>(label, value);
	}

	bool ModelPropertyDrawer::operator()(const String & label, const_reference value, int32_t flags) const
	{
		ImGui::Text("%s", label.c_str());
		return false;
	}

	bool ModelPropertyDrawer::operator()(const String & label, reference value, int32_t flags) const
	{
		ImGui::PushID(ML_ADDRESSOF(&value));
		ImGui::Text("Meshes: %u", value.meshes().size());
		for (size_t i = 0; i < value.meshes().size(); i++)
		{
			const Mesh * mesh { value.meshes()[i] };
			ImGui::PushID(ML_ADDRESSOF(mesh));

			if (ImGui::TreeNode(String(label + " [" + std::to_string(i) + "]").c_str()))
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
	bool ScriptPropertyDrawer::operator()(const String & label, const_pointer & value, int32_t flags) const
	{
		return asset_dropdown<Script>(label, value);
	}

	bool ScriptPropertyDrawer::operator()(const String & label, const_reference value, int32_t flags) const
	{
		ImGui::Text("%s", label.c_str());
		return false;
	}

	bool ScriptPropertyDrawer::operator()(const String & label, reference value, int32_t flags) const
	{
		return (*this)(label, (const_reference)value, flags);
	}


	// Shader Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool ShaderPropertyDrawer::operator()(const String & label, const_pointer & value, int32_t flags) const
	{
		return asset_dropdown<Shader>(label, value);
	}
	
	bool ShaderPropertyDrawer::operator()(const String & label, const_reference value, int32_t flags) const
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

	bool ShaderPropertyDrawer::operator()(const String & label, reference value, int32_t flags) const
	{
		return (*this)(label, (const Shader &)value);
	}


	// Sound Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool SoundPropertyDrawer::operator()(const String & label, const_pointer & value, int32_t flags) const
	{
		return asset_dropdown<Sound>(label, value);
	}

	bool SoundPropertyDrawer::operator()(const String & label, const_reference value, int32_t flags) const
	{
		return false;
	}

	bool SoundPropertyDrawer::operator()(const String & label, reference value, int32_t flags) const
	{
		return false;
	}


	// Sprite Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool SpritePropertyDrawer::operator()(const String & label, const_pointer & value, int32_t flags) const
	{
		return asset_dropdown<Sprite>(label, value);
	}

	bool SpritePropertyDrawer::operator()(const String & label, const_reference value, int32_t flags) const
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

	bool SpritePropertyDrawer::operator()(const String & label, reference value, int32_t flags) const
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
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool SurfacePropertyDrawer::operator()(const String & label, const_pointer & value, int32_t flags) const
	{
		return asset_dropdown<Surface>(label, value);
	}

	bool SurfacePropertyDrawer::operator()(const String & label, const_reference value, int32_t flags) const
	{
		const vec2 previewSize = ([](const vec2 & src, const vec2 & dst)
		{
			const vec2
				hs = { (dst[0] / src[0]), (dst[0] / src[0]) },
				vs = { (dst[1] / src[1]), (dst[1] / src[1]) };
			return (src * (((hs) < (vs)) ? (hs) : (vs)));

		})(value.size(), { 256, 256 }); // <- target size

		ImGui::Image(
			value.texture().get_handle(),
			{ previewSize[0], previewSize[1] },
			{ 0, 1 },
			{ 1, 0 },
			{ 255, 255, 255, 255 },
			{ 255, 255, 255, 128 }
		);
		return false;
	}

	bool SurfacePropertyDrawer::operator()(const String & label, reference value, int32_t flags) const
	{
		const vec2 previewSize = ([](const vec2 & src, const vec2 & dst)
		{
			const vec2
				hs = { (dst[0] / src[0]), (dst[0] / src[0]) },
				vs = { (dst[1] / src[1]), (dst[1] / src[1]) };
			return (src * (((hs) < (vs)) ? (hs) : (vs)));

		})(value.size(), { 256, 256 }); // <- target size

		ImGui::Image(
			value.texture().get_handle(),
			{ previewSize[0], previewSize[1] },
			{ 0, 1 },
			{ 1, 0 },
			{ 255, 255, 255, 255 },
			{ 255, 255, 255, 128 }
		);
		return false;
	}

	
	// Texture Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool TexturePropertyDrawer::operator()(const String & label, const_pointer & value, int32_t flags) const
	{
		return asset_dropdown<Texture>(label, value);
	}

	bool TexturePropertyDrawer::operator()(const String & label, const_reference value, int32_t flags) const
	{
		return false;
	}

	bool TexturePropertyDrawer::operator()(const String & label, reference value, int32_t flags) const
	{
		bool changed = false;

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

		/* * * * * * * * * * * * * * * * * * * * */

		int32_t level { value.level() };
		if (ImGui::InputInt(("Level##" + label).c_str(), &level))
		{
			value.setLevel(level); 
			changed = true;
		}
		ImGui::SameLine(); ImGuiExt::HelpMarker("Work In Progress");

		/* * * * * * * * * * * * * * * * * * * * */

		int32_t target { GL::index_of(value.sampler()) };
		if (ImGui::Combo(
			("Sampler##" + label).c_str(),
			&(target), 
			GL::Sampler_names, 
			ML_ARRAYSIZE(GL::Sampler_names)
		))
		{
			GL::Sampler temp;
			if (GL::value_at(target, temp)) value.setSampler(temp);
			changed = true;
		}
		ImGui::SameLine(); ImGuiExt::HelpMarker("Work In Progress");

		/* * * * * * * * * * * * * * * * * * * * */

		int32_t colorFormat { GL::index_of(value.color_fmt()) };
		if (ImGui::Combo(
			("Color Format##" + label).c_str(),
			&(colorFormat),
			GL::Format_names,
			ML_ARRAYSIZE(GL::Format_names)
		))
		{
			GL::Format temp;
			if (GL::value_at(colorFormat, temp)) value.setColorFormat(temp);
			changed = true;
		}
		ImGui::SameLine(); ImGuiExt::HelpMarker("Work In Progress");

		/* * * * * * * * * * * * * * * * * * * * */

		int32_t internalFormat { GL::index_of(value.internal_fmt()) };
		if (ImGui::Combo(
			("Internal Format##" + label).c_str(),
			&(internalFormat),
			GL::Format_names,
			ML_ARRAYSIZE(GL::Format_names)
		))
		{
			GL::Format temp;
			if (GL::value_at(internalFormat, temp)) value.setInternalFormat(temp);
			changed = true;
		}
		ImGui::SameLine(); ImGuiExt::HelpMarker("Work In Progress");

		/* * * * * * * * * * * * * * * * * * * * */

		int32_t pixelType { GL::index_of(value.pixel_type()) };
		if (ImGui::Combo(
			("Pixel Type##" + label).c_str(),
			&(pixelType),
			GL::Type_names,
			ML_ARRAYSIZE(GL::Type_names)
		))
		{
			GL::Type temp;
			if (GL::value_at(pixelType, temp)) value.setPixelType(temp);
			changed = true;
		}
		ImGui::SameLine(); ImGuiExt::HelpMarker("Work In Progress");

		/* * * * * * * * * * * * * * * * * * * * */

		switch (value.sampler())
		{
		case GL::Texture2D:
		{
			const vec2 scl { alg::scale_to_fit((vec2)value.size(), { 256, 256 }) };
			ImGui::Image(value.get_handle(), { scl[0], scl[1] }, { 0, 1 }, { 1, 0 });
		}
		break;
		case GL::TextureCubeMap:
		{

		}
		break;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		return changed;
	}


	// Uniform Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool UniformPropertyDrawer::operator()(const String & label, const_pointer & value, int32_t flags) const
	{
		return asset_dropdown<Uni>(label, value);
	}

	bool UniformPropertyDrawer::operator()(const String & label, const_reference value, int32_t flags) const
	{
		return false;
	}

	bool UniformPropertyDrawer::operator()(const String & label, pointer & value, int32_t flags) const
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
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			// Type Input
			static int32_t	type = 0;
			ImGui::Combo(
				"Type",
				&type,
				Uni::Type_names,
				ML_ARRAYSIZE(Uni::Type_names)
			);

			// Name Input
			static char name[32] = "my_uniform\0";
			const bool enterPress = ImGui::InputText(
				"Name",
				name,
				ML_ARRAYSIZE(name),
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
				value = ([&]()
				{
					switch (type)
					{
					case uni_bool	::ID: return (Uni *)new uni_bool	{ name, { NULL } };
					case uni_float	::ID: return (Uni *)new uni_float	{ name, { NULL } };
					case uni_int	::ID: return (Uni *)new uni_int		{ name, { NULL } };
					case uni_vec2	::ID: return (Uni *)new uni_vec2	{ name, { NULL } };
					case uni_vec3	::ID: return (Uni *)new uni_vec3	{ name, { NULL } };
					case uni_vec4	::ID: return (Uni *)new uni_vec4	{ name, { NULL } };
					case uni_color	::ID: return (Uni *)new uni_color	{ name, { NULL } };
					case uni_mat3	::ID: return (Uni *)new uni_mat3	{ name, { NULL } };
					case uni_mat4	::ID: return (Uni *)new uni_mat4	{ name, { NULL } };
					case uni_sampler::ID: return (Uni *)new uni_sampler	{ name, { NULL } };
					}
					return (Uni *)nullptr;
				})();

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

	bool UniformPropertyDrawer::operator()(const String & label, reference value, int32_t flags) const
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
				if (TexturePropertyDrawer()(name, temp)) { u->data = temp; }
				return true;
			}
			break;
		}
		
		ImGui::SameLine();
		ImGuiExt::HelpMarker("This uniform cannot be modified.");
		return false;
	}
}