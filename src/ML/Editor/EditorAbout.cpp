#include <ML/Editor/EditorAbout.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Editor/ImGuiExt.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EditorAbout::EditorAbout()
		: EditorWindow { "About", "Ctrl+Alt+A", false }
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void EditorAbout::update()
	{
	}

	bool EditorAbout::draw()
	{
		if (!beginDraw(ImGuiWindowFlags_MenuBar)) { return endDraw(); }

		// Menu Bar
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::Button("Downloads")) Debug::execute("open", "https://bit.ly/ml_noobs");
			if (ImGui::Button("Repository")) Debug::execute("open", ML_PROJECT_URL);
			ImGui::EndMenuBar();
		}

		// Version String
		ImGui::Text("%s/%s | %s/%s | %s/%s",
			ML_PROJECT_NAME,
			ML_PROJECT_VER,
			ML_CONFIGURATION,
			ML_PLATFORM_TARGET,
			ML_PROJECT_DATE,
			ML_PROJECT_TIME
		);

		// Config/Build Information
		if (ImGui::CollapsingHeader("Config/Build Information"))
		{
			auto draw_def = ([](C_String label, C_String fmt, auto && data)
			{
				ImGui::Text(label);
				ImGui::NextColumn();
				ImGui::Text(fmt, data);
				ImGui::NextColumn();
			});

			auto draw_head = ([&](C_String label) 
			{
				ImGui::Separator(); draw_def(label, "", "");
			});

			ImGui::Columns(2);

			// Build
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
			draw_head("Build");
			draw_def("define: ML_DEBUG", "%s", ML_DEBUG ? "true" : "false");
			draw_def("define: ML_CONFIGURATION", "%s", ML_CONFIGURATION);

#if defined(ML_STATIC)
			draw_def("define: ML_STATIC", "", "");
# endif

# if defined (ML_API_EXPORT)
			draw_def("define: ML_API_EXPORT", "", "");
# endif

# if defined (ML_API_IMPORT)
			draw_def("define: ML_API_IMPORT", "", "");
# endif

			// Compiler
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
			draw_head("Compiler");
# if defined(ML_CC_MSC)
			draw_def("define: ML_CC_MSC", "", "");
# elif defined(ML_CC_CLANG)
			draw_def("define: ML_CC_CLANG", "", "");
# elif defined(ML_CC_GCC)
			draw_def("define: ML_CC_GCC", "", "");
# elif defined(ML_CC_INTEL)
			draw_def("define: ML_CC_INTEL", "", "");
# elif defined(ML_CC_EMSCRIPTEN)
			draw_def("define: ML_CC_EMSCRIPTEN", "", "");
# endif
			draw_def("define: ML_CC_NAME", "%s", ML_CC_NAME);
			draw_def("define: ML_CC_VER", "%u", ML_CC_VER);

			// Graphics
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
			draw_head("Graphics");
			draw_def("OpenGL Version", "%s", ML_GL.getString(GL::Version));
			draw_def("OpenGL Vendor", "%s", ML_GL.getString(GL::Vendor));
			draw_def("OpenGL Renderer", "%s", ML_GL.getString(GL::Renderer));

			// Language
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
			draw_head("Language");
			draw_def("define: ML_CPP", "%u", ML_CPP);
# ifdef ML_HAS_CXX11
			draw_def("define: ML_HAS_CXX11", "", "");
# endif
#ifdef ML_HAS_CXX14
			draw_def("define: ML_HAS_CXX14", "", "");
#endif
#ifdef ML_HAS_CXX17
			draw_def("define: ML_HAS_CXX17", "", "");
#endif
#ifdef ML_HAS_CXX20
			draw_def("define: ML_HAS_CXX20", "", "");
#endif
#ifdef ML_CPP_CONSTEXPR
			draw_def("define: ML_CPP_CONSTEXPR", "%u", ML_CPP_CONSTEXPR);
#endif
			
			// Platform
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
			draw_head("Platform");
#ifdef ML_X64
			draw_def("define: ML_X64", "", "");
#endif
#ifdef ML_X86
			draw_def("define: ML_X86", "", "");
#endif
#ifdef ML_ARM64
			draw_def("define: ML_ARM64", "", "");
#endif
#ifdef ML_ARM32
			draw_def("define: ML_ARM32", "", "");
#endif
#ifdef ML_PPC64
			draw_def("define: ML_PPC64", "", "");
#endif
#ifdef ML_PPC32
			draw_def("define: ML_PPC32", "", "");
#endif
			draw_def("define: ML_ARCHITECTURE", "%u-bit", ML_ARCHITECTURE);
			draw_def("define: ML_PLATFORM_TARGET", "%s", ML_PLATFORM_TARGET);

			// Project
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
			draw_head("Project");
			draw_def("define: ML_PROJECT_AUTH", "%s", ML_PROJECT_AUTH);
			draw_def("define: ML_PROJECT_DATE", "%s", ML_PROJECT_DATE);
			draw_def("define: ML_PROJECT_TIME", "%s", ML_PROJECT_TIME);
			draw_def("define: ML_PROJECT_URL", "%s", ML_PROJECT_URL);
			draw_def("define: ML_PROJECT_VER", "%s", ML_PROJECT_VER);

			// System
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
			draw_head("System");
# if defined(ML_SYSTEM_WINDOWS)
			draw_def("define: ML_SYSTEM_WINDOWS", "", "");
# elif defined(ML_SYSTEM_APPLE)
			draw_def("define: ML_SYSTEM_APPLE", "", "");
# elif defined(ML_SYSTEM_UNIX)
			draw_def("define: ML_SYSTEM_UNIX", "", "");
#	if defined(ML_SYSTEM_ANDROID"
			draw_def("define: ML_SYSTEM_ANDROID", "", "");
#	elif defined(ML_SYSTEM_LINUX"
			draw_def("define: ML_SYSTEM_LINUX", "", "");
#	elif defined(ML_SYSTEM_FREEBSD"
			draw_def("define: ML_SYSTEM_FREEBSD", "", "");
#	endif
# endif
			draw_def("define: ML_SYSTEM_NAME", "%s", ML_SYSTEM_NAME);

			ImGui::NextColumn();
			ImGui::Columns(1);
			ImGui::Separator();
		}

		// Contibutors
		if (ImGui::CollapsingHeader("Contibutors"))
		{
			ImGui::TextWrapped(
				"Created by Melody Gurman\n"
				"Special thanks to Sajid Farooq and Champlain College for their help and support.\n"
			);
		}

		// Disclaimer
		if (ImGui::CollapsingHeader("Disclaimer"))
		{
			ImGui::TextWrapped(
				"Please note that I am often kind of an idiot.\n"
				"I am not an expert, I am just a person.\n"
				"Everything here should be considered experimental.\n"
				"Never take my word for anything, do your own research.\n"
			);
		}
		
		// Third Party Software
		if (ImGui::CollapsingHeader("Third Party Software"))
		{
			auto draw_lnk = ([](C_String label, C_String url)
			{
				if (ImGui::MenuItem(label, url)) Debug::execute("open", url);
			});
			draw_lnk("assimp", "https://github.com/assimp/assimp");
			draw_lnk("cpython", "https://github.com/python/cpython");
			draw_lnk("dirent", "https://github.com/tronkko/dirent");
			draw_lnk("flac", "https://github.com/xiph/flac");
			draw_lnk("FreeType", "https://www.freetype.org/");
			draw_lnk("GCEM", "https://github.com/kthohr/gcem");
			draw_lnk("GLEW", "https://github.com/nigels-com/glew");
			draw_lnk("GLFW", "https://github.com/glfw/glfw");
			draw_lnk("GLM", "https://github.com/g-truc/glm");
			draw_lnk("ImGui", "https://github.com/ocornut/imgui");
			draw_lnk("ImGuiColorTextEdit", "https://github.com/BalazsJako/ImGuiColorTextEdit");
			draw_lnk("INIReader", "https://github.com/benhoyt/inih");
			draw_lnk("Lua", "https://github.com/lua/lua");
			draw_lnk("ogg", "https://github.com/xiph/ogg");
			draw_lnk("OpenAL", "https://www.openal.org/");
			draw_lnk("pdcurses", "https://github.com/wmcbrine/PDCurses");
			draw_lnk("pybind11", "https://github.com/pybind/pybind11");
			draw_lnk("premake5", "https://github.com/premake/premake-core");
			draw_lnk("RakNet", "http://www.jenkinssoftware.com/");
			draw_lnk("stb", "https://github.com/nothings/stb");
			draw_lnk("vorbis", "https://github.com/xiph/vorbis");
		}

		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}