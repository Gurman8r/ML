#ifndef _ML_IMGUI_BUILTIN_HPP_
#define _ML_IMGUI_BUILTIN_HPP_

#include <ML/Editor/Export.hpp>

namespace ml
{
	// ImGui Demo Windows
	class ML_EDITOR_API ImGui_Builtin final
	{
	public:
		static void showDemo(bool * p_open);
		static void showMetrics(bool * p_open);
		static void showStyle(bool * p_open);
		static void showAbout(bool * p_open);
	};
}

#endif // !_ML_IMGUI_BUILTIN_HPP_