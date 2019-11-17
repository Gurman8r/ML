#ifndef _ML_EDITOR_MANUAL_HPP_
#define _ML_EDITOR_MANUAL_HPP_

#include <ML/Editor/EditorComponent.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API Editor_Manual final : public EditorComponent
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend class Editor;

		Editor_Manual();

		~Editor_Manual() {}

		void update() override;

		bool draw() override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_ABOUT_HPP_