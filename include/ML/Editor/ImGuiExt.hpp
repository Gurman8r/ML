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
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ImGuiExt() = delete;

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum PopupState { Canceled = -1, None = 0, Submitted = 1 };

		static inline int32_t GetState(bool submit, bool cancel)
		{
			return (submit 
				? PopupState::Submitted 
				: (cancel ? PopupState::Canceled : PopupState::None)
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static vec2 GetContentRegionAvail();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool Combo(C_String label, int32_t * index, std::vector<String> const & arr, int32_t max_height = -1);

		static bool Combo(C_String label, int32_t * index, C_String const * arr, int32_t max_height = -1);

		static bool Combo(C_String label, int32_t * index, C_String items, int32_t max_height = -1);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static int32_t Confirm(bool trigger, C_String label, bool * p_open, C_String message, C_String msgYes, C_String msgNo, int32_t flags);

		static int32_t Confirm(bool trigger, C_String label, bool * p_open, C_String message, int32_t popup_flags);

		static int32_t Confirm(String const & label, bool trigger, String const & message);

		static void HelpMarker(String const & message);

		static int32_t OpenFile(String const & label, String & path, vec2 const & size);

		static void Tooltip(String const & message);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct Util final
		{
			static inline bool get_vector(void * value, int32_t i, C_String * out)
			{
				auto * vector{ static_cast<std::vector<String> *>(value) };
				if (vector && (i >= 0) && (i < static_cast<int32_t>(vector->size())))
				{
					(*out) = vector->at(i).c_str();
					return true;
				}
				return false;
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool DragMat2(C_String label, mat2 & v, float_t v_speed = 1.f, float_t v_min = 0.f, float_t v_max = 0.f, C_String format = "%.3f");
		static bool DragMat3(C_String label, mat3 & v, float_t v_speed = 1.f, float_t v_min = 0.f, float_t v_max = 0.f, C_String format = "%.3f");
		static bool DragMat4(C_String label, mat4 & v, float_t v_speed = 1.f, float_t v_min = 0.f, float_t v_max = 0.f, C_String format = "%.3f");

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_IMGUI_EXT_HPP_