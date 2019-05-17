#include <ML/Editor/ImGui_Builtin.hpp>
#include <ML/Editor/ImGui.hpp>

namespace ml
{
	void ImGui_Builtin::showDemo(bool * p_open)
	{
		ImGui::ShowDemoWindow(p_open);
	}
	
	void ImGui_Builtin::showMetrics(bool * p_open)
	{
		ImGui::ShowMetricsWindow(p_open);
	}
	
	void ImGui_Builtin::showStyle(bool * p_open)
	{
		ImGui::Begin("ImGui Style Editor", p_open);
		ImGui::ShowStyleEditor();
		ImGui::End();
	}
	
	void ImGui_Builtin::showAbout(bool * p_open)
	{
		ImGui::ShowAboutWindow(p_open);
	}
}