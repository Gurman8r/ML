#ifndef _ML_EDITOR_UTILITY_HPP_
#define _ML_EDITOR_UTILITY_HPP_

#include <ML/Editor/EditorEvents.hpp>
#include <ML/Core/Rect.hpp>
#include <ML/Core/String.hpp>
#include <ML/Core/List.hpp>
#include <ML/Core/I_Singleton.hpp>

#define ML_EditorUtility _ML EditorUtility::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API EditorUtility final
		: public I_Singleton<EditorUtility>
	{
		friend struct I_Singleton<EditorUtility>;

	public:
		static vec2 getCursorPos();
		static vec2 getCursorScreenPos();
		static vec2 getMousePos();
		static vec2 getWindowPos();
		static vec2 getWindowSize();

	public:
		static void HelpMarker(const String & desc);
		static bool StringCombo(C_String label, int32_t & index, const List<String> & keys);
		static bool BeginWindow(C_String name, bool & open, const int32_t flags = 0);
		static void EndWindow();

	public:
		template <
			class Fun, class ... Args
		> inline static void DrawWindow(
			C_String name, bool & open, int32_t flags, Fun && fun, Args && ... args)
		{	
			if (open)
			{
				if (BeginWindow(name, open, flags))
				{
					fun(std::forward<Args>(args)...);
				}
				EndWindow();
			}
		}

	public:
		inline static bool vector_getter(void * vec, int32_t idx, C_String * out_text)
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

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_UTILITY_HPP_