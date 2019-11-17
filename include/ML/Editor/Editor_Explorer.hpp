#ifndef _ML_EDITOR_EXPLORER_HPP_
#define _ML_EDITOR_EXPLORER_HPP_

#include <ML/Editor/Editor_Base.hpp>
#include <ML/Editor/FileBrowser.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API Editor_Explorer final : public Editor_Base
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend class Editor;

		Editor_Explorer();

		~Editor_Explorer() {}

		void onEvent(const Event & value) override;

		bool draw() override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		FileBrowser m_browser;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_EXPLORER_HPP_