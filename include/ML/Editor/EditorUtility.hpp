#ifndef _ML_EDITOR_UTILITY_HPP_
#define _ML_EDITOR_UTILITY_HPP_

#include <ML/Editor/EditorEvents.hpp>
#include <ML/Core/Transform.hpp>

#define ML_EditorUtility ml::EditorUtility::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API EditorUtility final
		: public ISingleton<EditorUtility>
	{
		friend class ISingleton<EditorUtility>;

	public:
		static vec2 getCursorPos();
		static vec2 getWindowPos();
		static vec2 getWindowSize();

	public:
		static void HelpMarker(CString desc);
		static bool BeginWindow(CString name, bool & open, const int32_t flags = 0);
		static void EndWindow();

	public:
		template <
			class Fun, class ... Args
		> inline static void DrawWindow(
			CString name, bool & open, int32_t flags, Fun && fun, Args && ... args)
		{	
			if (open)
			{
				if (BeginWindow(name, open, flags))
				{
					fun((args)...);
				}
				EndWindow();
			}
		}

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

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_UTILITY_HPP_