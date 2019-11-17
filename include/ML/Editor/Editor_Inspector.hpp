#ifndef _ML_EDITOR_INSPECTOR_HPP_
#define _ML_EDITOR_INSPECTOR_HPP_

#include <ML/Editor/Editor_Base.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API Editor_Inspector final : public Editor_Base
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend class Editor;

		struct Layout;

		Editor_Inspector();

		~Editor_Inspector() {}

		void onEvent(const Event & value) override;

		bool draw() override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_INSPECTOR_HPP_