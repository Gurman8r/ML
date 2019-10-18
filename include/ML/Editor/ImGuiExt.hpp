#ifndef _ML_IMGUI_EXT_HPP_
#define _ML_IMGUI_EXT_HPP_

#include <ML/Editor/Export.hpp>
#include <ML/Core/StringUtility.hpp>
#include <ML/Core/Matrix.hpp>
#include <ML/Core/Singleton.hpp>

namespace ml
{
	struct ML_EDITOR_API ImGuiExt final
	{
		ImGuiExt() = delete;
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static vec2 GetContentRegionAvail();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool Combo(C_String label, int32_t * index, const List<String> & arr, int32_t max_height = -1);

		static bool Combo(C_String label, int32_t * index, const C_String * arr, int32_t max_height = -1);

		static bool Combo(C_String label, int32_t * index, C_String items, int32_t max_height = -1);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static int32_t Confirm(const String & label, bool trigger, const String & message);

		static void HelpMarker(const String & message);

		static bool OpenFile(const String & label, String & path, const vec2 & size);

		static void Tooltip(const String & message);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline static bool IsHidden(const String & value)
		{
			return ((value.size() >= 2) && (value.substr(0, 2) == "##"));
		}

		static inline bool vector_getter(void * vec, int32_t index, C_String * out)
		{
			if (auto * vector { static_cast<List<String> *>(vec) })
			{
				if ((index >= 0) && (index < static_cast<int32_t>(vector->size())))
				{
					(*out) = vector->at(index).c_str();
					return true;
				}
			}
			return false;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_IMGUI_EXT_HPP_