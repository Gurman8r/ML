/* * * * * * * * * * * * * * * * * * * * */

#include "Noobs.hpp"

#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/FileSystem.hpp> 
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/EditorUtility.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Engine/GameTime.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Engine/Resources.hpp>
#include <ML/Graphics/Font.hpp>
#include <ML/Graphics/Model.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Graphics/Renderer.hpp>
#include <ML/Graphics/Sprite.hpp>
#include <ML/Graphics/Surface.hpp>
#include <ML/Graphics/RenderWindow.hpp>
#include <ML/Graphics/Uniform.hpp>
#include <ML/Graphics/ShaderAPI.hpp>
#include <ML/Script/Interpreter.hpp>
#include <ML/Script/Script.hpp>
#include <ML/Window/WindowEvents.hpp>

/* * * * * * * * * * * * * * * * * * * * */

ML_PLUGIN_API ml::Plugin * ML_Plugin_Main(ml::EventSystem & eventSystem)
{
	return new DEMO::Noobs(eventSystem);
}

/* * * * * * * * * * * * * * * * * * * * */

namespace DEMO
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Noobs::Noobs(ml::EventSystem & eventSystem)
		: EditorPlugin(eventSystem)
	{
		eventSystem.addListener(ml::CommandEvent::ID, this);
		eventSystem.addListener(ml::KeyEvent::ID, this);
		eventSystem.addListener(ml::BuildDockspaceEvent::ID, this);
	}

	Noobs::~Noobs() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Noobs::onEvent(const ml::Event * value)
	{
		// Handle base events
		ml::EditorPlugin::onEvent(value);

		switch (*value)
		{
			// Command Event
			/* * * * * * * * * * * * * * * * * * * * */
		case ml::CommandEvent::ID:
			if (auto ev = value->as<ml::CommandEvent>())
			{
				ml::Var v;
				if ((v = ML_Interpreter.execCommand(ev->cmd)).isErrorType())
				{
					ml::Debug::logError(v.errorValue());
				}
			}
			break;

			// Key Event
			/* * * * * * * * * * * * * * * * * * * * */
		case ml::KeyEvent::ID:
			if (auto ev = value->as<ml::KeyEvent>())
			{
			}
			break;


			// Build Dockspace Event
			/* * * * * * * * * * * * * * * * * * * * */
		case ml::BuildDockspaceEvent::ID:
			if (auto ev = value->as<ml::BuildDockspaceEvent>())
			{
				ml::DockspaceGui & d = ev->dockspace;
				d.dockWindow("Noobs Scene", d.getNode(d.MidUp));
			}
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Noobs::onEnter(const ml::EnterEvent & ev)
	{
		// Hello!
		ml::Debug::log("Hello from {0}!", typeid(*this).name());
	}

	void Noobs::onStart(const ml::StartEvent & ev)
	{
		// Surfaces
		self.sf.main = ev.resources.surfaces.get("sf_noobs_main");
		self.sf.post = ev.resources.surfaces.get("sf_noobs_post");

		// Orthographic
		ml::Transform ortho = ml::Transform::Orthographic({
			{ 0.f, 0.f }, { 1920.f, 1080.f }
		});

		// Perspective
		ml::Transform persp = ml::Transform::Perspective(
			45.f, ML_ASPECT(1920.f, 1080.f), 0.001f, 1000.f
		);

		// Camera Transform
		ml::Transform camera = ml::Transform(
			{ 0.0f, 0.0f, 10.0f }, 
			{ 1.0f }, 
			{ } 
		);
		camera.lookAt(
			camera.getPos(),
			(camera.getPos() - ml::vec3::Back).normalized()
		);

		// Light Transform
		ml::Transform light = ml::Transform(
			{ 0.0f, 0.0f, 30.0f },
			{ 1.0f },
			{ }
		);

		// Model Transform
		ml::Transform model = ml::Transform(
			{ 0.0f }, 
			{ 1.0f }, 
			{ }
		);

		// Entity Material
		const ml::Material * material = ev.resources.materials.load_forward(
			"noobs_material",
			ev.resources.shaders.get("noobs_shader"),
			ml::List<ml::uni_base *>({
				new ml::uni_mat4("Vert.proj",		persp.getMat()),
				new ml::uni_mat4("Vert.view",		camera.getMat()),
				new ml::uni_mat4("Vert.model",		model.getMat()),
				new ml::uni_vec3("Frag.cameraPos",	camera.getPos()),
				new ml::uni_vec3("Frag.lightPos",	light.getPos()),
				new ml::uni_col4("Frag.diffuse",	ml::Color::LightYellow),
				new ml::uni_col4("Frag.mainCol",	ml::Color::White),
				new ml::uni_tex2("Frag.mainTex",	ev.resources.textures.get("earth_dm")),
				new ml::uni_tex2("Frag.specTex",	ev.resources.textures.get("earth_sm")),
				new ml::uni_flt1("Frag.ambient",	0.01f),
				new ml::uni_flt1("Frag.specular",	0.1f),
				new ml::uni_int1("Frag.shininess",	8),
				}));

		// Create Entity
		if (ml::Entity * ent = ev.resources.entities.load("noobs_entity_0"))
		{
			// Entity Renderer
			ml::Renderer * r = ent->add<ml::Renderer>(
				ev.resources.models.get("sphere32x24"),
				material
			);
		}
	}

	void Noobs::onUpdate(const ml::UpdateEvent & ev)
	{
		// Update Surface Sizes
		self.sf.main->resize(ev.window.getFrameSize());
		self.sf.post->resize(ev.window.getFrameSize());
	}

	void Noobs::onDraw(const ml::DrawEvent & ev)
	{
		// Clear Screen
		ev.window.clear(ml::Color::Gray);

		// Bind Main Surface
		self.sf.main->bind();

		// Clear Screen
		ev.window.clear(ml::Color::Gray);

		// Draw Renderers
		for (const auto & pair : ev.resources.entities)
		{
			if (pair.first.find("noobs") != ml::String::npos)
				ev.window.draw(pair.second->get<ml::Renderer>());
		}

		// Finalize States
		static ml::RenderStates states(
			{ true, ml::GL::Greater, 0.01f },
			{ true },
			{ false },
			{ false },
			{ true, ml::GL::Texture2D, ml::GL::Texture0 },
			{ false, false }
		);
		states.apply();

		// Unbind Main Surface
		self.sf.main->unbind();

		// Bind Post Surface
		self.sf.post->bind();
		
		// Draw Main Surface
		if (const ml::Shader * shader = self.sf.main->shader())
		{
			shader->setUniform("Effect.mode", 0);
		}
		ev.window.draw(*self.sf.main);

		// Unbind Post Surface
		self.sf.post->unbind();
	}

	void Noobs::onGui(const ml::GuiEvent & ev)
	{
		// Noobs Scene
		/* * * * * * * * * * * * * * * * * * * * */
		static bool showScene = true;
		ML_EditorUtility.DrawWindow(
			"Noobs Scene",
			&showScene,
			ImGuiWindowFlags_MenuBar,
			[&]()
		{
			/* * * * * * * * * * * * * * * * * * * * */

			if (ImGui::BeginMenuBar())
			{
				ImGui::Text("Noobs Scene");
				ImGui::EndMenuBar();
			}

			/* * * * * * * * * * * * * * * * * * * * */

			ml::Texture * texture = &self.sf.post->texture();
			if (!texture || !(*texture))
				return;

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

			/* * * * * * * * * * * * * * * * * * * * */
		});
	}

	void Noobs::onExit(const ml::ExitEvent & ev)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}