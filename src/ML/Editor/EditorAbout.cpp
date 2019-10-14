#include <ML/Editor/EditorAbout.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Editor/ImGuiExt.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/OS.hpp>

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
		if (!beginDraw(ImGuiWindowFlags_MenuBar))
		{ 
			return endDraw(); 
		}

		// Menu Bar
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::Button("Downloads")) OS::execute("open", "https://bit.ly/ml_noobs");
			if (ImGui::Button("Repository")) OS::execute("open", ML_PROJECT_URL);
			ImGui::EndMenuBar();
		}

		// Version String
		ImGui::Text("%s", String("{0} / {1} | {2} / {3} | {4} / {5}").format(
			ML_PROJECT_NAME,
			ML_PROJECT_VER,
			ML_CONFIGURATION,
			ML_PLATFORM_TARGET,
			ML_PROJECT_DATE,
			ML_PROJECT_TIME
		).c_str());

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

			// Project
			draw_head("Project");
			draw_def("define: ML_PROJECT_AUTH", "%s", ML_PROJECT_AUTH);
			draw_def("define: ML_PROJECT_DATE", "%s", ML_PROJECT_DATE);
			draw_def("define: ML_PROJECT_TIME", "%s", ML_PROJECT_TIME);
			draw_def("define: ML_PROJECT_URL", "%s", ML_PROJECT_URL);
			draw_def("define: ML_PROJECT_VER", "%s", ML_PROJECT_VER);
			
			// System
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

			// Compiler
			draw_head("Compiler");
# if defined(ML_CC_MSC)
			draw_def("define: ML_CC_MSC", "", "");
# elif defined(ML_CC_CLANG)
			draw_def("define: ML_CC_CLANG", "", "");
# elif defined(ML_CC_GNU)
			draw_def("define: ML_CC_GNU", "", "");
# elif defined(ML_CC_INTEL)
			draw_def("define: ML_CC_INTEL", "", "");
# elif defined(ML_CC_EMSCRIPTEN)
			draw_def("define: ML_CC_EMSCRIPTEN", "", "");
# endif
			draw_def("define: ML_CC_NAME", "%s", ML_CC_NAME);
			draw_def("define: ML_CC_VER", "%u", ML_CC_VER);

			// Language
			draw_head("Language");
			draw_def("define: ML_CPLUSPLUS", "%u", ML_CPLUSPLUS);
			draw_def("define: ML_HAS_CXX11", "%s", ML_HAS_CXX11 ? "true" : "false");
			draw_def("define: ML_HAS_CXX14", "%s", ML_HAS_CXX14 ? "true" : "false");
			draw_def("define: ML_HAS_CXX17", "%s", ML_HAS_CXX17 ? "true" : "false");

			// Config
			draw_head("Build");
			draw_def("define: ML_DEBUG", "%s", ML_DEBUG ? "true" : "false");
			draw_def("define: ML_ARCHITECTURE", "%u-bit", ML_ARCHITECTURE);
			draw_def("define: ML_CONFIGURATION", "%s", ML_CONFIGURATION);
			draw_def("define: ML_PLATFORM_TARGET", "%s", ML_PLATFORM_TARGET);

#if defined(ML_STATIC)
			draw_def("define: ML_STATIC", "", "");
# endif

# if defined (ML_API_EXPORT)
			draw_def("define: ML_API_EXPORT", "%s", ML_STRINGIFY(ML_API_EXPORT));
# endif

# if defined (ML_API_IMPORT)
			draw_def("define: ML_API_IMPORT", "%s", ML_STRINGIFY(ML_API_IMPORT));
# endif

			// Graphics
			draw_head("Graphics");
			draw_def("OpenGL Version", "%s", ML_GL.getString(GL::Version));
			draw_def("OpenGL Vendor", "%s", ML_GL.getString(GL::Vendor));
			draw_def("OpenGL Renderer", "%s", ML_GL.getString(GL::Renderer));

			ImGui::NextColumn();
			ImGui::Columns(1);
		}

		// Contibutors
		if (ImGui::CollapsingHeader("Contibutors"))
		{
			ImGui::Text("Created by Melody Gurman");
			ImGui::Text("Special thanks to Sajid Farooq and Champlain College for their help and support.");
		}
		
		// Third Party Software
		if (ImGui::CollapsingHeader("Third Party Software"))
		{
			auto link_item = ([](C_String label, C_String url)
			{
				if (ImGui::MenuItem(label, url)) OS::execute("open", url);
			});
			link_item("assimp", "https://github.com/assimp/assimp");
			link_item("cpython", "https://github.com/python/cpython");
			link_item("dirent", "https://github.com/tronkko/dirent");
			link_item("flac", "https://github.com/xiph/flac");
			link_item("FreeType", "https://www.freetype.org/");
			link_item("GCEM", "https://github.com/kthohr/gcem");
			link_item("GLEW", "https://github.com/nigels-com/glew");
			link_item("GLFW", "https://github.com/glfw/glfw");
			link_item("GLM", "https://github.com/g-truc/glm");
			link_item("ImGui", "https://github.com/ocornut/imgui");
			link_item("ImGuiColorTextEdit", "https://github.com/BalazsJako/ImGuiColorTextEdit");
			link_item("INIReader", "https://github.com/benhoyt/inih");
			link_item("Lua", "https://github.com/lua/lua");
			link_item("ogg", "https://github.com/xiph/ogg");
			link_item("OpenAL", "https://www.openal.org/");
			link_item("pdcurses", "https://github.com/wmcbrine/PDCurses");
			link_item("pybind11", "https://github.com/pybind/pybind11");
			link_item("RakNet", "http://www.jenkinssoftware.com/");
			link_item("stb", "https://github.com/nothings/stb");
			link_item("vorbis", "https://github.com/xiph/vorbis");
		}

		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}