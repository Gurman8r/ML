#ifndef _ML_GUI_HPP_
#define _ML_GUI_HPP_

#include <ML/Editor/Export.hpp>
#include <ML/Core/Transform.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API GUI final
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * */

		static bool DrawLabel(CString label);

		/* * * * * * * * * * * * * * * * * * * * */

		static bool EditVec2f	(CString label, vec2 & value, float speed = 1.f);
		static bool EditVec3f	(CString label, vec3 & value, float speed = 1.f);
		static bool EditVec4f	(CString label, vec4 & value, float speed = 1.f);
		static bool EditQuat	(CString label, quat & value, float speed = 1.f);
		static bool EditMat3f	(CString label, mat3 & value, float speed = 1.f);
		static bool EditMat4f	(CString label, mat4 & value, float speed = 1.f);

		/* * * * * * * * * * * * * * * * * * * * */

		static bool BeginWindow(CString label, bool * p_open, int32_t flags);
		static void EndWindow();

		template <
			typename Fun
		> inline static bool DrawWindow(CString label, bool * p_open, int32_t flags, Fun fun)
		{
			bool good;
			if (good = BeginWindow(label, p_open, flags))
			{
				fun();
			}
			EndWindow();
			return good;
		}

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_GUI_HPP_