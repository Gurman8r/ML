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

		// Menu Bar
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::Button("Downloads")) OS::execute("open", "https://bit.ly/ml_noobs");
			if (ImGui::Button("Repository")) OS::execute("open", ML_PROJECT_URL);
			ImGui::EndMenuBar();
		}

		// Header
		ImGui::Text("Created by Melody Gurman");
		ImGui::Text(MEMELIB_VERSION);
		ImGui::Text(
			"Special thanks to Sajid Farooq and Champlain College for their help and support."
		); 
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
			link_item("GLEW", "http://glew.sourceforge.net/");
			link_item("GLFW", "https://www.glfw.org/");
			link_item("GLM", "https://glm.g-truc.net/0.9.9/index.html");
			link_item("ImGui", "https://github.com/ocornut/imgui");
			link_item("ImGuiColorTextEdit", "https://github.com/BalazsJako/ImGuiColorTextEdit");
			link_item("ImGuizmo", "https://github.com/CedricGuillemet/ImGuizmo");
			link_item("INIReader", "https://github.com/benhoyt/inih");
			link_item("Lua", "https://github.com/lua/lua");
			link_item("ogg", "https://github.com/xiph/ogg");
			link_item("OpenAL", "https://www.openal.org/");
			link_item("pdcurses", "https://github.com/wmcbrine/PDCurses");
			link_item("pybind11", "https://github.com/pybind/pybind11");
			link_item("RakNet", "http://www.jenkinssoftware.com/");
			link_item("rapidjson", "https://github.com/Tencent/rapidjson");
			link_item("stb", "https://github.com/nothings/stb");
			link_item("vorbis", "https://github.com/xiph/vorbis");
		}

		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}