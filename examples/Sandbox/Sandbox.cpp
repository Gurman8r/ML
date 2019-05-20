/* * * * * * * * * * * * * * * * * * * * */

#include "Sandbox.hpp"

#include <ML/Audio/OpenAL.hpp>
#include <ML/Audio/Sound.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/FileSystem.hpp> 
#include <ML/Core/OS.hpp>
#include <ML/Graphics/Camera.hpp>
#include <ML/Graphics/Font.hpp>
#include <ML/Graphics/Light.hpp>
#include <ML/Graphics/Model.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Graphics/Renderer.hpp>
#include <ML/Graphics/ShaderAPI.hpp>
#include <ML/Graphics/Skybox.hpp>
#include <ML/Graphics/Sprite.hpp>
#include <ML/Graphics/Surface.hpp>
#include <ML/Graphics/Uni.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Editor/ImGui_Style.hpp>
#include <ML/Editor/StyleLoader.hpp>
#include <ML/Engine/Engine.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Engine/Resources.hpp>
#include <ML/Network/NetClient.hpp>
#include <ML/Network/NetServer.hpp>
#include <ML/Physics/PhysicsWorld.hpp>
#include <ML/Physics/PhysicsState.hpp>
#include <ML/Physics/Collider.hpp>
#include <ML/Physics/Particle.hpp>
#include <ML/Physics/Rigidbody.hpp>
#include <ML/Physics/SphereCollider.hpp>
#include <ML/Physics/BoxCollider.hpp>
#include <ML/Script/Interpreter.hpp>
#include <ML/Script/Script.hpp>
#include <ML/Window/WindowEvents.hpp>

/* * * * * * * * * * * * * * * * * * * * */

ML_PLUGIN_API ml::Application * ML_Plugin_Main(ml::EventSystem & eventSystem)
{
	return new DEMO::Sandbox(eventSystem);
}

/* * * * * * * * * * * * * * * * * * * * */

