#ifndef _ML_EDITOR_ABOUT_HPP_
#define _ML_EDITOR_ABOUT_HPP_

#include <ML/Editor/EditorComponent.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API Editor_About final : public EditorComponent
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend class Editor;

		Editor_About();

		~Editor_About() {}

		void update() override;

		bool draw() override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_ABOUT_HPP_