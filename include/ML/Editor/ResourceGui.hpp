#ifndef _ML_RESOURCE_GUI_HPP_
#define _ML_RESOURCE_GUI_HPP_

#include <ML/Editor/EditorGui.hpp>
#include <ML/Engine/Content.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API ResourceGui final 
		: public EditorGui
	{
		friend class Editor;

	private:
		explicit ResourceGui(EventSystem & eventSystem);
		~ResourceGui();

		bool drawGui(const GuiEvent & ev) override;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RESOURCE_GUI_HPP_