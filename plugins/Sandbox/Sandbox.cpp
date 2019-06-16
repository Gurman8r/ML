/* * * * * * * * * * * * * * * * * * * * */

#include "Sandbox.hpp"

#include <ML/Audio/OpenAL.hpp>
#include <ML/Audio/Sound.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/FileSystem.hpp> 
#include <ML/Core/OS.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/EditorUtility.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Engine/GameTime.hpp>
#include <ML/Core/SharedLibrary.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Engine/Resources.hpp>
#include <ML/Graphics/Camera.hpp>
#include <ML/Graphics/Font.hpp>
#include <ML/Graphics/Light.hpp>
#include <ML/Graphics/Model.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Graphics/Renderer.hpp>
#include <ML/Graphics/Sprite.hpp>
#include <ML/Graphics/Surface.hpp>
#include <ML/Graphics/Uniform.hpp>
#include <ML/Graphics/RenderWindow.hpp>
#include <ML/Network/NetClient.hpp>
#include <ML/Network/NetServer.hpp>
#include <ML/Engine/PhysicsWorld.hpp>
#include <ML/Engine/PhysicsState.hpp>
#include <ML/Engine/Collider.hpp>
#include <ML/Engine/Particle.hpp>
#include <ML/Engine/Rigidbody.hpp>
#include <ML/Engine/SphereCollider.hpp>
#include <ML/Engine/BoxCollider.hpp>
#include <ML/Window/WindowEvents.hpp>

/* * * * * * * * * * * * * * * * * * * * */

ML_PLUGIN_API ml::Plugin * ML_Plugin_Main(ml::EventSystem & eventSystem)
{
	return new DEMO::Sandbox(eventSystem);
}

/* * * * * * * * * * * * * * * * * * * * */

