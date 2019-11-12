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

		static bool Combo(C_String label, int32_t * index, const List<String> & arr, int32_t max_height = -1);

		static bool Combo(C_String label, int32_t * index, const C_String * arr, int32_t max_height = -1);

		static bool Combo(C_String label, int32_t * index, C_String items, int32_t max_height = -1);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static int32_t Confirm(bool trigger, C_String label, bool * p_open, C_String message, int32_t popup_flags);

		static int32_t Confirm(const String & label, bool trigger, const String & message);

		static void HelpMarker(const String & message);

		static int32_t OpenFile(const String & label, String & path, const vec2 & size);

		static void Tooltip(const String & message);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct Util final
		{
<<<<<<< HEAD
			static inline bool get_vector(ptr_t<void> value, int32_t i, C_String * out)
=======
			static inline bool get_vector(void * value, int32_t i, C_String * out)
>>>>>>> f7c7043e8a25a18fb5a96df82e94d8eabbcf6a68
			{
				auto * vector{ static_cast<List<String> *>(value) };
				if (vector && (i >= 0) && (i < static_cast<int32_t>(vector->size())))
				{
					(*out) = vector->at(i).c_str();
					return true;
				}
				return false;
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_IMGUI_EXT_HPP_