#include <ML/Editor/Project.hpp>

#include <ML/Audio/Sound.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/OS.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Editor/GUI.hpp>
#include <ML/Engine/Resources.hpp>
#include <ML/Engine/Entity.hpp>
#include <ML/Engine/SharedLibrary.hpp>
#include <ML/Graphics/Renderer.hpp>
#include <ML/Graphics/Light.hpp>
#include <ML/Graphics/Camera.hpp>
#include <ML/Graphics/Uni.hpp>
#include <ML/Graphics/Surface.hpp>
#include <ML/Graphics/Font.hpp>
#include <ML/Graphics/Model.hpp>
#include <ML/Graphics/Skybox.hpp>
#include <ML/Graphics/Sprite.hpp>
#include <ML/Engine/Rigidbody.hpp>
#include <ML/Engine/Particle.hpp>
#include <ML/Script/Script.hpp>

namespace ml
{
	struct Project::Layout
	{
		/* * * * * * * * * * * * * * * * * * * * */

		inline static bool vector_getter(void * vec, int32_t idx, CString * out_text)
		{
			auto & vector = (*static_cast<List<String>*>(vec));
			if ((idx >= 0) && (idx < static_cast<int32_t>(vector.size())))
			{
				(*out_text) = vector.at(idx).c_str();
				return true;
			}
			return false;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			class Fun, class ... Args
		> inline static void Columns(Fun fun, Args ... args)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 2, 2 });
			ImGui::Columns(2);
			ImGui::Separator();
			{
				fun((args)...);
			}
			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::PopStyleVar();
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			class Fun, class ... Args
		> inline static void Group(CString label, Fun fun, Args ... args)
		{
			ImGui::PushID(label);
			ImGui::AlignTextToFramePadding();
			bool node_open;
			{
				node_open = ImGui::TreeNode("ResourceView_Group", "%s", label);
			}
			ImGui::NextColumn();
			ImGui::AlignTextToFramePadding();
			{
				ImGui::Text("");
			}
			ImGui::NextColumn();
			if (node_open)
			{
				fun((args)...);

				ImGui::TreePop();
			}
			ImGui::PopID();
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			class Fun, class ... Args
		> inline static void Field(CString label, Fun fun, Args ... args)
		{
			ImGui::AlignTextToFramePadding();
			ImGui::TreeNodeEx(
				"ResourceView_Field",
				ImGuiTreeNodeFlags_Leaf |
				ImGuiTreeNodeFlags_NoTreePushOnOpen |
				ImGuiTreeNodeFlags_Bullet,
				"%s",
				label);
			ImGui::NextColumn();
			ImGui::PushItemWidth(-1);
			{
				fun(label, (args)...);
			}
			ImGui::PopItemWidth();
			ImGui::NextColumn();
		}

		/* * * * * * * * * * * * * * * * * * * * */

		inline static int32_t EditUniform(Resources & resources, const String & label, uni_base * value, bool show_constants)
		{
			int32_t flag = 0;

			auto toolbar_editable = [&]()
			{
				if (ImGui::Button(String("Delete" + label).c_str())) { flag = 1; }
			};

			auto toolbar_constant = [&]()
			{
				ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 0.4f, 0.4f, 1.0f });
				ImGui::Text(" [const]");
				ImGui::PopStyleColor();
			};

