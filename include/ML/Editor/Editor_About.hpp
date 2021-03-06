#ifndef _ML_EDITOR_ABOUT_HPP_
#define _ML_EDITOR_ABOUT_HPP_

#include <ML/Editor/Editor_Widget.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API Editor_About final : public Editor_Widget
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend class Editor;

		Editor_About();

		~Editor_About() {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void onEvent(Event const & value) override;

		bool draw() override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_ABOUT_HPP_