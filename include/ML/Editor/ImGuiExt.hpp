#ifndef _ML_IMGUI_EXT_HPP_
#define _ML_IMGUI_EXT_HPP_

#include <ML/Editor/Export.hpp>
#include <ML/Core/String.hpp>
#include <ML/Core/List.hpp>
#include <ML/Core/I_Singleton.hpp>

namespace ml
{
	struct ML_EDITOR_API ImGuiExt final : public I_Singleton<ImGuiExt>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool Combo(C_String label, int32_t * index, const List<String> & arr);

		static bool Combo(C_String label, int32_t * index, const C_String * arr, int32_t size);

		template <
			size_t N
		> inline static bool Combo(C_String label, int32_t * index, const C_String(&arr)[N])
		{
			return Combo(label, index, arr, ML_ARRAYSIZE(arr));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void HelpMarker(const String & message);

		static void Tooltip(const String & message);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline bool vector_getter(void * value, int32_t index, C_String * out)
		{
			if (auto * vector = static_cast<List<String> *>(value))
			{
				if ((index >= 0) && (index < static_cast<int32_t>(vector->size())))
				{
					(*out) = vector->at(index).c_str();
					return true;
				}
			}
			return false;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: friend struct I_Singleton<ImGuiExt>;
	};
}

#endif // !_ML_IMGUI_EXT_HPP_