			switch (value->type)
			{
				// Flt
				/* * * * * * * * * * * * * * * * * * * * */
			case uni_flt::ID:
				if (auto u = dynamic_cast<uni_flt *>(value))
				{
					Layout::Field(value->name.c_str(), [&](CString)
					{
						ImGui::DragFloat(String(label + "##Float##Uni##" + value->name).c_str(), &u->data, 0.1f);
						toolbar_editable();
					});
					break;
				}
				else if (show_constants)
				{
					if (auto u = dynamic_cast<uni_flt_cr *>(value))
					{
						toolbar_constant();
						Layout::Field(value->name.c_str(), [&](CString)
						{
							auto temp = u->data;
							ImGui::DragFloat(String(label + "##Float##Uni##" + value->name).c_str(), &temp, 0.1f);
						});
						break;
					}
				}

				// Int
				/* * * * * * * * * * * * * * * * * * * * */
			case uni_int::ID:
				if (auto u = dynamic_cast<uni_int *>(value))
				{
					Layout::Field(value->name.c_str(), [&](CString)
					{
						ImGui::DragInt(String(label + "##Int##Uni##" + value->name).c_str(), &u->data, 0.1f);
						toolbar_editable();
					});
					break;
				}
				else if (show_constants)
				{
					if (auto u = dynamic_cast<uni_int_cr *>(value))
					{
						toolbar_constant();
						Layout::Field(value->name.c_str(), [&](CString)
						{
							auto temp = u->data;
							ImGui::DragInt(String(label + "##Int##Uni##" + value->name).c_str(), &temp, 0.1f);
						});
						break;
					}
				}

				// Vec2
				/* * * * * * * * * * * * * * * * * * * * */
			case uni_vec2::ID:
				if (auto u = dynamic_cast<uni_vec2 *>(value))
				{
					Layout::Field(value->name.c_str(), [&](CString)
					{
						GUI::EditVec2f(String(label + "##Vec2##Uni##" + value->name).c_str(), u->data, 0.1f);
						toolbar_editable();
					});
					break;
				}
				else if (show_constants)
				{
					if (auto u = dynamic_cast<uni_vec2_cr *>(value))
					{
						toolbar_constant();
						Layout::Field(value->name.c_str(), [&](CString)
						{
							auto temp = u->data;
							GUI::EditVec2f(String(label + "##Vec2##Uni##" + value->name).c_str(), temp, 0.1f);
						});
						break;
					}
				}

				// Vec3
				/* * * * * * * * * * * * * * * * * * * * */
			case uni_vec3::ID:
				if (auto u = dynamic_cast<uni_vec3 *>(value))
				{
					Layout::Field(value->name.c_str(), [&](CString)
					{
						GUI::EditVec3f(String(label + "##Vec3##Uni##" + value->name).c_str(), u->data, 0.1f);
						toolbar_editable();
					});
					break;
				}
				else if (show_constants)
				{
					if (auto u = dynamic_cast<uni_vec3_cr *>(value))
					{
						toolbar_constant();
						Layout::Field(value->name.c_str(), [&](CString)
						{
							auto temp = u->data;
							GUI::EditVec3f(String(label + "##Vec3##Uni##" + value->name).c_str(), temp, 0.1f);
						});
						break;
					}
				}

				// Vec4
				/* * * * * * * * * * * * * * * * * * * * */
			case uni_vec4::ID:
				if (auto u = dynamic_cast<uni_vec4 *>(value))
				{
					Layout::Field(value->name.c_str(), [&](CString)
					{
						GUI::EditVec4f(String(label + "##Vec4##Uni##" + value->name).c_str(), u->data, 0.1f);
						toolbar_editable();
					});
					break;
				}
				else if (show_constants)
				{
					if (auto u = dynamic_cast<uni_vec4_cr *>(value))
					{
						toolbar_constant();
						Layout::Field(value->name.c_str(), [&](CString)
						{
							auto temp = u->data;
							GUI::EditVec4f(String(label + "##Vec4##Uni##" + value->name).c_str(), temp, 0.1f);
						});
						break;
					}
				}

				// Col4
				/* * * * * * * * * * * * * * * * * * * * */
			case uni_col4::ID:
				if (auto u = dynamic_cast<uni_col4 *>(value))
				{
					Layout::Field(value->name.c_str(), [&](CString)
					{
						ImGui::ColorEdit4(String(label + "##Color##Uni##" + value->name).c_str(), &u->data[0]);
						toolbar_editable();
					});
					break;
				}
				else if (show_constants)
				{
					if (auto u = dynamic_cast<uni_col4_cr *>(value))
					{
						toolbar_constant();
						Layout::Field(value->name.c_str(), [&](CString)
						{
							auto temp = u->data;
							ImGui::ColorEdit4(String(label + "##Color##Uni##" + value->name).c_str(), &temp[0]);
						});
						break;
					}
				}

				// Mat3
				/* * * * * * * * * * * * * * * * * * * * */
			case uni_mat3::ID:
				if (auto u = dynamic_cast<uni_mat3 *>(value))
				{
					Layout::Field(value->name.c_str(), [&](CString)
					{
						GUI::EditMat3f(String(label + "##Mat3##Uni##" + value->name).c_str(), u->data, 0.1f);
						toolbar_editable();
					});
					break;
				}
				else if (show_constants)
				{
					if (auto u = dynamic_cast<uni_mat3_cr *>(value))
					{
						toolbar_constant();
						Layout::Field(value->name.c_str(), [&](CString)
						{
							auto temp = u->data;
							GUI::EditMat3f(String(label + "##Mat3##Uni##" + value->name).c_str(), temp, 0.1f);
						});
						break;
					}
				}

				// Mat4
				/* * * * * * * * * * * * * * * * * * * * */
			case uni_mat4::ID:
				if (auto u = dynamic_cast<uni_mat4 *>(value))
				{
					Layout::Field(value->name.c_str(), [&](CString)
					{
						GUI::EditMat4f(String(label + "##Mat4##Uni##" + value->name).c_str(), u->data, 0.1f);
						toolbar_editable();
					});
					break;
				}
				else if (show_constants)
				{
					if (auto u = dynamic_cast<uni_mat4_cr *>(value))
					{
						toolbar_constant();
						Layout::Field(value->name.c_str(), [&](CString)
						{
							auto temp = u->data;
							GUI::EditMat4f(String(label + "##Mat4##Uni##" + value->name).c_str(), temp, 0.1f);
						});
						break;
					}
				}

				// Tex
				/* * * * * * * * * * * * * * * * * * * * */
			case uni_tex_cp::ID:
				if (auto u = dynamic_cast<uni_tex_cp *>(value))
				{
					Layout::Field(value->name.c_str(), [&](CString)
					{
						int32_t index = resources.textures.getIndexOf(u->data);
						List<String> keys = resources.textures.keys();
						if (ImGui::Combo(
							String(label + "##Tex##Uni##" + value->name).c_str(),
							&index,
							Layout::vector_getter,
							static_cast<void *>(&keys),
							(int32_t)(keys.size())))
						{
							u->data = resources.textures.getByIndex(index);
						}
						toolbar_editable();
					});

				}
				break;
			}

			ImGui::Separator();