namespace DEMO
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Sandbox::Sandbox(ml::EventSystem & eventSystem)
		: EditorPlugin(eventSystem)
	{
		eventSystem.addListener(ml::KeyEvent::ID, this);
		eventSystem.addListener(ml::MainMenuBarEvent::ID, this);
		eventSystem.addListener(ml::BuildDockspaceEvent::ID, this);
	}

	Sandbox::~Sandbox() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Sandbox::onEvent(const ml::Event * value)
	{
		// Handle base events
		ml::EditorPlugin::onEvent(value);

		switch (*value)
		{
			// Key Event
			/* * * * * * * * * * * * * * * * * * * * */
		case ml::KeyEvent::ID:
			if (auto ev = value->as<ml::KeyEvent>())
			{
			}
			break;

			// Main Menu Bar Event
			/* * * * * * * * * * * * * * * * * * * * */
		case ml::MainMenuBarEvent::ID:
			if (auto ev = value->as<ml::MainMenuBarEvent>())
			{
				switch (ev->menu)
				{
				case ml::MainMenuBarEvent::File:
					break;
				case ml::MainMenuBarEvent::Edit:
					break;
				case ml::MainMenuBarEvent::Window:
					ImGui::Separator();
					ImGui::MenuItem("Sandbox Scene", "", &sandbox.showScene);
					ImGui::MenuItem("Sandbox Inspector", "", &sandbox.showInspector);
					break;
				case ml::MainMenuBarEvent::Help:
					break;
				case ml::MainMenuBarEvent::None:
					break;
				}
			}
			break;

			// Build Dockspace Event
			/* * * * * * * * * * * * * * * * * * * * */
		case ml::BuildDockspaceEvent::ID:
			if (auto ev = value->as<ml::BuildDockspaceEvent>())
			{
				ml::DockspaceGui & d = ev->dockspace;
				//d.dockWindow("Sandbox Inspector", d.getNode(d.Right));
				d.dockWindow("Sandbox Scene", d.getNode(d.MidUp));
			}
			break;
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Sandbox::onEnter(const ml::EnterEvent & ev)
	{
		// Hello!
		ml::Debug::log("Hello from {0}!", (*this));
	}

	void Sandbox::onStart(const ml::StartEvent & ev)
	{
		// Buffers
		/* * * * * * * * * * * * * * * * * * * * */
		sandbox.vao.create(ml::GL::Triangles).bind();
		sandbox.vbo.create(ml::GL::DynamicDraw).bind();
		sandbox.vbo.bufferData(NULL, ml::Shapes::RectQuad::Size);
		ml::BufferLayout::Default.bind();
		sandbox.vbo.unbind();
		sandbox.vao.unbind();

		// Sprites
		/* * * * * * * * * * * * * * * * * * * * */
		if (ml::Sprite * spr = ev.resources.sprites.get("neutrino"))
		{
			spr->setPosition(ml::vec2(0.95f, 0.925f) * ev.window.getSize())
				.setScale	(0.5f)
				.setRotation(0.0f)
				.setOrigin	(0.5f)
				.setColor	(ml::Color::White);
		}

		// Camera
		/* * * * * * * * * * * * * * * * * * * * */
		if (ml::Entity * ent = ev.resources.entities.get("camera"))
		{
			sandbox.camera = ent;

			ml::Camera * camera = ent->add<ml::Camera>();
			camera->color = { ml::vec3 { 0.198f }, 1.0f };
			camera->position = { 0.0f, 1.0f, 10.0f };
			camera->forward(ml::vec3::Back);

			ml::Transform * transform = ent->add<ml::Transform>();
			transform->lookAt(camera->position, camera->forward());
		}

		// Light
		/* * * * * * * * * * * * * * * * * * * * */
		if (ml::Entity * ent = ev.resources.entities.get("light"))
		{
			sandbox.light = ent;

			ml::Transform * transform = ent->add<ml::Transform>(
				ml::vec3 { 0.0f, 1.0f, 30.0f }, // position
				ml::vec3 { 1.0f }, // scale
				ml::quat { } // rotation
			);

			ml::Light * light = ent->add<ml::Light>(
				ml::Color::LightYellow
			);

			const ml::Material * material = ev.resources.materials.create(
				"mat_light",
				ev.resources.shaders.get("solid"),
				ml::List<ml::uni_base *>({
					new ml::uni_mat4_cr	("Vert.proj",		sandbox.camera->get<ml::Camera>()->getPerspMatrix()),
					new ml::uni_mat4_cr	("Vert.view",		sandbox.camera->get<ml::Transform>()->getMat()),
					new ml::uni_mat4_cr	("Vert.model",		transform->getMat()),
					new ml::uni_col4_cr	("Frag.mainCol",	light->color),
					}));

			ml::Renderer * renderer = ent->add<ml::Renderer>(
				ev.resources.models.get("sphere8x6"),
				material
			);
		}

		// Borg
		/* * * * * * * * * * * * * * * * * * * * */
		if (ml::Entity * ent = ev.resources.entities.get("borg"))
		{
			ml::Transform * transform = ent->add<ml::Transform>(
				ml::vec3 { 5.0f, 0.0f, 0.0f }, // position
				ml::vec3 { 1.0f }, // scale
				ml::quat { } // rotation
			);

			ml::BoxCollider * collider = ent->add<ml::BoxCollider>(
				transform->getScl() // size
			);

			ml::Particle * particle = ent->add<ml::Particle>(
				transform->getPos(), // position
				1.0f // mass
			);

			ml::Rigidbody * rb = ent->add<ml::Rigidbody>(sandbox.physWorld.createNewRigidbody(
				RB_BORG, transform, collider, particle
			));

			const ml::Material * material = ev.resources.materials.create(
				"mat_borg",
				ev.resources.shaders.get("basic"),
				ml::List<ml::uni_base *>({
					new ml::uni_mat4_cr	("Vert.proj",		sandbox.camera->get<ml::Camera>()->getPerspMatrix()),
					new ml::uni_mat4_cr	("Vert.view",		sandbox.camera->get<ml::Transform>()->getMat()),
					new ml::uni_mat4_cr	("Vert.model",		transform->getMat()),
					new ml::uni_col4	("Frag.mainCol",	ml::Color::White),
					new ml::uni_tex2	("Frag.mainTex",	ev.resources.textures.get("borg")),
					}));

			ml::Renderer * renderer = ent->add<ml::Renderer>(
				ev.resources.models.get("default_cube"),
				material
			);
		}

		// Cube
		/* * * * * * * * * * * * * * * * * * * * */
		if (ml::Entity * ent = ev.resources.entities.get("cube"))
		{
			ml::Transform * transform = ent->add<ml::Transform>(
				ml::vec3 { 0.0f, 0.0f, -5.0f }, // position
				ml::vec3 { 0.5f }, // scale
				ml::quat { } // rotation
			);

			ml::BoxCollider * collider = ent->add<ml::BoxCollider>(
				transform->getScl() // size
			);

			ml::Particle * particle = ent->add<ml::Particle>(
				transform->getPos(), // position
				1.0f // mass
			);

			ml::Rigidbody * rb = ent->add<ml::Rigidbody>(sandbox.physWorld.createNewRigidbody(
				RB_CUBE, transform, collider, particle
			));

			const ml::Material * material = ev.resources.materials.create(
				"mat_cube",
				ev.resources.shaders.get("normal"),
				ml::List<ml::uni_base *>({
					new ml::uni_mat4_cr	("Vert.proj",		sandbox.camera->get<ml::Camera>()->getPerspMatrix()),
					new ml::uni_mat4_cr	("Vert.view",		sandbox.camera->get<ml::Transform>()->getMat()),
					new ml::uni_mat4_cr	("Vert.model",		transform->getMat()),
					new ml::uni_col4	("Frag.mainCol",	ml::Color::White),
					new ml::uni_tex2	("Frag.mainTex",	ev.resources.textures.get("stone_dm")),
					}));

			ml::Renderer * renderer = ent->add<ml::Renderer>(
				ev.resources.models.get("cube"),
				material
			);
		}

		// Navball
		/* * * * * * * * * * * * * * * * * * * * */
		if (ml::Entity * ent = ev.resources.entities.get("navball"))
		{
			ml::Transform * transform = ent->add<ml::Transform>(
				ml::vec3 { -5.0f, 0.0f, 0.0f }, // position
				ml::vec3 { 0.5f }, // scale
				ml::quat { } // rotation
			);

			ml::SphereCollider * collider = ent->add<ml::SphereCollider>(
				transform->getScl()[1] // radius
			);

			ml::Particle * particle = ent->add<ml::Particle>(
				transform->getPos(), // position
				1.0f // mass
			);

			ml::Rigidbody * rb = ent->add<ml::Rigidbody>(sandbox.physWorld.createNewRigidbody(
				RB_NAVBALL, transform, collider, particle
			));

			const ml::Material * material = ev.resources.materials.create(
				"mat_navball",
				ev.resources.shaders.get("basic"),
				ml::List<ml::uni_base *>({
					new ml::uni_mat4_cr	("Vert.proj",		sandbox.camera->get<ml::Camera>()->getPerspMatrix()),
					new ml::uni_mat4_cr	("Vert.view",		sandbox.camera->get<ml::Transform>()->getMat()),
					new ml::uni_mat4_cr	("Vert.model",		transform->getMat()),
					new ml::uni_col4	("Frag.mainCol",	ml::Color::White),
					new ml::uni_tex2	("Frag.mainTex",	ev.resources.textures.get("navball")),
					}));

			ml::Renderer * renderer = ent->add<ml::Renderer>(
				ev.resources.models.get("sphere32x24"),
				material
			);
		}

		// Moon
		/* * * * * * * * * * * * * * * * * * * * */
		if (ml::Entity * ent = ev.resources.entities.get("moon"))
		{
			ml::Transform * transform = ent->add<ml::Transform>(
				ml::vec3 { 0.0f, 0.0f, 5.0f }, // position
				ml::vec3 { 0.5f }, // scale
				ml::quat { } // rotation
			);

			ml::SphereCollider * collider = ent->add<ml::SphereCollider>(
				transform->getScl()[1] // radius
			);

			ml::Particle * particle = ent->add<ml::Particle>(
				transform->getPos(), // position
				1.0f // mass
			);

			ml::Rigidbody * rb = ent->add<ml::Rigidbody>(sandbox.physWorld.createNewRigidbody(
				RB_MOON, transform, collider, particle
			));

			const ml::Material * material = ev.resources.materials.create(
				"mat_moon",
				ev.resources.shaders.get("lighting"),
				ml::List<ml::uni_base *> ({
					new ml::uni_mat4_cr	("Vert.proj",		sandbox.camera->get<ml::Camera>()->getPerspMatrix()),
					new ml::uni_mat4_cr	("Vert.view",		sandbox.camera->get<ml::Transform>()->getMat()),
					new ml::uni_mat4_cr	("Vert.model",		transform->getMat()),
					new ml::uni_vec3_cr	("Frag.cameraPos",	sandbox.camera->get<ml::Transform>()->getPos()),
					new ml::uni_vec3_cr	("Frag.lightPos",	sandbox.light->get<ml::Transform>()->getPos()),
					new ml::uni_col4_cr	("Frag.diffuse",	sandbox.light->get<ml::Light>()->color),
					new ml::uni_col4	("Frag.mainCol",	ml::Color::White),
					new ml::uni_tex2	("Frag.mainTex",	ev.resources.textures.get("moon_dm")),
					new ml::uni_tex2	("Frag.specTex",	ev.resources.textures.get("moon_nm")),
					new ml::uni_flt		("Frag.ambient",	0.01f),
					new ml::uni_flt		("Frag.specular",	0.1f),
					new ml::uni_int		("Frag.shininess",	8),
					}));

			ml::Renderer * renderer = ent->add<ml::Renderer>(
				ev.resources.models.get("sphere32x24"),
				material
			);
		}

		// Earth
		/* * * * * * * * * * * * * * * * * * * * */
		if (ml::Entity * ent = ev.resources.entities.get("earth"))
		{
			ml::Transform * transform = ent->add<ml::Transform>(
				ml::vec3 { 0.0f }, // position
				ml::vec3 { 1.0f }, // scale
				ml::quat { } // rotation
			);

			ml::SphereCollider * collider = ent->add<ml::SphereCollider>(
				transform->getScl()[0] // radius
			);
			collider->center_world = transform->getPos();

			ml::Particle * particle = ent->add<ml::Particle>(
				transform->getPos(), // position
				0.25f // mass
			);
			ml::Rigidbody * rb = ent->add<ml::Rigidbody>(sandbox.physWorld.createNewRigidbody(
				RB_EARTH, transform, collider, particle
			));

			const ml::Material * material = ev.resources.materials.create(
				"mat_earth",
				ev.resources.shaders.get("lighting"),
				ml::List<ml::uni_base *> ({
					new ml::uni_mat4_cr	("Vert.proj",		sandbox.camera->get<ml::Camera>()->getPerspMatrix()),
					new ml::uni_mat4_cr	("Vert.view",		sandbox.camera->get<ml::Transform>()->getMat()),
					new ml::uni_mat4_cr	("Vert.model",		transform->getMat()),
					new ml::uni_vec3_cr	("Frag.cameraPos",	sandbox.camera->get<ml::Transform>()->getPos()),
					new ml::uni_vec3_cr	("Frag.lightPos",	sandbox.light->get<ml::Transform>()->getPos()),
					new ml::uni_col4_cr	("Frag.diffuse",	sandbox.light->get<ml::Light>()->color),
					new ml::uni_col4	("Frag.mainCol",	ml::Color::White),
					new ml::uni_tex2	("Frag.mainTex",	ev.resources.textures.get("earth_dm")),
					new ml::uni_tex2	("Frag.specTex",	ev.resources.textures.get("earth_sm")),
					new ml::uni_flt		("Frag.ambient",	0.01f),
					new ml::uni_flt		("Frag.specular",	0.1f),
					new ml::uni_int		("Frag.shininess",	8),
					}));

			ml::Renderer * renderer = ent->add<ml::Renderer>(
				ev.resources.models.get("sphere32x24"),
				material
			);
		}

		// Ground
		/* * * * * * * * * * * * * * * * * * * * */
		if (ml::Entity * ent = ev.resources.entities.get("ground"))
		{
			ml::Transform * transform = ent->add<ml::Transform>(
				ml::vec3 { 0.0f, -2.5f, 0.0f }, // position
				ml::vec3 { 12.5, 0.25f, 12.5 }, // scale
				ml::quat { } // rotation
			);

			ml::BoxCollider * collider = ent->add<ml::BoxCollider>(
				transform->getScl() // size
			);
			collider->center_world = transform->getPos();

			ml::Particle * particle = ent->add<ml::Particle>(
				transform->getPos(), // position
				1.0f // mass
			);

			ml::Rigidbody * rb = ent->add<ml::Rigidbody>(sandbox.physWorld.createNewRigidbody(
				RB_GROUND, transform, collider, particle
			));

			const ml::Material * material = ev.resources.materials.create(
				"mat_ground",
				ev.resources.shaders.get("normal"),
				ml::List<ml::uni_base *>({
					new ml::uni_mat4_cr	("Vert.proj",		sandbox.camera->get<ml::Camera>()->getPerspMatrix()),
					new ml::uni_mat4_cr	("Vert.view",		sandbox.camera->get<ml::Transform>()->getMat()),
					new ml::uni_mat4_cr	("Vert.model",		transform->getMat()),
					new ml::uni_col4	("Frag.mainCol",	ml::Color::White),
					new ml::uni_tex2	("Frag.mainTex",	ev.resources.textures.get("stone_dm")),
					}));

			ml::Renderer * renderer = ent->add<ml::Renderer>(
				ev.resources.models.get("cube"),
				material
			);
		}

		// Setup Physics
		/* * * * * * * * * * * * * * * * * * * * */
		sandbox.physWorld.launchFun([&]()
		{
			const ml::GameTime & time = ev.time;
			while (ev.window.isOpen())
			{
				// Time stuff
				/* * * * * * * * * * * * * * * * * * * * */
				using Rep = typename ml::Milliseconds;
				using Per = typename ml::Ratio<1, 10000>;
				const float totalT	= time.total().delta();
				const float deltaT	= time.elapsed().delta<Rep, Per>();
				const float sinTime = std::sinf(totalT);
				const float cosTime = std::cosf(totalT);

				// Iterate over each entry
				/* * * * * * * * * * * * * * * * * * * * */
				sandbox.physWorld.updateFun([&](int32_t i, ml::PhysicsState & state)
				{	
					// Get copy data
					ml::vec3 pos;
					ml::quat rot;
					ml::mat4 mat;
					ml::mat4 inv;
					if (state.get<state.T_Pos>(i, pos) &&
						state.get<state.T_Rot>(i, rot) &&
						state.get<state.T_Mat>(i, mat) &&
						state.get<state.T_Inv>(i, inv))
					{	
						// Modify copy data
						switch (i)
						{
						case RB_BORG:
							pos = { pos[0], +cosTime, pos[2] };
							rot = ml::quat::angleAxis(totalT, ml::vec3::One);
							break;
						case RB_CUBE:
							pos = { pos[0], -sinTime, pos[2] };
							rot = ml::quat::angleAxis(totalT, ml::vec3::One);
							break;
						case RB_NAVBALL:
							pos = { pos[0], -cosTime, pos[2] };
							rot = ml::quat::angleAxis(totalT, ml::vec3::Forward);
							break;
						case RB_MOON:
							pos = { pos[0], +sinTime, pos[2] };
							rot = ml::quat::angleAxis(totalT, ml::vec3::Up);
							break;
						case RB_EARTH:
							rot = ml::quat::angleAxis(totalT, ml::vec3::Up);
							break;
						case RB_GROUND:
							break;
						}
						// Apply changes
						assert(
							state.set<state.T_Pos>(i, pos) &&
							state.set<state.T_Rot>(i, rot) &&
							state.set<state.T_Mat>(i, mat) &&
							state.set<state.T_Inv>(i, inv)
						);
					}
				});
			}
		});
	}

	void Sandbox::onUpdate(const ml::UpdateEvent & ev)
	{
		// Update Physics
		/* * * * * * * * * * * * * * * * * * * * */
		while (!sandbox.physWorld.syncFun([&](const ml::PhysicsState & state)
		{
			for (auto & pair : ev.resources.entities)
			{
				if (ml::Rigidbody * rb = pair.second->get<ml::Rigidbody>())
				{
					ml::vec3 scl = rb->transform()->getScl();
					ml::vec3 pos;
					ml::quat rot;
					ml::mat4 mat;
					ml::mat4 inv;
					if (state.get<state.T_Pos>(rb->index(), pos) &&
						state.get<state.T_Rot>(rb->index(), rot) &&
						state.get<state.T_Mat>(rb->index(), mat) &&
						state.get<state.T_Inv>(rb->index(), inv))
					{
						(*rb->transform())
							.update(ml::mat4::Identity())
							.translate(pos)
							.rotate(rot)
							.scale(scl);
					}
				}
			}
		}));

		// Update Surfaces
		/* * * * * * * * * * * * * * * * * * * * */
		ev.resources.surfaces.get("sf_sandbox_main")->resize(ev.window.getFrameSize());
		ev.resources.surfaces.get("sf_sandbox_post")->resize(ev.window.getFrameSize());

		// Update Camera
		/* * * * * * * * * * * * * * * * * * * * */
		if (ml::Camera * camera = sandbox.camera->get<ml::Camera>())
		{
			camera->updateRes(ev.window.getFrameSize());

			// Camera Transform
			if (ml::Transform * transform = sandbox.camera->get<ml::Transform>())
			{
				// Target Entity
				if (const ml::Entity * ent = ev.resources.entities.get("earth"))
				{
					// Target Transform
					if (const ml::Transform * target = ent->get<ml::Transform>())
					{
						if (sandbox.cameraOrbit)
						{
							camera->forward(target->getPos() - camera->position);

							transform->lookAt(camera->position, camera->forward());

							camera->position
								+= camera->right()
								*	ev.time.elapsed().delta()
								*	sandbox.cameraSpeed;
						}
					}
				}
			}
		}

		// Update Text
		/* * * * * * * * * * * * * * * * * * * * */
		{
			sandbox.text["project_url"]
				.setFont(ev.resources.fonts.get("minecraft"))
				.setFontSize(56)
				.setPosition({ 48, (float)ev.window.getFrameHeight() - 48 })
				.setString(ML_PROJECT_URL);

			const ml::Font *font	 = ev.resources.fonts.get("consolas");
			const uint32_t	fontSize = 24;
			const float		hOff	 = 0.0f;
			const float		vOff	 = 4.0f;
			const ml::vec2	offset	 = { hOff, (vOff + (float)fontSize) };
			const ml::vec2	origin	 = { (float)fontSize, (float)(fontSize * 2) };
			ml::vec2		linePos	 = 0.0f;
			size_t			lineNum	 = 0;
			auto			newLine  = [&]() { return (linePos = (origin + (offset * (float)(lineNum++)))); };

			sandbox.text["gl_version"]
				.setFont(font)
				.setFontSize(fontSize)
				.setPosition(newLine())
				.setString(ml::String("GL Version: {0}").format(
					ML_GL.getString(ml::GL::Version)
				));

			sandbox.text["gl_renderer"]
				.setFont(font)
				.setFontSize(fontSize)
				.setPosition(newLine())
				.setString(ml::String("GL Renderer: {0}").format(
					ML_GL.getString(ml::GL::Renderer)
				));

			sandbox.text["gl_vendor"]
				.setFont(font)
				.setFontSize(fontSize)
				.setPosition(newLine())
				.setString(ml::String("GL Vendor: {0}").format(
					ML_GL.getString(ml::GL::Vendor)
				));

			sandbox.text["glsl_version"]
				.setFont(font)
				.setFontSize(fontSize)
				.setPosition(newLine())
				.setString(ml::String("GLSL Version: {0}").format(
					ML_GL.getString(ml::GL::ShadingLanguageVersion)
				));

			newLine();

			sandbox.text["framerate"]
				.setFont(font)
				.setFontSize(fontSize)
				.setPosition(newLine())
				.setString(ml::String("{0} ms/frame ({1} fps)").format(
					ev.time.elapsed().delta(), 
					ev.time.frameRate()
				));

			sandbox.text["time_total"]
				.setFont(font)
				.setFontSize(fontSize)
				.setPosition(newLine())
				.setString(ml::String("time: {0}").format(
					ev.time.total()
				));

			newLine();

			sandbox.text["time_sin"]
				.setFont(font)
				.setFontSize(fontSize)
				.setPosition(newLine())
				.setString(ml::String("sin: {0}").format(
					std::sinf(ev.time.total().delta())
				));

			sandbox.text["time_cos"]
				.setFont(font)
				.setFontSize(fontSize)
				.setPosition(newLine())
				.setString(ml::String("cos: {0}").format(
					std::cosf(ev.time.total().delta())
				));

			newLine();

			sandbox.text["cursor_pos"]
				.setFont(font)
				.setFontSize(fontSize)
				.setPosition(newLine())
				.setString(ml::String("cx/cy: {0}").format(
					ev.window.getCursorPos()
				));

			sandbox.text["window_pos"]
				.setFont(font)
				.setFontSize(fontSize)
				.setPosition(newLine())
				.setString(ml::String("wx/wy: {0}").format(
					ev.window.getPosition()
				));

			sandbox.text["window_size"]
				.setFont(font)
				.setFontSize(fontSize)
				.setPosition(newLine())
				.setString(ml::String("ww/wh: {0}").format(
					ev.window.getSize()
				));

			// Ensure geometry update before main draw call
			for (auto & pair : sandbox.text)
			{
				pair.second.update();
			}
		}
	}

	void Sandbox::onDraw(const ml::DrawEvent & ev)
	{
		// Draw Scene
		/* * * * * * * * * * * * * * * * * * * * */
		if (const ml::Surface * scene = ev.resources.surfaces.get("sf_sandbox_main"))
		{
			// Bind Scene
			scene->bind();

			// Clear Screen
			ev.window.clear(sandbox.camera->get<ml::Camera>()->color);

			// Draw Renderers
			for (const auto & pair : ev.resources.entities)
			{
				if (pair.first.find("noobs") == ml::String::npos)
					ev.window.draw(pair.second->get<ml::Renderer>());
			}

			// Draw 2D
			/* * * * * * * * * * * * * * * * * * * * */
			{
				// Ortho Matrix
				const ml::mat4 & ortho = sandbox.camera->get<ml::Camera>()->getOrthoMatrix();

				// Render States
				static ml::RenderStates states(
					{ true, ml::GL::Greater, 0.01f },
					{ true },
					{ false },
					{ false },
					{ true, ml::GL::Texture2D, ml::GL::Texture0 },
					{ false, false }
				);
				states.apply();

				// Draw Sprites
				if (const ml::Shader * shader = ev.resources.shaders.get("sprites"))
				{
					static ml::Material * const material = ev.resources.materials.create(
						"mat_sprites",
						shader,
						ml::List<ml::uni_base *>({
							new ml::uni_mat4_cr	("Vert.proj",		ortho),
							new ml::uni_col4	("Frag.mainCol",	ml::Color::White),
							new ml::uni_tex2	("Frag.mainTex",	NULL),
							}));
					static ml::RenderBatch batch(
						&sandbox.vao, 
						&sandbox.vbo, 
						material
					);
					for (const auto & pair : ev.resources.sprites)
					{
						ev.window.draw(pair.second, batch);
					}
				}

				// Draw Text
				if (const ml::Shader * shader = ev.resources.shaders.get("text"))
				{
					static ml::Material * const material = ev.resources.materials.create(
						"mat_text",
						shader,
						ml::List<ml::uni_base *>({
							new ml::uni_mat4_cr	("Vert.proj",		ortho),
							new ml::uni_col4	("Frag.mainCol",	ml::Color::White),
							new ml::uni_tex2	("Frag.mainTex",	NULL),
							}));
					static ml::RenderBatch batch(
						&sandbox.vao,
						&sandbox.vbo,
						material
					);
					for (const auto & pair : sandbox.text)
					{
						ev.window.draw(pair.second, batch);
					}
				}

				// Draw Geometry
				if (const ml::Shader * shader = ev.resources.shaders.get("geometry"))
				{
					static ml::Material * const material = ev.resources.materials.create(
						"mat_geometry", 
						shader,
						ml::List<ml::uni_base *>({
							new ml::uni_col4("Frag.mainCol",	{ 0.385f, 0.0f, 1.0f, 1.0f }),
							new ml::uni_int("Geom.mode",		-1),
							new ml::uni_flt("Geom.delta",		1.0f),
							new ml::uni_flt("Geom.size",		0.995f),
							new ml::uni_int("Geom.samples",	16),
							}));
					if (material && material->bind())
					{
						ML_GL.drawArrays(ml::GL::Points, 0, 4);
						material->unbind();
					}
				}
			}
			
			// Unbind Scene
			scene->unbind();

			// Draw Post Processing
			/* * * * * * * * * * * * * * * * * * * * */
			if (const ml::Surface * post = ev.resources.surfaces.get("sf_sandbox_post"))
			{
				post->bind();
				if (const ml::Shader * shader = scene->shader())
					shader->setUniform("Effect.mode", sandbox.effectMode);
				ev.window.draw(scene);
				post->unbind();
			}
		}
	}

	void Sandbox::onGui(const ml::GuiEvent & ev)
	{
		// Sandbox Scene
		/* * * * * * * * * * * * * * * * * * * * */
		ML_EditorUtility.DrawWindow(
			"Sandbox Scene", 
			sandbox.showScene, 
			ImGuiWindowFlags_MenuBar, 
			[&]()
		{
			/* * * * * * * * * * * * * * * * * * * * */

			if (ImGui::BeginMenuBar())
			{
				ImGui::MenuItem("Sandbox Scene");
				ImGui::EndMenuBar();
			}

			/* * * * * * * * * * * * * * * * * * * * */

			if (ml::Surface * surface = ev.resources.surfaces.get("sf_sandbox_post"))
			{
				ml::Texture * texture = &surface->texture();
				if (texture && (*texture))
				{
					auto scaleToFit = [](const ml::vec2 & src, const ml::vec2 & dst)
					{
						const ml::vec2
							hs = (dst[0] / src[0]),
							vs = (dst[1] / src[1]);
						return (src * (((hs) < (vs)) ? (hs) : (vs)));
					};

					const ml::vec2 src = texture->size();
					const ml::vec2 dst = ml::EditorUtility::getWindowSize();
					const ml::vec2 scl = scaleToFit(src, dst);
					const ml::vec2 pos = ((dst - scl) * 0.5f);

					ImGui::BeginChild("Viewport", { -1, -1 });
					ImGui::SetCursorPos({ pos[0], pos[1] });
					ImGui::Image(texture->get_address(), { scl[0], scl[1] }, { 0, 1 }, { 1, 0 });
					ImGui::EndChild();
				}
			}

			/* * * * * * * * * * * * * * * * * * * * */
		});
		
		// Sandbox Inspector
		/* * * * * * * * * * * * * * * * * * * * */
		ML_EditorUtility.DrawWindow(
			"Sandbox Inspector", 
			sandbox.showInspector,
			ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysAutoResize,
			[&]()
		{
			/* * * * * * * * * * * * * * * * * * * * */

			if (ImGui::BeginMenuBar())
			{
				ImGui::MenuItem("Sandbox Inspector");
				ImGui::EndMenuBar();
			}

			/* * * * * * * * * * * * * * * * * * * * */

			ImGui::Combo("Effect Mode", &sandbox.effectMode, 
				"Normal\0"
				"Grayscale\0"
				"Blur\0"
				"Juicy\0"
				"Inverted\0"
			);
			ImGui::Separator();

			/* * * * * * * * * * * * * * * * * * * * */
		
			ImGui::Checkbox("Camera Orbit", &sandbox.cameraOrbit);
			ImGui::DragFloat("Camera Speed", &sandbox.cameraSpeed, 0.1f, -5.f, 5.f);
			ImGui::Separator();

			/* * * * * * * * * * * * * * * * * * * * */
		});
	}

	void Sandbox::onExit(const ml::ExitEvent & ev)
	{
		// Cleanup Physics Thread
		sandbox.physWorld.dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}