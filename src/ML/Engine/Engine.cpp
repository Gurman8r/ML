#include <ML/Engine/Engine.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Engine/Ref.hpp>
#include <ML/Engine/MetadataParser.hpp>
#include <ML/Engine/CommandRegistry.hpp>
#include <ML/Engine/GameTime.hpp>
#include <ML/Engine/Plugin.hpp>
#include <ML/Engine/EngineEvents.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Engine/PluginLoader.hpp>
#include <ML/Engine/Python.hpp>
#include <ML/Graphics/GLM.hpp>
#include <ML/Graphics/Material.hpp>
#include <ML/Graphics/Model.hpp>
#include <ML/Graphics/RenderWindow.hpp>
#include <ML/Graphics/Geometry.hpp>
#include <ML/Network/NetClient.hpp>
#include <ML/Network/NetServer.hpp>
#include <ML/Window/WindowEvents.hpp>

namespace ml
{
	static inline mat4 LOOK_AT(const vec3 & eye, const vec3 & center, const vec3 & up)
	{
		const vec3 f = alg::normalize(center - eye);
		const vec3 s = alg::normalize(alg::cross(f, up));
		const vec3 u = alg::cross(s, f);
		mat4 m { 1 };
		m[ 0] = s[0];
		m[ 1] = u[0];
		m[ 2] = -f[0];
		m[ 4] = s[1];
		m[ 5] = u[1];
		m[ 6] = -f[1];
		m[ 8] = s[2];
		m[ 9] = u[2];
		m[10] = -f[2];
		m[12] = -alg::dot(s, eye);
		m[13] = -alg::dot(u, eye);
		m[14] = alg::dot(f, eye);
		return m;
	}