			return flag;
		}

		inline static void NewUniform(Material * mat)
		{
			if (ImGui::Button("New Uniform..."))
			{
				ImGui::OpenPopup("New Uniform Editor");
			}
			if (ImGui::BeginPopupModal("New Uniform Editor", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

				static const CString typeList[] =
				{
					"Float",
					"Int",
					"Vec2",
					"Vec3",
					"Vec4",
					"Col4",
					"Mat3",
					"Mat4",
					"Tex",
				};
				static int32_t type = 0;
				static char name[32] = "New_Uniform\0";

				auto closePopup = [&]()
				{
					type = 0;
					std::strcpy(name, "New_Uniform\0");
					ImGui::CloseCurrentPopup();
				};

				ImGui::Combo("Type", &type, typeList, IM_ARRAYSIZE(typeList));
				ImGui::InputText("Name", name, IM_ARRAYSIZE(name), ImGuiInputTextFlags_EnterReturnsTrue);

				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

				if (ImGui::Button("Submit"))
				{
					if (String(name) && !mat->find_any(name))
					{
						uni_base * u = NULL;
						switch (type)
						{
						case uni_base::Flt:  u = new uni_flt	(name, 0);	break;
						case uni_base::Int:  u = new uni_int	(name, 0);	break;
						case uni_base::Vec2: u = new uni_vec2	(name, 0);	break;
						case uni_base::Vec3: u = new uni_vec3	(name, 0);	break;
						case uni_base::Vec4: u = new uni_vec4	(name, 0);	break;
						case uni_base::Col4: u = new uni_col4	(name, 0);	break;
						case uni_base::Mat3: u = new uni_mat3	(name, 0);	break;
						case uni_base::Mat4: u = new uni_mat4	(name, 0);	break;
						case uni_base::Tex:  u = new uni_tex_cp	(name, 0);	break;
						}
						if (u && (u = mat->uniforms().insert({ name, u }).first->second))
						{
							closePopup();
						}
					}
					else
					{
						Debug::logError("A uniform with that name already exists!");
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Cancel"))
				{
					closePopup();
				}

				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

				ImGui::EndPopup();
			}
		}

		/* * * * * * * * * * * * * * * * * * * * */
	};
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Project::Project(EventSystem & eventSystem, bool startOpen)
		: EditorWindow(eventSystem, "Project", startOpen)
	{
	}

	Project::~Project()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Project::drawGui(const GuiEvent & ev)
	{
		if (beginDraw(ImGuiWindowFlags_MenuBar))
		{
			/* * * * * * * * * * * * * * * * * * * * */

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("New (WIP)"))
				{
					if (ImGui::MenuItem("Entity"))		{ /**/ }
					if (ImGui::MenuItem("Font"))		{ /**/ }
					if (ImGui::MenuItem("Image"))		{ /**/ }
					if (ImGui::MenuItem("Lua"))			{ /**/ }
					if (ImGui::MenuItem("Material"))	{ /**/ }
					if (ImGui::MenuItem("Mesh"))		{ /**/ }
					if (ImGui::MenuItem("Model"))		{ /**/ }
					if (ImGui::MenuItem("Script"))		{ /**/ }
					if (ImGui::MenuItem("Shader"))		{ /**/ }
					if (ImGui::MenuItem("Skybox"))		{ /**/ }
					if (ImGui::MenuItem("Sound"))		{ /**/ }
					if (ImGui::MenuItem("Sprite"))		{ /**/ }
					if (ImGui::MenuItem("Surface"))		{ /**/ }
					if (ImGui::MenuItem("Texture"))		{ /**/ }
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			/* * * * * * * * * * * * * * * * * * * * */

			Layout::Columns([&]()
			{
				draw_entity_registry	(ev.resources, ev.resources.entities	);
				draw_font_registry		(ev.resources, ev.resources.fonts		);
				draw_image_registry		(ev.resources, ev.resources.images		);
				draw_mesh_registry		(ev.resources, ev.resources.meshes		);
				draw_material_registry	(ev.resources, ev.resources.materials	);
				draw_model_registry		(ev.resources, ev.resources.models		);
				draw_script_registry	(ev.resources, ev.resources.scripts		);
				draw_shader_registry	(ev.resources, ev.resources.shaders		);
				draw_skybox_registry	(ev.resources, ev.resources.skyboxes	);
				draw_sound_registry		(ev.resources, ev.resources.sounds		);
				draw_sprite_registry	(ev.resources, ev.resources.sprites		);
				draw_surface_registry	(ev.resources, ev.resources.surfaces	);
				draw_texture_registry	(ev.resources, ev.resources.textures	);
			});

			/* * * * * * * * * * * * * * * * * * * * */
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Project::draw_entity_registry(Resources & resources, Registry<Entity> & entities)
	{
		if (entities.empty()) return;

		Layout::Group(entities.name().c_str(), [&]()
		{
			for (auto & pair : entities)
			{
				ImGui::PushID(pair.first.c_str());
				
				Layout::Group(pair.first.c_str(), [&](CString name, Entity * ent)
				{
					// Name
					/* * * * * * * * * * * * * * * * * * * * */
					Layout::Field("Name", [&](CString)
					{
						ImGui::Text("%s", name);
					});

					// Components
					/* * * * * * * * * * * * * * * * * * * * */
					{
						// Transform
						/* * * * * * * * * * * * * * * * * * * * */
						if (Transform * transform = ent->get<Transform>())
						{
							Layout::Group("Transform", [&]()
							{
								Layout::Field("Actions", [&](CString)
								{
									if (ImGui::Selectable("Reset"))
									{
										transform->update(mat4::Identity());
									}
								});

								Layout::Field("Position", [&](CString)
								{
									vec3 pos = transform->getPos();
									if (GUI::EditVec3f("##Position##Transform", pos, 0.01f))
									{
										(*transform)
											.translate(pos - transform->getPos())
											.rotate(0.0f, vec3::One)
											.scale(1.0f)
											;
									}
								});

								Layout::Field("Rotation", [&](CString)
								{
									quat rot = transform->getRot();
									if (GUI::EditQuat("##Rotation##Transform", rot, 0.01f))
									{
										(*transform)
											.translate(0.0f)
											.rotate(rot)
											.scale(1.0f)
											;
									}



								});

								Layout::Field("Euler Angles", [&](CString)
								{
									quat rot = transform->getRot();

									vec3 euler = rot.eulerAngles();
									GUI::EditVec3f("##EulerAngles##Transform", euler);

								});

								Layout::Field("Scale", [&](CString)
								{
									vec3 scl = transform->getScl();
									if (GUI::EditVec3f("##Scale##Transform", scl, 0.01f))
									{
										(*transform)
											.translate(0.0f)
											.rotate(0.0f, vec3::One)
											.scale(scl)
											;
									}
								});

								Layout::Field("Matrix", [&](CString)
								{
									ImGui::NewLine();
									mat4 mat = transform->getMat();
									if (GUI::EditMat4f("##Matrix##Transform", mat, 0.01f))
									{
										transform->update(mat);
									}
									ImGui::NewLine();
								});

							});
						}

						// Camera
						/* * * * * * * * * * * * * * * * * * * * */
						if (Camera * camera = ent->get<Camera>())
						{
							Layout::Group("Camera", [&]()
							{
								Layout::Field("Color", [&](CString)
								{
									ImGui::ColorEdit4("##Color##Camera", &camera->color[0]);
								});
								Layout::Field("FOV", [&](CString)
								{
									ImGui::DragFloat("##FOV##Camera", &camera->fov, 0.5f, 10.f, 100.f);
								});
								Layout::Group("Perspective", [&]()
								{
									Layout::Field("Near", [&](CString)
									{
										ImGui::DragFloat("##pNear##Camera", &camera->pNear, 0.5f, 10.f, camera->pFar - 1.f);
									});
									Layout::Field("Far", [&](CString)
									{
										ImGui::DragFloat("##pFar##Camera", &camera->pFar, 0.5f, camera->pNear + 1.f, 1000.0f);
									});
									Layout::Field("Matrix", [&](CString)
									{
										mat4 mat = camera->getPerspMatrix();
										GUI::EditMat4f("##O##Matrix##Camera", mat);
									});
								});
								Layout::Group("Orthographic", [&]()
								{
									Layout::Field("Near", [&](CString)
									{
										ImGui::DragFloat("##oNear##Camera", &camera->oNear, 0.5f, 10.f, camera->oFar - 1.f);
									});
									Layout::Field("Far", [&](CString)
									{
										ImGui::DragFloat("##oFar##Camera", &camera->oFar, 0.5f, camera->oNear + 1.f, 100.f);
									});
									Layout::Field("Matrix", [&](CString)
									{
										mat4 mat = camera->getOrthoMatrix();
										GUI::EditMat4f("##O##Matrix##Camera", mat);
									});
								});
							});
						}

						// Light
						/* * * * * * * * * * * * * * * * * * * * */
						if (Light * light = ent->get<Light>())
						{
							Layout::Group("Light", [&]()
							{
								Layout::Field("Color", [&](CString)
								{
									ImGui::ColorEdit4("##Color##Light", &light->color[0]);
								});
							});
						}

						// Renderer
						/* * * * * * * * * * * * * * * * * * * * */
						if (Renderer * renderer = ent->get<Renderer>())
						{
							Layout::Group("Renderer", [&]()
							{
								// Model
								Layout::Field("Model", [&](CString)
								{
									List<String> keys = resources.models.keys();
									int32_t index = resources.models.getIndexOf((Model *)(renderer->drawable()));
									if (ImGui::Combo(
										"##Model##Renderer",
										&index,
										Layout::vector_getter,
										static_cast<void *>(&keys),
										(int32_t)(keys.size())))
									{
										if (const Model * value = resources.models.getByIndex(index))
										{
											renderer->drawable() = value;
										}
									}
								});

								// States
								for (auto & pair : renderer->states())
								{
									switch (pair.first)
									{
									case GL::AlphaTest:
										Layout::Field("Alpha Test", [&](CString)
										{
											ImGui::Checkbox("##AlphaTest##States", (bool *)(pair.second.ptr()));
										});
										break;
									case GL::Blend:
										Layout::Field("Blend", [&](CString)
										{
											ImGui::Checkbox("##Blend##States", (bool *)(pair.second.ptr()));
										});
										break;
									case GL::CullFace:
										Layout::Field("Cull Face", [&](CString)
										{
											ImGui::Checkbox("##CullFace##States", (bool *)(pair.second.ptr()));
										});
										break;
									case GL::DepthTest:
										Layout::Field("Depth Test", [&](CString)
										{
											ImGui::Checkbox("##DepthTest##States", (bool *)(pair.second.ptr()));
										});
										break;
									case GL::Multisample:
										Layout::Field("Multisample", [&](CString)
										{
											ImGui::Checkbox("##Multisample##States", (bool *)(pair.second.ptr()));
										});
										break;
									case GL::Texture2D:
										Layout::Field("Texture2D", [&](CString)
										{
											ImGui::Checkbox("##Texture2D##States", (bool *)(pair.second.ptr()));
										});
										break;
									}
								}
							});
						}

						// Rigidbody
						/* * * * * * * * * * * * * * * * * * * * */
						if (Rigidbody * rb = ent->get<Rigidbody>())
						{
							Layout::Group("Rigidbody", [&]()
							{
								Layout::Field("Enabled", [&](CString)
								{
									ImGui::Checkbox("##Enabled##Rigidbody", &rb->enabled);
								});

								Layout::Field("Index", [&](CString)
								{
									ImGui::Text("%i", rb->index());
								});

								Layout::Group("Motion Controller", [&]()
								{
									Layout::Field("Actions", [&](CString)
									{
										if (ImGui::Selectable("Reset##Particle##Rigidbody"))
										{
											rb->particle()->reset();
										}
									});
									Layout::Field("Mass",					[&](CString) {	ImGui::DragFloat("##Mass##Particle##Rigidbody",							&rb->particle()->mass);});
									Layout::Field("Mass Inv",				[&](CString) {	ImGui::DragFloat("##MassInv##Particle##Rigidbody",						&rb->particle()->massInv);});
									Layout::Field("Position",				[&](CString) {	GUI::EditVec3f("##Position##Particle##Rigidbody",						rb->particle()->pos);});
									Layout::Field("Velocity",				[&](CString) {	GUI::EditVec3f("##Velocity##Particle##Rigidbody",						rb->particle()->vel);});
									Layout::Field("Acceleration",			[&](CString) {	GUI::EditVec3f("##Acceleration##Particle##Rigidbody",					rb->particle()->acc);});
									Layout::Field("Force",					[&](CString) {	GUI::EditVec3f("##Force##Particle##Rigidbody",							rb->particle()->force);});
									Layout::Field("Momentum",				[&](CString) {	GUI::EditVec3f("##Momentum##Particle##Rigidbody",						rb->particle()->momentum);});
									Layout::Field("Rotation",				[&](CString) {	GUI::EditQuat ("##Rotation##Particle##Rigidbody",						rb->particle()->rotation);});
									Layout::Field("Angular Velocity",		[&](CString) {	GUI::EditVec3f("##Angular Velocity##Particle##Rigidbody",				rb->particle()->angularVel);});
									Layout::Field("Angular Acceleration",	[&](CString) {	GUI::EditVec3f("##Angular Acceleration##Particle##Rigidbody",			rb->particle()->angularAcc);});
									Layout::Field("Angular Momentum",		[&](CString) {	GUI::EditVec3f("##Angular Momentum##Particle##Rigidbody",				rb->particle()->angularMomentum);});
									Layout::Field("Torque",					[&](CString) {	GUI::EditVec3f("##Torque##Particle##Rigidbody",							rb->particle()->torque);});
									Layout::Field("Inirtia Tensor",			[&](CString) {	GUI::EditMat3f("##Inirtia Tensor##Particle##Rigidbody",					rb->particle()->inertiaTensor);});
									Layout::Field("IT Inverse",				[&](CString) {	GUI::EditMat3f("##Inirtia Tensor Inverse##Particle##Rigidbody",			rb->particle()->inertiaTensorInv);});
									Layout::Field("IT World",				[&](CString) {	GUI::EditMat3f("##Inirtia Tensor World##Particle##Rigidbody",			rb->particle()->inertiaTensor_world);});
									Layout::Field("IT World Inverse",		[&](CString) {	GUI::EditMat3f("##Inirtia Tensor World Inverse##Particle##Rigidbody",	rb->particle()->inertiaTensorInv_world);});
									Layout::Field("Center Mass",			[&](CString) {	GUI::EditVec3f("##Center Mass##Particle##Rigidbody",					rb->particle()->centerMass);});
									Layout::Field("Center Mass World",		[&](CString) {	GUI::EditVec3f("##Center Mass World##Particle##Rigidbody",				rb->particle()->centerMass_world);});

								});

								Layout::Field("Collider", [&](CString)
								{
									ImGui::Text("%s", rb->collider() ? "OK" : "NULL");
								});

								Layout::Field("Transform", [&](CString)
								{
									ImGui::Text("%s", rb->transform() ? "OK" : "NULL");
								});
							});
						}
					}

					// File
					/* * * * * * * * * * * * * * * * * * * * */
					if (const String file = entities.getFile(name))
					{
						Layout::Field("File", [&](CString)
						{
							const String fName = ML_FS.getFileName(file);
							if (ImGui::Selectable(fName.c_str()))
							{
								OS::execute("open", ML_FS.getPathTo(file));
							}
						});
						Layout::Field("Path", [&](CString)
						{
							const String fPath = ML_FS.getFilePath(file);
							if (ImGui::Selectable(fPath.c_str()))
							{
								OS::execute("open", ML_FS.getPathTo(fPath));
							}
						});
					}

					/* * * * * * * * * * * * * * * * * * * * */

				}, pair.first.c_str(), pair.second);
			
				ImGui::PopID();
			}
		});
	}

	void Project::draw_font_registry(Resources & resources, Registry<Font> & fonts)
	{
		if (fonts.empty()) return;

		Layout::Group(fonts.name().c_str(), [&]()
		{
			for (auto & pair : fonts)
			{
				Layout::Group(pair.first.c_str(), [&](CString name, const Font * e)
				{
					Layout::Field("Name", [&](CString label)
					{
						ImGui::Selectable(name);
					});
					Layout::Field("Family", [&](CString label)
					{
						ImGui::Selectable(e->getInfo().family.c_str());
					});
					if (const String file = fonts.getFile(name))
					{
						Layout::Field("File", [&](CString)
						{
							const String fName = ML_FS.getFileName(file);
							if (ImGui::Selectable(fName.c_str()))
							{
								OS::execute("open", ML_FS.getPathTo(file));
							}
						});
						Layout::Field("Path", [&](CString)
						{
							const String fPath = ML_FS.getFilePath(file);
							if (ImGui::Selectable(fPath.c_str()))
							{
								OS::execute("open", ML_FS.getPathTo(fPath));
							}
						});
					}

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	void Project::draw_image_registry(Resources & resources, Registry<Image> & images)
	{
		if (images.empty()) return;

		Layout::Group(images.name().c_str(), [&]()
		{
			for (auto & pair : images)
			{
				Layout::Group(pair.first.c_str(), [&](CString name, const Image * e)
				{
					Layout::Field("Name", [&](CString label)
					{
						ImGui::Text("%s", name);
					});
					Layout::Field("Size", [&](CString label)
					{
						ImGui::Text("%u x %u", e->width(), e->height());
					});
					if (const String file = images.getFile(name))
					{
						Layout::Field("File", [&](CString)
						{
							const String fName = ML_FS.getFileName(file);
							if (ImGui::Selectable(fName.c_str()))
							{
								OS::execute("open", ML_FS.getPathTo(file));
							}
						});
						Layout::Field("Path", [&](CString)
						{
							const String fPath = ML_FS.getFilePath(file);
							if (ImGui::Selectable(fPath.c_str()))
							{
								OS::execute("open", ML_FS.getPathTo(fPath));
							}
						});
					}

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	void Project::draw_material_registry(Resources & resources, Registry<Material> & materials)
	{
		if (materials.empty()) return;

		Layout::Group(materials.name().c_str(), [&]()
		{
			for (auto & pair : materials)
			{
				Layout::Group(pair.first.c_str(), [&](CString name, Material * mat)
				{
					// Name
					Layout::Field("Name", [&](CString)
					{
						ImGui::Text("%s", name);
					});

					// Shader
					Layout::Field("Shader", [&](CString)
					{
						List<String> keys = resources.shaders.keys();
						int32_t index = resources.shaders.getIndexOf(mat->shader());
						if (ImGui::Combo(
							"##Shader##Renderer",
							&index,
							Layout::vector_getter,
							static_cast<void *>(&keys),
							(int32_t)(keys.size())))
						{
							if (const Shader * value = resources.shaders.getByIndex(index))
							{
								mat->shader() = value;
							}
						}
					});

					// Uniforms
					Layout::Group("Uniforms", [&]()
					{
						static bool show_constants = true;

						ImGui::Checkbox("Show Constants", &show_constants);

						Layout::NewUniform(mat);

						if (!mat->uniforms().empty())
						{
							ImGui::Separator();
						}

						std::vector<Material::UniformMap::iterator> toRemove;

						for (auto it = mat->uniforms().begin(); it != mat->uniforms().end(); it++)
						{
							const String label("##" + String(name) + "##Uniform##" + it->first);

							const int32_t flag = Layout::EditUniform(resources, label, it->second, show_constants);

							if (flag == 1)
							{
								toRemove.push_back(it);
							}
						}

						for (auto it : toRemove)
						{
							uni_base * u = it->second;
							mat->uniforms().erase(it);
							delete u;
						}

					});

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	void Project::draw_mesh_registry(Resources & resources, Registry<Mesh> & meshes)
	{
		if (meshes.empty()) return;

		Layout::Group(meshes.name().c_str(), [&]()
		{
			for (auto & pair : meshes)
			{
				Layout::Group(pair.first.c_str(), [&](CString name, const Mesh * e)
				{
					Layout::Field("Name", [&](CString label)
					{
						ImGui::Text("%s", name);
					});
					if (const String file = meshes.getFile(name))
					{
						Layout::Field("File", [&](CString)
						{
							const String fName = ML_FS.getFileName(file);
							if (ImGui::Selectable(fName.c_str()))
							{
								OS::execute("open", ML_FS.getPathTo(file));
							}
						});
						Layout::Field("Path", [&](CString)
						{
							const String fPath = ML_FS.getFilePath(file);
							if (ImGui::Selectable(fPath.c_str()))
							{
								OS::execute("open", ML_FS.getPathTo(fPath));
							}
						});
					}

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	void Project::draw_model_registry(Resources & resources, Registry<Model> & models)
	{
		if (models.empty()) return;

		Layout::Group(models.name().c_str(), [&]()
		{
			for (auto & pair : models)
			{
				Layout::Group(pair.first.c_str(), [&](CString name, const Model * e)
				{
					Layout::Field("Name", [&](CString label)
					{
						ImGui::Text("%s", name);
					});
					if (const String file = models.getFile(name))
					{
						Layout::Field("File", [&](CString)
						{
							const String fName = ML_FS.getFileName(file);
							if (ImGui::Selectable(fName.c_str()))
							{
								OS::execute("open", ML_FS.getPathTo(file));
							}
						});
						Layout::Field("Path", [&](CString)
						{
							const String fPath = ML_FS.getFilePath(file);
							if (ImGui::Selectable(fPath.c_str()))
							{
								OS::execute("open", ML_FS.getPathTo(fPath));
							}
						});
					}

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	void Project::draw_script_registry(Resources & resources, Registry<Script> & scripts)
	{
		if (scripts.empty()) return;

		Layout::Group(scripts.name().c_str(), [&]()
		{
			for (auto & pair : scripts)
			{
				Layout::Group(pair.first.c_str(), [&](CString name, Script * scr)
				{
					Layout::Field("Name", [&](CString)
					{
						ImGui::Text("%s", name);
					});
					Layout::Field("Actions", [&](CString)
					{
						if (ImGui::Selectable("Build"))
						{
							scr->build({});
						}
						if (ImGui::Selectable("Run"))
						{
							scr->run();
						}
						if (ImGui::Selectable("Build & Run"))
						{
							scr->buildAndRun({});
						}
					});
					if (const String file = scripts.getFile(name))
					{
						Layout::Field("File", [&](CString)
						{
							const String fName = ML_FS.getFileName(file);
							if (ImGui::Selectable(fName.c_str()))
							{
								OS::execute("open", ML_FS.getPathTo(file));
							}
						});
						Layout::Field("Path", [&](CString)
						{
							const String fPath = ML_FS.getFilePath(file);
							if (ImGui::Selectable(fPath.c_str()))
							{
								OS::execute("open", ML_FS.getPathTo(fPath));
							}
						});
					}

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	void Project::draw_shader_registry(Resources & resources, Registry<Shader> & shaders)
	{
		if (shaders.empty()) return;

		Layout::Group(shaders.name().c_str(), [&]()
		{
			for (auto & pair : shaders)
			{
				Layout::Group(pair.first.c_str(), [&](CString name, const Shader * e)
				{
					Layout::Field("Name", [&](CString label)
					{
						ImGui::Text("%s", name);
					});
					if (const String file = shaders.getFile(name))
					{
						Layout::Field("File", [&](CString)
						{
							const String fName = ML_FS.getFileName(file);
							if (ImGui::Selectable(fName.c_str()))
							{
								OS::execute("open", ML_FS.getPathTo(file));
							}
						});
						Layout::Field("Path", [&](CString)
						{
							const String fPath = ML_FS.getFilePath(file);
							if (ImGui::Selectable(fPath.c_str()))
							{
								OS::execute("open", ML_FS.getPathTo(fPath));
							}
						});
					}

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	void Project::draw_skybox_registry(Resources & resources, Registry<Skybox> & skyboxes)
	{
		if (skyboxes.empty()) return;

		Layout::Group(skyboxes.name().c_str(), [&]()
		{
			for (auto & pair : skyboxes)
			{
				Layout::Group(pair.first.c_str(), [&](CString name, const Skybox * e)
				{
					Layout::Field("Name", [&](CString label)
					{
						ImGui::Text("%s", name);
					});
					if (const String file = skyboxes.getFile(name))
					{
						Layout::Field("File", [&](CString)
						{
							const String fName = ML_FS.getFileName(file);
							if (ImGui::Selectable(fName.c_str()))
							{
								OS::execute("open", ML_FS.getPathTo(file));
							}
						});
						Layout::Field("Path", [&](CString)
						{
							const String fPath = ML_FS.getFilePath(file);
							if (ImGui::Selectable(fPath.c_str()))
							{
								OS::execute("open", ML_FS.getPathTo(fPath));
							}
						});
					}

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	void Project::draw_sound_registry(Resources & resources, Registry<Sound> & sounds)
	{
		if (sounds.empty()) return;

		Layout::Group(sounds.name().c_str(), [&]()
		{
			for (auto & pair : sounds)
			{
				Layout::Group(pair.first.c_str(), [&](CString name, const Sound * e)
				{
					Layout::Field("Name", [&](CString label)
					{
						ImGui::Text("%s", name);
					});
					if (const String file = sounds.getFile(name))
					{
						Layout::Field("File", [&](CString)
						{
							const String fName = ML_FS.getFileName(file);
							if (ImGui::Selectable(fName.c_str()))
							{
								OS::execute("open", ML_FS.getPathTo(file));
							}
						});
						Layout::Field("Path", [&](CString)
						{
							const String fPath = ML_FS.getFilePath(file);
							if (ImGui::Selectable(fPath.c_str()))
							{
								OS::execute("open", ML_FS.getPathTo(fPath));
							}
						});
					}

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	void Project::draw_sprite_registry(Resources & resources, Registry<Sprite> & sprites)
	{
		if (sprites.empty()) return;

		Layout::Group(sprites.name().c_str(), [&]()
		{
			for (auto & pair : sprites)
			{
				Layout::Group(pair.first.c_str(), [&](CString name, Sprite * spr)
				{
					Layout::Field("Name", [&](CString label)
					{
						ImGui::Text("%s", name);
					});
					Layout::Field("Color", [&](CString label)
					{
						auto temp = spr->color();
						if (ImGui::ColorEdit4("##Color##Sprite", &temp[0]))
						{
							spr->setColor(temp);
						}
					});
					Layout::Field("Origin", [&](CString label)
					{
						auto temp = spr->origin();
						if (GUI::EditVec2f("##Origin##Sprite", temp))
						{
							spr->setOrigin(temp);
						}
					});
					Layout::Field("Position", [&](CString label)
					{
						auto temp = spr->position();
						if (GUI::EditVec2f("##Position##Sprite", temp))
						{
							spr->setPosition(temp);
						}
					});
					Layout::Field("Rotation", [&](CString label)
					{
						auto temp = spr->rotation();
						if (ImGui::DragFloat("##Rotation##Sprite", &temp, 0.5f))
						{
							spr->setRotation(temp);
						}
					});
					Layout::Field("Scale", [&](CString label)
					{
						auto temp = spr->scale();
						if (GUI::EditVec2f("##Scale##Sprite", temp))
						{
							spr->setScale(temp);
						}
					});
					Layout::Field("Texture", [&](CString label)
					{
						int32_t index = resources.textures.getIndexOf(spr->texture());
						List<String> keys = resources.textures.keys();
						if (ImGui::Combo(
							"##Tex2D##Value",
							&index,
							Layout::vector_getter,
							static_cast<void *>(&keys),
							(int32_t)(keys.size())))
						{
							if (const Texture * value = resources.textures.getByIndex(index))
							{
								spr->setTexture(value);
							}
						}
					});
					if (const String file = sprites.getFile(name))
					{
						Layout::Field("File", [&](CString)
						{
							const String fName = ML_FS.getFileName(file);
							if (ImGui::Selectable(fName.c_str()))
							{
								OS::execute("open", ML_FS.getPathTo(file));
							}
						});
						Layout::Field("Path", [&](CString)
						{
							const String fPath = ML_FS.getFilePath(file);
							if (ImGui::Selectable(fPath.c_str()))
							{
								OS::execute("open", ML_FS.getPathTo(fPath));
							}
						});
					}

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	void Project::draw_surface_registry(Resources & resources, Registry<Surface> & surfaces)
	{
		if (surfaces.empty()) return;

		Layout::Group(surfaces.name().c_str(), [&]()
		{
			for (auto & pair : surfaces)
			{
				Layout::Group(pair.first.c_str(), [&](CString name, const Surface * e)
				{
					Layout::Field("Name", [&](CString label)
					{
						ImGui::Text("%s", name);
					});
					if (const String file = surfaces.getFile(name))
					{
						Layout::Field("File", [&](CString)
						{
							const String fName = ML_FS.getFileName(file);
							if (ImGui::Selectable(fName.c_str()))
							{
								OS::execute("open", ML_FS.getPathTo(file));
							}
						});
						Layout::Field("Path", [&](CString)
						{
							const String fPath = ML_FS.getFilePath(file);
							if (ImGui::Selectable(fPath.c_str()))
							{
								OS::execute("open", ML_FS.getPathTo(fPath));
							}
						});
					}

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	void Project::draw_texture_registry(Resources & resources, Registry<Texture> & textures)
	{
		if (textures.empty()) return;

		Layout::Group(textures.name().c_str(), [&]()
		{
			for (auto & pair : textures)
			{
				Layout::Group(pair.first.c_str(), [&](CString name, Texture * tex)
				{
					Layout::Field("Name", [&](CString label)
					{
						ImGui::Text("%s", name);
					});
					Layout::Field("Size", [&](CString label)
					{
						ImGui::Text("%u x %u", tex->width(), tex->height());
					});
					Layout::Field("Real Size", [&](CString label)
					{
						ImGui::Text("%u x %u", tex->realWidth(), tex->realHeight());
					});
					Layout::Field("Mipmapped", [&](CString label)
					{
						bool temp = tex->mipmapped();
						if (ImGui::Checkbox("##Texture##Mipmapped", &temp))
						{
							tex->setMipmapped(temp);
						}
					});
					Layout::Field("Repeated", [&](CString label)
					{
						bool temp = tex->repeated();
						if (ImGui::Checkbox("##Texture##Repeated", &temp))
						{
							tex->setRepeated(temp);
						}
					});
					Layout::Field("Smooth", [&](CString label)
					{
						bool temp = tex->smooth();
						if (ImGui::Checkbox("##Texture##Smooth", &temp))
						{
							tex->setSmooth(temp);
						}
					});

					if (const String file = textures.getFile(name))
					{
						Layout::Field("File", [&](CString)
						{
							const String fName = ML_FS.getFileName(file);
							if (ImGui::Selectable(fName.c_str()))
							{
								OS::execute("open", ML_FS.getPathTo(file));
							}
						});
						Layout::Field("Path", [&](CString)
						{
							const String fPath = ML_FS.getFilePath(file);
							if (ImGui::Selectable(fPath.c_str()))
							{
								OS::execute("open", ML_FS.getPathTo(fPath));
							}
						});
					}

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	/* * * * * * * * * * * * * * * * * * * * */
}