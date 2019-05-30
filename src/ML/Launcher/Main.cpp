/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Engine/Plugin.hpp>
#include <ML/Engine/Engine.hpp>
#include <ML/Engine/GameTime.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Engine/Resources.hpp>
#include <ML/Engine/SharedLibrary.hpp>
#include <ML/Engine/StateMachine.hpp>
#include <ML/Graphics/RenderWindow.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

# ifndef ML_CONFIG_INI
# define ML_CONFIG_INI "../../../ML.ini"
# endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace ml;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static Preferences	g_Preferences	{ ML_CONFIG_INI };
static EventSystem	g_EventSystem	{};
static Resources	g_Resources		{};
static GameTime		g_Time			{};
static RenderWindow g_Window		{ g_EventSystem };
static Engine		g_Engine		{ g_EventSystem };
static Editor		g_Editor		{ g_EventSystem };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

enum State { Startup, Loop, Shutdown };

static StateMachine<State> g_ControlFlow
{
{ State::Startup, []()
{	/* Enter */
	/* * * * * * * * * * * * * * * * * * * * */
	g_EventSystem.fireEvent(EnterEvent(
		g_Preferences,
		g_Window
	));

	/* Load */
	/* * * * * * * * * * * * * * * * * * * * */
	g_EventSystem.fireEvent(LoadEvent(
		g_Preferences,
		g_Resources
	));

	/* Start */
	/* * * * * * * * * * * * * * * * * * * * */
	g_EventSystem.fireEvent(StartEvent(
		g_Time,
		g_Resources,
		g_Window
	));
	return g_ControlFlow(State::Loop);
} },
{ State::Loop, []()
{	while (g_Window.isOpen())
	{
		/* Begin Frame */
		/* * * * * * * * * * * * * * * * * * * * */
		g_EventSystem.fireEvent(BeginFrameEvent(
			g_Time, 
			g_Window
		));

		/* Update */
		/* * * * * * * * * * * * * * * * * * * * */
		g_EventSystem.fireEvent(UpdateEvent(
			g_Time,
			g_Resources,
			g_Window
		));

		/* Draw */
		/* * * * * * * * * * * * * * * * * * * * */
		g_EventSystem.fireEvent(BeginDrawEvent());
		g_EventSystem.fireEvent(DrawEvent(
			g_Time,
			g_Resources,
			g_Window
		));
		g_EventSystem.fireEvent(EndDrawEvent());

		/* Gui */
		/* * * * * * * * * * * * * * * * * * * * */
		g_EventSystem.fireEvent(BeginGuiEvent());
		g_EventSystem.fireEvent(GuiEvent(
			g_Time,
			g_Resources,
			g_Editor
		));
		g_EventSystem.fireEvent(EndGuiEvent());

		/* End Frame */
		/* * * * * * * * * * * * * * * * * * * * */
		g_EventSystem.fireEvent(EndFrameEvent(
			g_Time, 
			g_Window
		));
	}
	return g_ControlFlow(State::Shutdown);
} },
{ State::Shutdown, []()
{	/* Unload */
	/* * * * * * * * * * * * * * * * * * * * */
	g_EventSystem.fireEvent(UnloadEvent(
		g_Resources,
		g_Window
	));

	/* Exit */
	/* * * * * * * * * * * * * * * * * * * * */
	g_EventSystem.fireEvent(ExitEvent());
	return g_ControlFlow.NoState;
} },
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/Meta.hpp>

inline static int32_t meta_tests()
{
	constexpr meta::mat4f ma {
		0.f,	1.f,	2.f,	3.f,
		4.f,	5.f,	6.f,	7.f,
		8.f,	9.f,	10.f,	11.f,
		12.f,	13.f,	14.f,	15.f
	};

	constexpr meta::mat4f mb = meta::alg::transpose(ma);

	cout
		<< ma << endl
		<< mb << std::endl;

	constexpr meta::vec3f v3 {};
	constexpr meta::mat3f m3 {};
	constexpr meta::mat4f m4 {};
	constexpr meta::mat4f m5 = m4;
	constexpr meta::mat4i m6 = (meta::mat4i)mb;

	constexpr auto rebaseV3 = meta::alg::rebase(v3, m4);
	constexpr auto rebaseM3 = meta::alg::rebase(m3, m4);

	constexpr auto va		= meta::vec2 { 0.f, 0.f };
	constexpr auto vb		= meta::vec2 { -10.f, -10.f };
	constexpr auto vc		= meta::alg::lerp(va, vb, meta::vec2::type::half);

	constexpr auto hash1	= meta::alg::hash()("Here");
	constexpr auto hash2	= meta::c_string("Here").hash();
	constexpr auto hash3	= meta::mat4i::identity().hash();
	constexpr auto hash4	= meta::mat4f::identity().hash();

	constexpr auto eps		= meta::value_t<long double>::epsilon;
	constexpr auto sqr_mag	= meta::alg::sqr_magnitude(vb);
	constexpr auto mag		= meta::alg::magnitude(vb);
	constexpr auto norm		= meta::alg::normalize(vb);
	constexpr auto lerp		= meta::alg::lerp(va, vb, 0.5f);
	constexpr auto sqr		= meta::alg::sqrt<float>()(13.0f);
	constexpr auto det		= meta::alg::determinant(ma);
	constexpr auto inv		= meta::alg::inverse(ma);
	constexpr auto dot		= meta::alg::dot(ma, mb);
	constexpr auto pow		= meta::alg::pow(1.23, 10);
	constexpr auto fact		= meta::alg::fact(10);

	constexpr auto tri		= meta::geometry::tri::contiguous;
	constexpr auto quat		= meta::geometry::quad::contiguous;
	constexpr auto cube		= meta::geometry::cube::contiguous;

	constexpr auto res		= meta::vec2 { 1280, 720 };
	constexpr auto fov		= 45.f;
	constexpr auto aspect	= (res[0] / res[1]);
	constexpr auto zNear	= 0.1f;
	constexpr auto zFar		= 1000.f;
	constexpr auto ortho	= meta::mat4::ortho(0, res[0], res[1], 0);
	constexpr auto persp	= meta::mat4::persp(fov, aspect, zNear, zFar);

	return Debug::pause(0);
}

int32_t main()
{
#if ML_DEBUG
	return meta_tests();
#endif

	// Setup
	Map<SharedLibrary *, Plugin *> plugins;
	if (auto file = std::ifstream(ML_FS.getPathTo(g_Preferences.GetString(
		"Launcher",
		"plugin_list",
		"../../../assets/data/plugins.txt"
	))))
	{	
		String line;
		while (std::getline(file, line))
		{
			if (line.empty() || (line.trim().front() == '#'))
				continue;
			
			// Load Library
			auto library = new SharedLibrary(ML_FS.getPathTo(line
				.replaceAll("$(Configuration)", ML_CONFIGURATION)
				.replaceAll("$(PlatformTarget)", ML_PLATFORM_TARGET)
			));

			// Load Plugin
			auto plugin = plugins.insert({
				library,
				library->callFunction<Plugin *>(ML_str(ML_Plugin_Main), g_EventSystem)
			}).first->second;
		}
		file.close();
	}

	// Run
	g_ControlFlow(State::Startup);

	// Cleanup
	for (auto & pair : plugins)
	{
		if (pair.second) { delete pair.second; }
		if (pair.first)  { delete pair.first;  }
	}

	// Goodbye!
	return EXIT_SUCCESS;

	/* * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */