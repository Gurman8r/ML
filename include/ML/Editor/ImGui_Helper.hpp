#ifndef _ML_IMGUI_HELPER_HPP_
#define _ML_IMGUI_HELPER_HPP_

#include <ML/Editor/Export.hpp>
#include <ML/Core/List.hpp>

namespace ml
{
	class ML_EDITOR_API ImGui_Helper final
	{
	public:
		inline static bool vector_getter(void * vec, int32_t idx, CString * out_text)
		{
			auto & vector = (*static_cast<List<String>*>(vec));
			if ((idx >= 0) && (idx < static_cast<int32_t>(vector.size())))
			{
				(*out_text) = vector.at(idx).c_str();
				return true;
			}
			return false;
		}
	};
}

#endif // !_ML_IMGUI_HELPER_HPP_