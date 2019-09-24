#include <ML/Editor/EditorAbout.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Editor/ImGuiExt.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/OS.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EditorAbout::EditorAbout(Editor & editor)
		: EditorWindow { editor, "About", "Ctrl+Alt+A", false }
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void EditorAbout::onUpdate(const UpdateEvent & ev)
	{
	}

	bool EditorAbout::onGui(const GuiEvent & ev)
	{
		if (!beginDraw(ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysAutoResize))
		{ 
			return endDraw(); 
		}

		auto draw_link = ([](C_String label, C_String url)
		{
			if (ImGui::MenuItem(label, url)) OS::execute("open", url);
		});

		// Menu Bar
		if (ImGui::BeginMenuBar())
		{
			draw_link("Downloads", "https://bit.ly/ml_noobs");
			draw_link("Repository", ML_PROJECT_URL);
			ImGui::EndMenuBar();
		}

		// Header
		ImGui::Text("%s", MEMELIB_VERSION);
		ImGui::Text("Special thanks to Sajid Farooq and Champlain College.");
		ImGui::Separator();

		// Config/Build Information
		if (ImGui::CollapsingHeader("Config/Build Information"))
		{
			auto draw_def = ([](C_String label, C_String fmt, auto data)
			{
				ImGui::Text("define: %s", label);
				ImGui::NextColumn();
				ImGui::Text(fmt, data);
				ImGui::NextColumn();
			});
			ImGui::Columns(2);
			draw_def("ML_ARCHITECTURE", "%u-bit", ML_ARCHITECTURE);
			draw_def("ML_CC_NAME", "%s", ML_CC_NAME);
			draw_def("ML_CC_VER", "%u", ML_CC_VER);
			draw_def("ML_CONFIGURATION", "%s", ML_CONFIGURATION);
			draw_def("ML_CPLUSPLUS", "%u", ML_CPLUSPLUS);
			draw_def("ML_HAS_CXX11", "%s", ML_HAS_CXX11 ? "true" : "false");
			draw_def("ML_HAS_CXX14", "%s", ML_HAS_CXX14 ? "true" : "false");
			draw_def("ML_HAS_CXX17", "%s", ML_HAS_CXX17 ? "true" : "false");
			draw_def("ML_PLATFORM_TARGET", "%s", ML_PLATFORM_TARGET);
			draw_def("ML_PROJECT_AUTH", "%s", ML_PROJECT_AUTH);
			draw_def("ML_PROJECT_DATE", "%s", ML_PROJECT_DATE);
			draw_def("ML_PROJECT_TIME", "%s", ML_PROJECT_TIME);
			draw_def("ML_PROJECT_URL", "%s", ML_PROJECT_URL);
			draw_def("ML_PROJECT_VER", "%s", ML_PROJECT_VER);
			draw_def("ML_SYSTEM_NAME", "%s", ML_SYSTEM_NAME);
			ImGui::NextColumn();
			ImGui::Columns(1);
		}
		
		// Third Party Software
		if (ImGui::CollapsingHeader("Third Party Software"))
		{
			draw_link("assimp", "https://github.com/assimp/assimp");
			draw_link("cpython", "https://github.com/python/cpython");
			draw_link("dirent", "https://github.com/tronkko/dirent");
			draw_link("flac", "https://github.com/xiph/flac");
			draw_link("FreeType", "https://www.freetype.org/");
			draw_link("GCEM", "https://github.com/kthohr/gcem");
			draw_link("GLEW", "http://glew.sourceforge.net/");
			draw_link("GLFW", "https://www.glfw.org/");
			draw_link("GLM", "https://glm.g-truc.net/0.9.9/index.html");
			draw_link("ImGui", "https://github.com/ocornut/imgui");
			draw_link("ImGuiColorTextEdit", "https://github.com/BalazsJako/ImGuiColorTextEdit");
			draw_link("ImGuizmo", "https://github.com/CedricGuillemet/ImGuizmo");
			draw_link("INIReader", "https://github.com/benhoyt/inih");
			draw_link("Lua", "https://github.com/lua/lua");
			draw_link("ogg", "https://github.com/xiph/ogg");
			draw_link("OpenAL", "https://www.openal.org/");
			draw_link("pdcurses", "https://github.com/wmcbrine/PDCurses");
			draw_link("pybind11", "https://github.com/pybind/pybind11");
			draw_link("RakNet", "http://www.jenkinssoftware.com/");
			draw_link("rapidjson", "https://github.com/Tencent/rapidjson");
			draw_link("stb", "https://github.com/nothings/stb");
			draw_link("vorbis", "https://github.com/xiph/vorbis");
		}

		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}