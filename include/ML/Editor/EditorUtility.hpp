#ifndef _ML_EDITOR_UTILITY_HPP_
#define _ML_EDITOR_UTILITY_HPP_

#include <ML/Editor/Export.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Core/Rect.hpp>
#include <ML/Core/String.hpp>
#include <ML/Core/List.hpp>
#include <ML/Core/I_Singleton.hpp>

#define ML_EditorUtility _ML EditorUtility::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_EDITOR_API EditorUtility final
		: public I_Singleton<EditorUtility>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend class I_Singleton<EditorUtility>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static vec2 getCursorPos();
		static vec2 getCursorScreenPos();
		static vec2 getMousePos();
		static vec2 getWindowPos();
		static vec2 getWindowSize();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void HelpMarker(const String & desc);
		static bool StringCombo(C_String label, int32_t & index, const List<String> & keys);
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline bool vector_getter(void * vec, int32_t idx, C_String * out_text)
		{
			auto & vector = (*static_cast<List<String> *>(vec));
			if ((idx >= 0) && (idx < static_cast<int32_t>(vector.size())))
			{
				(*out_text) = vector.at(idx).c_str();
				return true;
			}
			return false;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_UTILITY_HPP_