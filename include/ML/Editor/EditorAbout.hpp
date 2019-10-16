#ifndef _ML_EDITOR_ABOUT_HPP_
#define _ML_EDITOR_ABOUT_HPP_

#include <ML/Editor/EditorWindow.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API EditorAbout final : public EditorWindow
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend class Editor;

		EditorAbout();

		~EditorAbout() {}

		void update() override;

		bool draw() override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_ABOUT_HPP_