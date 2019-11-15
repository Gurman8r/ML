#include <ML/Engine/Engine.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/CoreEvents.hpp>
#include <ML/Engine/Engine.hpp>
#include <ML/Engine/MetadataParser.hpp>
#include <ML/Engine/GameTime.hpp>
#include <ML/Engine/Plugin.hpp>
#include <ML/Engine/EngineEvents.hpp>
#include <ML/Engine/Registry.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Engine/PluginManager.hpp>
#include <ML/Engine/Python.hpp>
#include <ML/Engine/Lua.hpp>
#include <ML/Engine/Script.hpp>
#include <ML/Graphics/Camera.hpp>
#include <ML/Graphics/Material.hpp>
#include <ML/Graphics/Model.hpp>
#include <ML/Graphics/RenderWindow.hpp>
#include <ML/Graphics/Geometry.hpp>
#include <ML/Network/NetClient.hpp>
#include <ML/Network/NetServer.hpp>
#include <ML/Window/WindowEvents.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Engine::Engine()
		: m_time	{}
		, m_prefs	{ ML_INI_FILENAME }
		, m_plugins	{}
		, m_content	{}
		, m_commands{}
		, m_window	{}
	{
		ML_EventSystem.addListener<EnterEvent>(this);
		ML_EventSystem.addListener<LoadEvent>(this);
		ML_EventSystem.addListener<BeginStepEvent>(this);
		ML_EventSystem.addListener<UpdateEvent>(this);
		ML_EventSystem.addListener<EndDrawEvent>(this);
		ML_EventSystem.addListener<EndStepEvent>(this);
		ML_EventSystem.addListener<UnloadEvent>(this);
		ML_EventSystem.addListener<ExitEvent>(this);
		ML_EventSystem.addListener<CommandEvent>(this);
		ML_EventSystem.addListener<KeyEvent>(this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Engine::onEvent(const Event & value)
	{
		switch (*value)
		{
			case EnterEvent::ID: if (auto ev{ value.as<EnterEvent>() })
			{
				/* * * * * * * * * * * * * * * * * * * * */

				// Init Lua
				ML_ASSERT(ML_Lua.init());

				// Init Python
				ML_ASSERT(ML_Py.init(
					ev->argv[0],
					ML_FS.pathTo(prefs().get_string("Engine", "library_path", ""))
				));

				// Run Enter Script
				if (const Script scr{ prefs().get_string("Engine", "enter_script", "") })
				{
					scr.execute();
				}

				// Init Window
				ML_ASSERT(window().create(
					prefs().get_string	("Window", "title",			"MemeLib"), {
					prefs().get_uint	("Window", "width",			1280),
					prefs().get_uint	("Window", "height",		720),
					prefs().get_uint	("Window", "color_depth",	32) }, {
					prefs().get_bool	("Window", "resizable",		true),
					prefs().get_bool	("Window", "visible",		false),
					prefs().get_bool	("Window", "decorated",		true),
					prefs().get_bool	("Window", "focused",		true),
					prefs().get_bool	("Window", "auto_iconify",	true),
					prefs().get_bool	("Window", "floating",		false),
					prefs().get_bool	("Window", "maximized",		true) }, {
					prefs().get_uint	("Window", "major_version",	3),
					prefs().get_uint	("Window", "minor_version",	3),
					prefs().get_uint	("Window", "profile",		ContextSettings::Compat),
					prefs().get_uint	("Window", "depth_bits",	24),
					prefs().get_uint	("Window", "stencil_bits",	8),
					prefs().get_bool	("Window", "multisample",	false),
					prefs().get_bool	("Window", "srgb_capable",	false)
				}));
				if (prefs().get_bool("Window", "fullscreen", false))
				{
					window().setFullscreen(true);
				}

				/* * * * * * * * * * * * * * * * * * * * */
			} break;
			case LoadEvent::ID: if (auto ev{ value.as<LoadEvent>() })
			{
				/* * * * * * * * * * * * * * * * * * * * */

				// Load Defaults
				content().create<Texture>("tex/default")->loadFromImage(
					*content().create<Image>("img/default", Image::Default)
				);
				content().create<Model>("obj/default/triangle")->loadFromMemory(
					geo::triangle_static::vertices, geo::triangle_static::indices
				);
				content().create<Model>("obj/default/quad")->loadFromMemory(
					geo::quad_static::vertices, geo::quad_static::indices
				);
				content().create<Model>("obj/default/cube")->loadFromMemory(
					geo::cube_static::vertices, geo::cube_static::indices
				);
				content().create<Model>("obj/default/skybox")->loadFromMemory(
					geo::skybox_static::vertices
				);
				content().insert<Uniform>("u_delta", new uni_float_clbk{ "u_delta", [&]() {
					return time().deltaTime();
				} });
				content().insert<Uniform>("u_fps", new uni_float_clbk{ "u_fps", [&]() {
					return time().frameRate();
				} });
				content().insert<Uniform>("u_frame", new uni_int_clbk{ "u_frame", [&]() {
					return (int32_t)time().frameCount();
				} });
				content().insert<Uniform>("u_mouse", new uni_vec4_clbk{ "u_mouse", [&]() {
					return vec4 {
						window().getCursorPos()[0],
						window().getCursorPos()[1],
						(float_t)window().getMouseButton(MouseButton::Button0),
						(float_t)window().getMouseButton(MouseButton::Button1)
					};
				} });
				content().insert<Uniform>("u_time", new uni_float_clbk{ "u_time", [&]() {
					return time().totalTime();
				} });
				content().insert<Uniform>("u_resolution", new uni_vec2_clbk{ "u_resolution", [&]() {
					const auto c { Camera::mainCamera() };
					return c ? (vec2)c->viewport().size() : vec2{ 0 };
				} });

				// Run Load Script
				if (const Script scr{ prefs().get_string("Engine", "load_script", "") })
				{
					scr.execute();
				}

				// Set Window Icon
				if (auto icon{ content().get<Image>("_app_icon_") })
				{
					window().setIcon(icon->width(), icon->height(), icon->data());
				}

				/* * * * * * * * * * * * * * * * * * * * */
			} break;
			case BeginStepEvent::ID: if (auto ev{ value.as<BeginStepEvent>() })
			{
				/* * * * * * * * * * * * * * * * * * * * */

				time().beginStep();

				window().pollEvents();

				/* * * * * * * * * * * * * * * * * * * * */
			} break;
			case UpdateEvent::ID: if (auto ev{ value.as<UpdateEvent>() })
			{
				/* * * * * * * * * * * * * * * * * * * * */

				// Update Window Title
				static const String original_title{ window().getTitle() };
				static Timer tm{};
				static float_t dt{ 0 };
				if (tm.elapsed().count() > 0.25)
				{
					dt = time().deltaTime();
					tm.reset();
				}
				window().setTitle(String{ "{0} | {1} | {2} | {3}s/frame" }.format(
					original_title,
					ML_CONFIGURATION,
					ML_PLATFORM_TARGET,
					dt
				));

				/* * * * * * * * * * * * * * * * * * * * */
			} break;
			case EndDrawEvent::ID: if (auto ev{ value.as<EndDrawEvent>() })
			{
				/* * * * * * * * * * * * * * * * * * * * */

				window().swapBuffers();

				/* * * * * * * * * * * * * * * * * * * * */
			} break;
			case EndStepEvent::ID: if (auto ev{ value.as<EndStepEvent>() })
			{
				/* * * * * * * * * * * * * * * * * * * * */

				time().endStep();

				/* * * * * * * * * * * * * * * * * * * * */
			} break;
			case UnloadEvent::ID: if (auto ev{ value.as<UnloadEvent>() })
			{
				/* * * * * * * * * * * * * * * * * * * * */

				content().dispose();

				commands().dispose();

				/* * * * * * * * * * * * * * * * * * * * */
			} break;
			case ExitEvent::ID: if (auto ev{ value.as<ExitEvent>() })
			{
				/* * * * * * * * * * * * * * * * * * * * */

				plugins().dispose();
			
				window().dispose();
			
				ML_Lua.dispose();
			
				ML_Py.dispose();

				/* * * * * * * * * * * * * * * * * * * * */
			} break;
			case CommandEvent::ID: if (auto ev{ value.as<CommandEvent>() })
			{
				/* * * * * * * * * * * * * * * * * * * * */

				commands().execute(value.as<CommandEvent>()->cmd);

				/* * * * * * * * * * * * * * * * * * * * */
			} break;
			case KeyEvent::ID: if (auto ev = value.as<KeyEvent>())
			{
				/* * * * * * * * * * * * * * * * * * * * */

				static String secret{ "" };
				if (ev->getPress(KeyCode::Up)) secret += "u";
				if (ev->getPress(KeyCode::Down)) secret += "d";
				if (ev->getPress(KeyCode::Left)) secret += "l";
				if (ev->getPress(KeyCode::Right)) secret += "r";
				if (ev->getPress(KeyCode::A)) secret += "a";
				if (ev->getPress(KeyCode::B)) secret += "b";
				if (ev->getPress(KeyCode::Enter))
				{
					if (secret == "uuddlrlrba")
					{
						ML_EventSystem.fireEvent<SecretEvent>();
					}
					secret.clear();
				}

				/* * * * * * * * * * * * * * * * * * * * */
			} break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}