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
		static vec2 getWindowPos();
		static vec2 getWindowSize();

	public:
		static bool BeginWindow(CString name, bool * p_open, const int32_t flags = 0);
		static void EndWindow();

	public:
		template <
			class Fun, class ... Args
		> inline static void DrawWindow(
			CString name, bool * p_open, int32_t flags, Fun && fun, Args && ... args)
		{	
			if (BeginWindow(name, p_open, flags))
			{
				fun(std::forward<Args>(args)...);
			}
			EndWindow();
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_UTILITY_HPP_