namespace DEMO
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Sandbox::Sandbox(ml::EventSystem & eventSystem)
		: EditorApplication(eventSystem)
		, sandbox(*this)
	{
		eventSystem.addListener(ml::ScriptEvent::EV_Command, this);
	}

	Sandbox::~Sandbox() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	void Sandbox::onEvent(const ml::IEvent * value)
	{
		// Handle base events
		ml::EditorApplication::onEvent(value);

		switch (*value)
		{
			// Command
			/* * * * * * * * * * * * * * * * * * * * */
		case ml::ScriptEvent::EV_Command:
			if (auto ev = value->as<ml::CommandEvent>())
			{
				ml::Var v;
				if ((v = ML_Interpreter.execCommand(ev->cmd)).isErrorType())
				{
					ml::Debug::logError(v.errorValue());
				}
			}
			break;

			// Keyboard Input
			/* * * * * * * * * * * * * * * * * * * * */
		case ml::WindowEvent::EV_Key:
			if (auto ev = value->as<ml::KeyEvent>())
			{
				/* * * * * * * * * * * * * * * * * * * * */

				// Close (Escape)
				if (ev->getKeyDown(ml::KeyCode::Escape))
				{
					eventSystem().fireEvent(ml::CloseEvent());
				}

				/* * * * * * * * * * * * * * * * * * * * */
				//
				//// Show Terminal (Ctrl+Alt+T)
				//if (ev->getKeyDown(ml::KeyCode::T) && (ev->mod_ctrl && ev->mod_alt))
				//	ML_Editor.show_terminal = true;
				//
				//// Show Browser (Ctrl+Alt+E)
				//if (ev->getKeyDown(ml::KeyCode::E) && (ev->mod_ctrl))
				//	ML_Editor.show_browser = true;
				//
				//// Show Builder (Ctrl+Alt+B)
				//if (ev->getKeyDown(ml::KeyCode::B) && (ev->mod_ctrl && ev->mod_alt))
				//	ML_Editor.show_builder = true;
				//
				//// Show Scene (Ctrl+Alt+S)
				//if (ev->getKeyDown(ml::KeyCode::S) && (ev->mod_ctrl && ev->mod_alt))
				//	ML_Editor.show_sceneView = true;
				//
				//// Show Inspector (Ctrl+Alt+I)
				//if (ev->getKeyDown(ml::KeyCode::I) && (ev->mod_ctrl && ev->mod_alt))
				//	ML_Editor.show_inspector = true;
				//
				//// Show ImGui Demo (Ctrl+H)
				//if (ev->getKeyDown(ml::KeyCode::H) && (ev->mod_ctrl))
				//	ML_Editor.show_imgui_demo = true;
				//
				/* * * * * * * * * * * * * * * * * * * * */
			}
			break;
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Sandbox::onEnter(const ml::EnterEvent & ev)
	{
		// Initialize Miscellaneous
		/* * * * * * * * * * * * * * * * * * * * */
		{
			// Setup Std Out
			if (!(sandbox.rdbuf = ml::cout.rdbuf(sandbox.rdstr.rdbuf())))
			{
				return ml::Debug::fatal("Failed Redirecting Std Output Handle");
			}

			// Set Resource Path
			sandbox.res_path = ev.engine.prefs().GetString(
			"Engine",
			"res_path",
			"../../../assets/"
			);

			// Set Resource File
			sandbox.res_data = ev.engine.prefs().GetString(
				"Engine",
				"res_data",
				"manifest.txt"
			);

			// Set Resource Manifest
			sandbox.manifest = ML_FS.getPathTo(
				sandbox.res_path + sandbox.res_data
			);
		}

		// Initialize Interpreter
		/* * * * * * * * * * * * * * * * * * * * */
		{
			// Set Parser Flags
			ML_Parser.showToks(ev.engine.prefs().GetBool("Script", "showToks", false));
			ML_Parser.showTree(ev.engine.prefs().GetBool("Script", "showTree", false));
			ML_Parser.showItoP(ev.engine.prefs().GetBool("Script", "showItoP", false));

			// Run Boot Script
			ml::Script scr;
			if (scr.loadFromFile(ML_FS.getPathTo(
				sandbox.res_path + ev.engine.prefs().GetString("Script", "bootScript", "")
			)))
			{
				if (!(scr.buildAndRun(ml::Args(__argc, __argv))))
				{
					ml::Debug::logError("Failed Running \'{0}\'", scr.path());
				}
			}
		}

		// Initialize Window
		/* * * * * * * * * * * * * * * * * * * * */
		if (sandbox->create(sandbox.title = 
			ev.engine.prefs().GetString	("Window", "title",			"ML"), { {
			ev.engine.prefs().GetUint	("Window", "width",			1280),
			ev.engine.prefs().GetUint	("Window", "height",		720) },
			ev.engine.prefs().GetUint	("Window", "colorDepth",	32) },
			ev.engine.prefs().GetUint	("Window", "style",			ml::Window::Default), {
			ev.engine.prefs().GetUint	("Window", "majorVersion",	3),
			ev.engine.prefs().GetUint	("Window", "minorVersion",	3),
			ev.engine.prefs().GetUint	("Window", "profile",		ml::Context::Compat),
			ev.engine.prefs().GetUint	("Window", "depthBits",		24),
			ev.engine.prefs().GetUint	("Window", "stencilBits",	8),
			ev.engine.prefs().GetBool	("Window", "multisample",	false),
			ev.engine.prefs().GetBool	("Window", "srgbCapable",	false)
		}))
		{
			sandbox->maximize();
			sandbox->seCursorMode(ml::Cursor::Normal);
			sandbox->setPosition((ml::Screen::desktop().resolution - sandbox->getSize()) / 2);
			sandbox->setViewport(ml::vec2i::Zero, sandbox->getFrameSize());
		}
		else
		{
			return ml::Debug::fatal("Failed Initializing Window");
		}

		// Initialize Gui
		/* * * * * * * * * * * * * * * * * * * * */
		if (IMGUI_CHECKVERSION())
		{
			ImGui::CreateContext();

			// Set ImGui Style
			ml::StyleLoader loader;
			if (loader.loadFromFile(ML_FS.getPathTo("../../../assets/styles/style4.txt")))
			{ 
				/* TODO */
			}
			else
			{
				ImGui::StyleHelper::Style4();
			}

			// Set ImGui Fonts
			if (ml::String	imguiFont = ev.engine.prefs().GetString("Editor", "imguiFont", ""))
			{	const float imguiSize = ev.engine.prefs().GetFloat ("Editor", "imguiSize", 12.0f);
				if (imguiFont && imguiSize > 0.0f)
				{
					ImGui::GetIO().Fonts->AddFontFromFileTTF(imguiFont.c_str(), imguiSize);
				}
			}

			// Set ImGui INI
			ml::String imguiINI = ev.engine.prefs().GetString("Editor", "imguiINI", "");
			imguiINI = imguiINI ? ML_FS.getPathTo(imguiINI) : ml::String();
			
			// Run ImGui Init
			if (!ImGui_ML_Init("#version 410", this, true, imguiINI.c_str()))
			{
				return ml::Debug::fatal("Failed Initializing ImGui");
			}
		}

		// Initialize Audio
		/* * * * * * * * * * * * * * * * * * * * */
		if (!ML_AL.init(eventSystem()))
		{
			return ml::Debug::fatal("Failed Initializing OpenAL");
		}

		// Initialize Network
		/* * * * * * * * * * * * * * * * * * * * */
		sandbox.isServer = ev.engine.prefs().GetBool("Network", "isServer", false);
		sandbox.isClient = ev.engine.prefs().GetBool("Network", "isClient", false);
		
		if (sandbox.isServer)
		{	// Start Server
			ml::Debug::log("Starting Server...");
			if (ev.engine.server().setup())
			{
				if (ev.engine.server().start({ ML_LOCALHOST, ML_DEFAULT_PORT }, ML_MAX_CLIENTS))
				{
					ml::Debug::log("Server Started: {0}", ev.engine.server().getMyAddress());
				}
			}
		}
		else if (sandbox.isClient)
		{	// Start Client
			ml::Debug::log("Starting Client...");
			if (ev.engine.client().setup())
			{
				if (ev.engine.client().connect({ ML_LOCALHOST, ML_DEFAULT_PORT }))
				{
					ml::Debug::log("Client Connected: {0}", ev.engine.client().getMyAddress());
				}
			}
		}
	}

	void Sandbox::onLoad(const ml::LoadEvent & ev)
	{
		// Load Default Meshes
		/* * * * * * * * * * * * * * * * * * * * */
		ev.engine.resources().meshes.load("default_triangle")->loadFromMemory(
			ml::Shapes::Triangle::Vertices,
			ml::Shapes::Triangle::Indices
		);
		ev.engine.resources().meshes.load("default_quad")->loadFromMemory(
			ml::Shapes::Quad::Vertices,
			ml::Shapes::Quad::Indices
		);
		ev.engine.resources().meshes.load("default_cube")->loadFromMemory(
			ml::Shapes::Cube::Vertices,
			ml::Shapes::Cube::Indices
		);
		ev.engine.resources().meshes.load("default_skybox")->loadFromMemory(
			ml::Shapes::Sky::Vertices
		);

		// Load Default Models
		/* * * * * * * * * * * * * * * * * * * * */
		ev.engine.resources().models.load("default_triangle")->loadFromMemory(
			*ev.engine.resources().meshes.get("default_triangle")
		);
		ev.engine.resources().models.load("default_quad")->loadFromMemory(
			*ev.engine.resources().meshes.get("default_quad")
		);
		ev.engine.resources().models.load("default_cube")->loadFromMemory(
			*ev.engine.resources().meshes.get("default_cube")
		);
		ev.engine.resources().models.load("default_skybox")->loadFromMemory(
			*ev.engine.resources().meshes.get("default_skybox")
		);

		// Load Manifest
		/* * * * * * * * * * * * * * * * * * * * */
		if (!ev.engine.resources().loadFromFile(sandbox.manifest))
		{
			ml::Debug::logError("Failed Loading Manifest");
		}
	}

	void Sandbox::onStart(const ml::StartEvent & ev)
	{
		// Set Icon
		/* * * * * * * * * * * * * * * * * * * * */
		if (const ml::Image * icon = ev.engine.resources().images.get("icon"))
		{
			const ml::Image temp = ml::Image(*icon).flipVertically();

			sandbox->setIcons({ temp });
		}

		// Setup Plugins
		/* * * * * * * * * * * * * * * * * * * * */
		if (!ev.engine.resources().plugins.empty())
		{
			if (ml::Plugin * plugin = ev.engine.resources().plugins.get("TestPlugin"))
			{
				if (void * msg = plugin->lib().callFun<void *>(
					ML_str(ML_Plugin_Test), "TEST"
					))
				{
					ml::Debug::log((ml::CString)(msg));
				}
				else
				{
					ml::Debug::logError("Plugin Callback Failed");
				}
			}
		}

		// Setup 2D Buffers
		/* * * * * * * * * * * * * * * * * * * * */
		sandbox.vao.create(ml::GL::Triangles).bind();
		sandbox.vbo.create(ml::GL::DynamicDraw).bind();
		sandbox.vbo.bufferData(NULL, ml::Shapes::RectQuad::Size);
		ml::BufferLayout::Default.bind();
		sandbox.vbo.unbind();
		sandbox.vao.unbind();

		// Setup Sprites
		/* * * * * * * * * * * * * * * * * * * * */
		if (ml::Sprite * spr = ev.engine.resources().sprites.get("neutrino"))
		{
			spr->setPosition(ml::vec2(0.95f, 0.925f) * sandbox->getSize())
				.setScale	(0.5f)
				.setRotation(0.0f)
				.setOrigin	(0.5f)
				.setColor	(ml::Color::White);
		}

		// Setup Entities
		/* * * * * * * * * * * * * * * * * * * * */
		{
			// Camera
			/* * * * * * * * * * * * * * * * * * * * */
			if (ml::Entity * ent = ev.engine.resources().entities.get("camera"))
			{
				sandbox.camera = ent;

				ml::Camera * camera = ent->add<ml::Camera>(
					ev.engine.prefs().GetFloat("Graphics", "fieldOfView", 45.0),
					ev.engine.prefs().GetFloat("Graphics", "perspNear", 0.1f),
					ev.engine.prefs().GetFloat("Graphics", "perspFar", 1000.0f),
					ev.engine.prefs().GetFloat("Graphics", "orthoNear", -1.0f),
					ev.engine.prefs().GetFloat("Graphics", "orthoFar", +1.0f)
				);
				camera->color = { ml::vec3 { 0.198f }, 1.0f };
				camera->position = { 0.0f, 1.0f, 10.0f };
				camera->forward(ml::vec3::Back);

				ml::Transform * transform = ent->add<ml::Transform>();
				transform->lookAt(camera->position, camera->forward());
			}

			// Light
			/* * * * * * * * * * * * * * * * * * * * */
			if (ml::Entity * ent = ev.engine.resources().entities.get("light"))
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

				const ml::Material * material = ev.engine.resources().materials.load_forward(
					"mat_light",
					ev.engine.resources().shaders.get("solid"),
					ml::List<ml::uni_base *>({
						new ml::uni_mat4_cr	(ML_VERT_PROJ,		sandbox.camera->get<ml::Camera>()->getPerspMatrix()),
						new ml::uni_mat4_cr	(ML_VERT_VIEW,		sandbox.camera->get<ml::Transform>()->getMat()),
						new ml::uni_mat4_cr	(ML_VERT_MODEL,		transform->getMat()),
						new ml::uni_col4_cr	(ML_FRAG_MAIN_COL,	light->color),
						}));

				ml::Renderer * renderer = ent->add<ml::Renderer>(
					ev.engine.resources().models.get("sphere8x6"),
					material,
					ml::RenderStates({
						{ ml::GL::AlphaTest,{ ml::RenderVar::Bool, true } },
						{ ml::GL::Blend,	{ ml::RenderVar::Bool, true } },
						{ ml::GL::CullFace,	{ ml::RenderVar::Bool, true } },
						{ ml::GL::DepthTest,{ ml::RenderVar::Bool, true } },
					})
				);
			}

			// Borg
			/* * * * * * * * * * * * * * * * * * * * */
			if (ml::Entity * ent = ev.engine.resources().entities.get("borg"))
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

				const ml::Material * material = ev.engine.resources().materials.load_forward(
					"mat_borg",
					ev.engine.resources().shaders.get("basic"),
					ml::List<ml::uni_base *>({
						new ml::uni_mat4_cr	(ML_VERT_PROJ,		sandbox.camera->get<ml::Camera>()->getPerspMatrix()),
						new ml::uni_mat4_cr	(ML_VERT_VIEW,		sandbox.camera->get<ml::Transform>()->getMat()),
						new ml::uni_mat4_cr	(ML_VERT_MODEL,		transform->getMat()),
						new ml::uni_col4	(ML_FRAG_MAIN_COL,	ml::Color::White),
						new ml::uni_tex_cp	(ML_FRAG_MAIN_TEX,	ev.engine.resources().textures.get("borg")),
						}));

				ml::Renderer * renderer = ent->add<ml::Renderer>(
					ev.engine.resources().models.get("default_cube"),
					material,
					ml::RenderStates({
						{ ml::GL::AlphaTest,{ ml::RenderVar::Bool, true } },
						{ ml::GL::Blend,	{ ml::RenderVar::Bool, true } },
						{ ml::GL::CullFace,	{ ml::RenderVar::Bool, true } },
						{ ml::GL::DepthTest,{ ml::RenderVar::Bool, true } },
					})
				);
			}

			// Cube
			/* * * * * * * * * * * * * * * * * * * * */
			if (ml::Entity * ent = ev.engine.resources().entities.get("cube"))
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

				const ml::Material * material = ev.engine.resources().materials.load_forward(
					"mat_cube",
					ev.engine.resources().shaders.get("normal"),
					ml::List<ml::uni_base *>({
						new ml::uni_mat4_cr	(ML_VERT_PROJ,		sandbox.camera->get<ml::Camera>()->getPerspMatrix()),
						new ml::uni_mat4_cr	(ML_VERT_VIEW,		sandbox.camera->get<ml::Transform>()->getMat()),
						new ml::uni_mat4_cr	(ML_VERT_MODEL,		transform->getMat()),
						new ml::uni_col4	(ML_FRAG_MAIN_COL,	ml::Color::White),
						new ml::uni_tex_cp	(ML_FRAG_MAIN_TEX,	ev.engine.resources().textures.get("stone_dm")),
						}));

				ml::Renderer * renderer = ent->add<ml::Renderer>(
					ev.engine.resources().models.get("cube"),
					material,
					ml::RenderStates({
						{ ml::GL::AlphaTest,{ ml::RenderVar::Bool, true } },
						{ ml::GL::Blend,	{ ml::RenderVar::Bool, true } },
						{ ml::GL::CullFace,	{ ml::RenderVar::Bool, true } },
						{ ml::GL::DepthTest,{ ml::RenderVar::Bool, true } },
					})
				);
			}

			// Navball
			/* * * * * * * * * * * * * * * * * * * * */
			if (ml::Entity * ent = ev.engine.resources().entities.get("navball"))
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

				const ml::Material * material = ev.engine.resources().materials.load_forward(
					"mat_navball",
					ev.engine.resources().shaders.get("basic"),
					ml::List<ml::uni_base *>({
						new ml::uni_mat4_cr	(ML_VERT_PROJ,		sandbox.camera->get<ml::Camera>()->getPerspMatrix()),
						new ml::uni_mat4_cr	(ML_VERT_VIEW,		sandbox.camera->get<ml::Transform>()->getMat()),
						new ml::uni_mat4_cr	(ML_VERT_MODEL,		transform->getMat()),
						new ml::uni_col4	(ML_FRAG_MAIN_COL,	ml::Color::White),
						new ml::uni_tex_cp	(ML_FRAG_MAIN_TEX,	ev.engine.resources().textures.get("navball")),
						}));

				ml::Renderer * renderer = ent->add<ml::Renderer>(
					ev.engine.resources().models.get("sphere32x24"),
					material,
					ml::RenderStates({
						{ ml::GL::AlphaTest,{ ml::RenderVar::Bool, true } },
						{ ml::GL::Blend,	{ ml::RenderVar::Bool, true } },
						{ ml::GL::CullFace,	{ ml::RenderVar::Bool, true } },
						{ ml::GL::DepthTest,{ ml::RenderVar::Bool, true } },
						}
				));
			}

			// Moon
			/* * * * * * * * * * * * * * * * * * * * */
			if (ml::Entity * ent = ev.engine.resources().entities.get("moon"))
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

				const ml::Material * material = ev.engine.resources().materials.load_forward(
					"mat_moon",
					ev.engine.resources().shaders.get("lighting"),
					ml::List<ml::uni_base *> ({
						new ml::uni_mat4_cr	(ML_VERT_PROJ,		sandbox.camera->get<ml::Camera>()->getPerspMatrix()),
						new ml::uni_mat4_cr	(ML_VERT_VIEW,		sandbox.camera->get<ml::Transform>()->getMat()),
						new ml::uni_mat4_cr	(ML_VERT_MODEL,		transform->getMat()),
						new ml::uni_vec3_cr	("Frag.cameraPos",	sandbox.camera->get<ml::Transform>()->getPos()),
						new ml::uni_vec3_cr	("Frag.lightPos",	sandbox.light->get<ml::Transform>()->getPos()),
						new ml::uni_col4_cr	("Frag.diffuse",	sandbox.light->get<ml::Light>()->color),
						new ml::uni_col4	(ML_FRAG_MAIN_COL,	ml::Color::White),
						new ml::uni_tex_cp	(ML_FRAG_MAIN_TEX,	ev.engine.resources().textures.get("moon_dm")),
						new ml::uni_tex_cp	("Frag.specTex",	ev.engine.resources().textures.get("moon_nm")),
						new ml::uni_flt		("Frag.ambient",	0.01f),
						new ml::uni_flt		("Frag.specular",	0.1f),
						new ml::uni_int		("Frag.shininess",	8),
						}));

				ml::Renderer * renderer = ent->add<ml::Renderer>(
					ev.engine.resources().models.get("sphere32x24"),
					material,
					ml::RenderStates({
						{ ml::GL::AlphaTest,{ ml::RenderVar::Bool, true } },
						{ ml::GL::Blend,	{ ml::RenderVar::Bool, true } },
						{ ml::GL::CullFace,	{ ml::RenderVar::Bool, true } },
						{ ml::GL::DepthTest,{ ml::RenderVar::Bool, true } },
						}
				));
			}

			// Earth
			/* * * * * * * * * * * * * * * * * * * * */
			if (ml::Entity * ent = ev.engine.resources().entities.get("earth"))
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

				const ml::Material * material = ev.engine.resources().materials.load_forward(
					"mat_earth",
					ev.engine.resources().shaders.get("lighting"),
					ml::List<ml::uni_base *> ({
						new ml::uni_mat4_cr	(ML_VERT_PROJ,		sandbox.camera->get<ml::Camera>()->getPerspMatrix()),
						new ml::uni_mat4_cr	(ML_VERT_VIEW,		sandbox.camera->get<ml::Transform>()->getMat()),
						new ml::uni_mat4_cr	(ML_VERT_MODEL,		transform->getMat()),
						new ml::uni_vec3_cr	("Frag.cameraPos",	sandbox.camera->get<ml::Transform>()->getPos()),
						new ml::uni_vec3_cr	("Frag.lightPos",	sandbox.light->get<ml::Transform>()->getPos()),
						new ml::uni_col4_cr	("Frag.diffuse",	sandbox.light->get<ml::Light>()->color),
						new ml::uni_col4	(ML_FRAG_MAIN_COL,	ml::Color::White),
						new ml::uni_tex_cp	(ML_FRAG_MAIN_TEX,	ev.engine.resources().textures.get("earth_dm")),
						new ml::uni_tex_cp	("Frag.specTex",	ev.engine.resources().textures.get("earth_sm")),
						new ml::uni_flt		("Frag.ambient",	0.01f),
						new ml::uni_flt		("Frag.specular",	0.1f),
						new ml::uni_int		("Frag.shininess",	8),
						}));

				ml::Renderer * renderer = ent->add<ml::Renderer>(
					ev.engine.resources().models.get("sphere32x24"),
					material,
					ml::RenderStates({
						{ ml::GL::AlphaTest,{ ml::RenderVar::Bool, true } },
						{ ml::GL::Blend,	{ ml::RenderVar::Bool, true } },
						{ ml::GL::CullFace,	{ ml::RenderVar::Bool, true } },
						{ ml::GL::DepthTest,{ ml::RenderVar::Bool, true } },
					}
				));
			}

			// Ground
			/* * * * * * * * * * * * * * * * * * * * */
			if (ml::Entity * ent = ev.engine.resources().entities.get("ground"))
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

				const ml::Material * material = ev.engine.resources().materials.load_forward(
					"mat_ground",
					ev.engine.resources().shaders.get("normal"),
					ml::List<ml::uni_base *>({
						new ml::uni_mat4_cr	(ML_VERT_PROJ,		sandbox.camera->get<ml::Camera>()->getPerspMatrix()),
						new ml::uni_mat4_cr	(ML_VERT_VIEW,		sandbox.camera->get<ml::Transform>()->getMat()),
						new ml::uni_mat4_cr	(ML_VERT_MODEL,		transform->getMat()),
						new ml::uni_col4	(ML_FRAG_MAIN_COL,	ml::Color::White),
						new ml::uni_tex_cp	(ML_FRAG_MAIN_TEX,	ev.engine.resources().textures.get("stone_dm")),
						}));

				ml::Renderer * renderer = ent->add<ml::Renderer>(
					ev.engine.resources().models.get("cube"),
					material,
					ml::RenderStates({
						{ ml::GL::AlphaTest,{ ml::RenderVar::Bool, true } },
						{ ml::GL::Blend,	{ ml::RenderVar::Bool, true } },
						{ ml::GL::CullFace,	{ ml::RenderVar::Bool, true } },
						{ ml::GL::DepthTest,{ ml::RenderVar::Bool, true } },
					})
				);
			}
		}

		// Setup Physics
		/* * * * * * * * * * * * * * * * * * * * */
		sandbox.physWorld.launchFun([&]()
		{
			static ml::Engine & engine(ev.engine);
			while (engine.isRunning())
			{
				sandbox.physWorld.updateFun([](int32_t i, ml::PhysicsState & state)
				{
					using Rep = typename ml::Milliseconds;
					using Per = typename ml::Ratio<1, 10000>;

					const float totalT	= engine.mainTimer().elapsed().delta();
					const float deltaT	= engine.loopTimer().elapsed().delta<Rep, Per>();
					const float sinTime = std::sinf(totalT);
					const float cosTime = std::cosf(totalT);

					// Get copy state
					ml::vec3 pos;
					ml::quat rot;
					ml::mat4 mat;
					ml::mat4 inv;
					if (state.get<state.T_Pos>(i, pos) &&
						state.get<state.T_Rot>(i, rot) &&
						state.get<state.T_Mat>(i, mat) &&
						state.get<state.T_Inv>(i, inv))
					{
						// Modify copy state
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
							state.set<state.T_Inv>(i, inv) &&
							"\n** Something went wrong... **\n"
							"\n** Failed applying changes to physics state! **\n"
						);
					}
				});
			}
		});
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Sandbox::onUpdate(const ml::UpdateEvent & ev)
	{
		// Update Title
		/* * * * * * * * * * * * * * * * * * * * */
		sandbox->setTitle(ml::String("{0} | {1} | {2} | {3} ms/frame ({4} fps)").format(
			sandbox.title,
			ML_CONFIGURATION,
			ML_PLATFORM_TARGET,
			ev.engine.frameTime().delta(),
			ev.engine.frameRate()
		));

		// Update Physics
		/* * * * * * * * * * * * * * * * * * * * */
		while (!sandbox.physWorld.syncFun([&](const ml::PhysicsState & state)
		{
			for (auto & pair : ev.engine.resources().entities)
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
							.update		(ml::mat4::Identity())
							.translate	(pos)
							.rotate		(rot)
							.scale		(scl)
							;
					}
				}
			}
		}));

		// Update Surfaces
		/* * * * * * * * * * * * * * * * * * * * */
		for (auto & pair : ev.engine.resources().surfaces)
		{
			pair.second->resize(sandbox->getFrameSize());
		}

		// Update Camera
		/* * * * * * * * * * * * * * * * * * * * */
		if (ml::Camera * camera = sandbox.camera->get<ml::Camera>())
		{
			camera->updateRes(sandbox->getFrameSize());

			// Camera Transform
			if (ml::Transform * transform = sandbox.camera->get<ml::Transform>())
			{
				// Target Entity
				if (const ml::Entity * ent = ev.engine.resources().entities.get("earth"))
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
								*	ev.engine.frameTime().delta()
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
				.setFont(ev.engine.resources().fonts.get("minecraft"))
				.setFontSize(56)
				.setPosition({ 48, (float)sandbox->getFrameHeight() - 48 })
				.setString(ML_PROJECT_URL);

			const ml::Font *font	 = ev.engine.resources().fonts.get("consolas");
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
					ML_GL.getString(ml::GL::Version))
				);

			sandbox.text["gl_vendor"]
				.setFont(font)
				.setFontSize(fontSize)
				.setPosition(newLine())
				.setString(ml::String("GL Vendor: {0}").format(
					ML_GL.getString(ml::GL::Vendor))
				);

			newLine();

			sandbox.text["framerate"]
				.setFont(font)
				.setFontSize(fontSize)
				.setPosition(newLine())
				.setString(ml::String("{0} ms/frame ({1} fps)").format(
					ev.engine.frameTime().delta(),
					ev.engine.frameRate())
				);

			sandbox.text["time_total"]
				.setFont(font)
				.setFontSize(fontSize)
				.setPosition(newLine())
				.setString(ml::String("time: {0}").format(
					ev.engine.mainTimer().elapsed())
				);

			newLine();

			sandbox.text["time_sin"]
				.setFont(font)
				.setFontSize(fontSize)
				.setPosition(newLine())
				.setString(ml::String("sin: {0}").format(
					std::sinf(ev.engine.mainTimer().elapsed().delta()))
				);

			sandbox.text["time_cos"]
				.setFont(font)
				.setFontSize(fontSize)
				.setPosition(newLine())
				.setString(ml::String("cos: {0}").format(
					std::cosf(ev.engine.mainTimer().elapsed().delta()))
				);

			newLine();

			sandbox.text["cursor_pos"]
				.setFont(font)
				.setFontSize(fontSize)
				.setPosition(newLine())
				.setString(ml::String("cx/cy: {0}").format(
					sandbox->getCursorPos())
				);

			sandbox.text["window_pos"]
				.setFont(font)
				.setFontSize(fontSize)
				.setPosition(newLine())
				.setString(ml::String("wx/wy: {0}").format(
					sandbox->getPosition())
				);

			sandbox.text["window_size"]
				.setFont(font)
				.setFontSize(fontSize)
				.setPosition(newLine())
				.setString(ml::String("ww/wh: {0}").format(
					sandbox->getSize())
				);

			// Ensure text update before main draw call
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
		if (const ml::Surface * scene = ev.engine.resources().surfaces.get("surface_main"))
		{
			// Bind Scene
			scene->bind();

			// Clear Screen
			sandbox->clear(sandbox.camera->get<ml::Camera>()->color);

			// Draw Renderers
			for (const auto & pair : ev.engine.resources().entities)
			{
				sandbox->draw(pair.second->get<ml::Renderer>());
			}

			// Draw 2D
			/* * * * * * * * * * * * * * * * * * * * */
			{
				// Ortho Matrix
				const ml::mat4 & ortho = sandbox.camera->get<ml::Camera>()->getOrthoMatrix();

				// Render States
				static ml::RenderStates states
				({
					{ ml::GL::AlphaTest,	{ ml::RenderVar::Bool, true } },
					{ ml::GL::Blend,		{ ml::RenderVar::Bool, true } },
					{ ml::GL::CullFace,		{ ml::RenderVar::Bool, false } },
					{ ml::GL::DepthTest,	{ ml::RenderVar::Bool, false } },
					{ ml::GL::Texture2D,	{ ml::RenderVar::Bool, true } },
				});
				states.apply();

				// Draw Sprites
				if (const ml::Shader * shader = ev.engine.resources().shaders.get("sprites"))
				{
					static ml::Material * const material = ev.engine.resources().materials.load_forward(
						"mat_sprites",
						shader,
						ml::List<ml::uni_base *>({
							new ml::uni_mat4_cr	(ML_VERT_PROJ,		ortho),
							new ml::uni_col4	(ML_FRAG_MAIN_COL,	ml::Color::White),
							new ml::uni_tex_cp	(ML_FRAG_MAIN_TEX,	NULL),
							}));

					static ml::RenderBatch batch(
						&sandbox.vao, 
						&sandbox.vbo, 
						material
					);

					for (const auto & pair : ev.engine.resources().sprites)
					{
						sandbox->draw(pair.second, batch);
					}
				}

				// Draw Text
				if (const ml::Shader * shader = ev.engine.resources().shaders.get("text"))
				{
					static ml::Material * const material = ev.engine.resources().materials.load_forward(
						"mat_text",
						shader,
						ml::List<ml::uni_base *>({
							new ml::uni_mat4_cr	(ML_VERT_PROJ,		ortho),
							new ml::uni_col4	(ML_FRAG_MAIN_COL,	ml::Color::White),
							new ml::uni_tex_cp	(ML_FRAG_MAIN_TEX,	NULL),
							}));

					static ml::RenderBatch batch(
						&sandbox.vao,
						&sandbox.vbo,
						material
					);

					for (const auto & pair : sandbox.text)
					{
						sandbox->draw(pair.second, batch);
					}
				}

				// Draw Geometry
				if (const ml::Shader * shader = ev.engine.resources().shaders.get("geometry"))
				{
					static ml::Material * const material = ev.engine.resources().materials.load_forward(
						"mat_geometry", 
						shader,
						ml::List<ml::uni_base *>({
							new ml::uni_col4(ML_FRAG_MAIN_COL,	{ 0.385f, 0.0f, 1.0f, 1.0f }),
							new ml::uni_int("Geom.mode",		-1),
							new ml::uni_flt("Geom.delta",		1.0f),
							new ml::uni_flt("Geom.size",		0.995f),
							new ml::uni_int("Geom.samples",		16),
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
		}

		// Draw Post
		/* * * * * * * * * * * * * * * * * * * * */
		if (const ml::Surface * post = ev.engine.resources().surfaces.get("surface_post"))
		{
			post->bind();
			if (const ml::Surface * scene = ev.engine.resources().surfaces.get("surface_main"))
			{
				scene->shader()->setUniform("Surface.mode", sandbox.effectMode);

				sandbox->draw(*scene);
			}
			post->unbind();
		}
	}

	void Sandbox::onGui(const ml::GuiEvent & ev)
	{
		// Update Terminal Output
		/* * * * * * * * * * * * * * * * * * * * */
		if (sandbox.rdbuf)
		{
			ev.editor.terminal.printss(sandbox.rdstr);
		}

		/*	Main Menu	*/ ev.editor.mainMenu.onGui(ev);
		/*	Dockspace	*/ ev.editor.dockspace.onGui(ev);
		/*	Network		*/ ev.editor.network.onGui(ev);
		/*	Profiler	*/ ev.editor.profiler.onGui(ev);
		/*	Browser		*/ ev.editor.browser.onGui(ev);
		/*	Terminal	*/ ev.editor.terminal.onGui(ev);
		/*	Text Editor	*/ ev.editor.textEditor.onGui(ev); 
		/*	Project		*/ ev.editor.project.onGui(ev);
		/*	Builder		*/ ev.editor.builder.onGui(ev);
		
		/*	Scene View	*/ 
		ev.editor.sceneView.drawFun(ev, [&]()
		{
			if (ml::Surface * post = ev.editor.resources().surfaces.get("surface_post"))
			{
				ev.editor.sceneView.updateTexture(&post->texture());
			}
		});

		/*	Inspector	*/ 
		ev.editor.inspector.drawFun(ev, [&]()
		{
			static ml::CString surf_modes[] = {
				"Normal",
				"Grayscale",
				"Blur",
				"Juicy",
				"Inverted",
			};
			ImGui::Combo("Surface Shader", &sandbox.effectMode, surf_modes, IM_ARRAYSIZE(surf_modes));
			ImGui::Separator();

			ImGui::Checkbox("Camera Orbit", &sandbox.cameraOrbit);
			ImGui::DragFloat("Camera Speed", &sandbox.cameraSpeed, 0.1f, -5.f, 5.f);
			ImGui::Separator();
		});

		// ImGui Builtin
		if (ev.editor.show_imgui_demo)		{ ml::ImGui_Builtin::showDemo(&ev.editor.show_imgui_demo); }
		if (ev.editor.show_imgui_metrics)	{ ml::ImGui_Builtin::showMetrics(&ev.editor.show_imgui_metrics); }
		if (ev.editor.show_imgui_style)		{ ml::ImGui_Builtin::showStyle(&ev.editor.show_imgui_style); }
		if (ev.editor.show_imgui_about)		{ ml::ImGui_Builtin::showAbout(&ev.editor.show_imgui_about); }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Sandbox::onUnload(const ml::UnloadEvent & ev)
	{
		ml::Debug::log("Unloading...");

		// Cleanup Physics Thread
		sandbox.physWorld.dispose();

		// Cleanup Resources
		ev.engine.resources().dispose();
	}

	void Sandbox::onExit(const ml::ExitEvent & ev)
	{
		ml::Debug::log("Exiting...");

		// Cleanup Std Out
		if (sandbox.rdbuf)
		{
			ml::cout.rdbuf(sandbox.rdbuf);
			sandbox.rdbuf = NULL;
		}

		// Shutdown ImGui
		ImGui_ML_Shutdown();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}