#ifndef _ML_EXPLORER_HPP_
#define _ML_EXPLORER_HPP_

#include <ML/Editor/EditorGui.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API Explorer final
		: public EditorGui
	{
		friend class Editor;

	public:
		enum : char
		{
			T_Reg = ' ',
			T_Dir = '/',
			T_Lnk = '@',
			T_Unk = '*',
		};

	private:
		explicit Explorer(EventSystem & eventSystem);
		~Explorer();

	public:
		void onEvent(const Event * value) override;

	protected:
		bool drawGui(const GuiEvent & ev) override;

	private:
		String m_path;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EXPLORER_HPP_