	static inline mat4 PERSP(float_t fov, float_t aspect, float_t zNear, float_t zFar)
	{
		mat4 m { uninit };
		m[0 * 4 + 0] = 1.0f / (aspect * std::tan(fov / 2.0f));
		m[1 * 4 + 1] = 1.0f / std::tan(fov / 2.0f);
		m[2 * 4 + 3] = -1.0f;
		m[2 * 4 + 2] = -(zFar + zNear) / (zFar - zNear);
		m[3 * 4 + 2] = -(2.0f * zFar * zNear) / (zFar - zNear);
		return m;
	}
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Engine::Engine(EventSystem & eventSystem)	
		: I_EventListener { eventSystem }
	{
		eventSystem.addListener(EnterEvent		::ID, this);
		eventSystem.addListener(LoadEvent		::ID, this);
		eventSystem.addListener(StartEvent		::ID, this);
		eventSystem.addListener(BeginFrameEvent	::ID, this);
		eventSystem.addListener(UpdateEvent		::ID, this);
		eventSystem.addListener(BeginDrawEvent	::ID, this);
		eventSystem.addListener(DrawEvent		::ID, this);
		eventSystem.addListener(EndDrawEvent	::ID, this);
		eventSystem.addListener(EndFrameEvent	::ID, this);
		eventSystem.addListener(UnloadEvent		::ID, this);
		eventSystem.addListener(ExitEvent		::ID, this);
		eventSystem.addListener(CommandEvent	::ID, this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Engine::onEvent(const Event & value)
	{
		switch (*value)
		{
		case EnterEvent::ID		: if (auto ev = value.as<EnterEvent>())		return onEnter(*ev);
		case LoadEvent::ID		: if (auto ev = value.as<LoadEvent>())		return onLoad(*ev);
		case StartEvent::ID		: if (auto ev = value.as<StartEvent>())		return onStart(*ev);
		case BeginFrameEvent::ID: if (auto ev = value.as<BeginFrameEvent>())return onBeginFrame(*ev);
		case UpdateEvent::ID	: if (auto ev = value.as<UpdateEvent>())	return onUpdate(*ev);
		case BeginDrawEvent::ID	: if (auto ev = value.as<BeginDrawEvent>())	return onBeginDraw(*ev);
		case DrawEvent::ID		: if (auto ev = value.as<DrawEvent>())		return onDraw(*ev);
		case EndDrawEvent::ID	: if (auto ev = value.as<EndDrawEvent>())	return onEndDraw(*ev);
		case EndFrameEvent::ID	: if (auto ev = value.as<EndFrameEvent>())	return onEndFrame(*ev);
		case UnloadEvent::ID	: if (auto ev = value.as<UnloadEvent>())	return onUnload(*ev);
		case ExitEvent::ID		: if (auto ev = value.as<ExitEvent>())		return onExit(*ev);

		case CommandEvent::ID:
			if (auto ev = value.as<CommandEvent>()) { ML_CommandRegistry.execute(ev->cmd); }
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Engine::onEnter(const EnterEvent & ev)
	{
		// Setup Python
		/* * * * * * * * * * * * * * * * * * * * */
		if (const String script_path { ev.prefs.get_string("Engine", "script_path", "") })
		{
			Py_SetPythonHome(alg::widen(ML_FS.pathTo(script_path)).c_str());
		}

		// Boot Script
		/* * * * * * * * * * * * * * * * * * * * */
		Py::RunFile(ev.prefs.get_string("Engine", "boot_script", ""));

		// Create Window
		/* * * * * * * * * * * * * * * * * * * * */
		if (!ev.window.create(
			ev.prefs.get_string	("Window", "title",				""), { 
			ev.prefs.get_uint	("Window", "width",				1280),
			ev.prefs.get_uint	("Window", "height",			720),
			ev.prefs.get_uint	("Window", "bits_per_pixel",	32) }, {
			ev.prefs.get_bool	("Window", "resizable",			true),
			ev.prefs.get_bool	("Window", "visible",			false),
			ev.prefs.get_bool	("Window", "decorated",			true),
			ev.prefs.get_bool	("Window", "focused",			true),
			ev.prefs.get_bool	("Window", "auto_iconify",		true),
			ev.prefs.get_bool	("Window", "floating",			false),
			ev.prefs.get_bool	("Window", "maximized",			true) }, {
			ev.prefs.get_uint	("Window", "major_version",		3),
			ev.prefs.get_uint	("Window", "minor_version",		3),
			ev.prefs.get_uint	("Window", "context_profile",	ContextSettings::Compat),
			ev.prefs.get_uint	("Window", "depth_bits",		24),
			ev.prefs.get_uint	("Window", "stencil_bits",		8),
			ev.prefs.get_bool	("Window", "multisample",		false),
			ev.prefs.get_bool	("Window", "srgb_capable",		false)
		}))
		{
			Debug::fatal("Failed Creating Window");
		}
		else if (ev.prefs.get_bool("Window", "fullscreen", false))
		{
			ev.window.setFullscreen(true);
		}
	}

	void Engine::onLoad(const LoadEvent & ev)
	{
		// Load Defaults
		/* * * * * * * * * * * * * * * * * * * * */
		ML_Content.create<Texture>("default_texture")->loadFromImage(
			ML_Content.create<Image>("default_image")->update(
				{ 512, 512 }, 4, Color32::magenta
			)
		);

		ML_Content.create<Model>("default_triangle")->loadFromMemory(
			geo::tri::vertices,
			geo::tri::indices
		);
		ML_Content.create<Model>("default_quad")->loadFromMemory(
			geo::quad::vertices,
			geo::quad::indices
		);
		ML_Content.create<Model>("default_cube")->loadFromMemory(
			geo::cube::vertices,
			geo::cube::indices
		);
		ML_Content.create<Model>("skybox")->loadFromMemory(
			geo::sky::vertices
		);

		ML_Content.insert<Uniform>("u_cursorPos",	new uni_vec2_ptr ("u_cursorPos",	&m_cursorPos));
		ML_Content.insert<Uniform>("u_deltaTime",	new uni_float_ptr("u_deltaTime",	&m_deltaTime));
		ML_Content.insert<Uniform>("u_frameCount",	new uni_int_ptr	 ("u_frameCount",	&m_frameCount));
		ML_Content.insert<Uniform>("u_frameRate",	new uni_float_ptr("u_frameRate",	&m_frameRate));
		ML_Content.insert<Uniform>("u_totalTime",	new uni_float_ptr("u_totalTime",	&m_totalTime));
		ML_Content.insert<Uniform>("u_viewport",	new uni_vec2_ptr ("u_viewport",		&m_viewport));
		//ML_Content.insert<Uniform>("u_view",		new uni_mat4_ptr ("u_view",			&m_proj));
		//ML_Content.insert<Uniform>("u_proj",		new uni_mat4_ptr ("u_proj",			&m_view));

		
		// Run Load Script
		/* * * * * * * * * * * * * * * * * * * * */
		Py::RunFile(ev.prefs.get_string("Engine", "load_script", ""));


		// Set Window Icon
		/* * * * * * * * * * * * * * * * * * * * */
		if (Ref<Image> icon { "icon" })
		{
			ev.window.setIcon(icon->width(), icon->height(), icon->data());
		}
	}

	void Engine::onStart(const StartEvent & ev)
	{
	}

	void Engine::onBeginFrame(const BeginFrameEvent & ev)
	{
		ev.time.beginLoop();
		ev.window.pollEvents();
	}

	void Engine::onUpdate(const UpdateEvent & ev)
	{
		// Update Default Uniforms
		m_cursorPos		= (vec2)ev.window.getCursorPos();
		m_deltaTime		= ev.time.elapsed().delta();
		m_frameCount	= m_frameCount + 1;
		m_frameRate		= (float_t)ev.time.frameRate();
		m_viewport		= (vec2)ev.window.size();
		m_totalTime		= ev.time.total().delta();
		m_view			= LOOK_AT({ 0.f, 0.f, 3.f }, { 0.f, 0.f, -1.f }, { 0.f, 1.f, 0.f });
		m_proj			= PERSP(45.f, ML_ASPECT_2(m_viewport), 0.001f, 1000.f);

		// Update Window Title
		static const String original_title { ev.window.title() };
		ev.window.setTitle(String("{0} | {1} | {2}").format(
			original_title, ML_CONFIGURATION, ML_PLATFORM_TARGET
		));
	}

	void Engine::onBeginDraw(const BeginDrawEvent & ev)
	{
	}

	void Engine::onDraw(const DrawEvent & ev)
	{
	}

	void Engine::onEndDraw(const EndDrawEvent & ev)
	{
	}

	void Engine::onEndFrame(const EndFrameEvent & ev)
	{
		ev.window.makeContextCurrent();
		ev.window.swapBuffers();
		ev.time.endLoop();
	}

	void Engine::onUnload(const UnloadEvent & ev)
	{
		ML_Content.dispose();
	}

	void Engine::onExit(const ExitEvent & ev)
	{
		// Dispose Window
		ev.window.dispose();
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}