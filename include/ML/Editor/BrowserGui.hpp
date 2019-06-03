#ifndef _ML_BROWSER_GUI_HPP_
#define _ML_BROWSER_GUI_HPP_

#include <ML/Editor/EditorGui.hpp>
#include <ML/Core/FileSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API BrowserGui final 
		: public EditorGui
	{
		friend class Editor;

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		enum : char
		{
			T_Reg = ' ',
			T_Dir = '/',
			T_Lnk = '@',
			T_Unk = '*',
		};

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		using Directory = typename FileSystem::Directory;

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		explicit BrowserGui(EventSystem & eventSystem);
		~BrowserGui();

	protected:
		/* * * * * * * * * * * * * * * * * * * * */
		bool drawGui(const GuiEvent & ev) override;

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		void draw_directory();
		void draw_file();
		void draw_file_preview();
		void draw_file_details();

	public:
		void	set_selected(char type, size_t index);
		String	get_selected_name() const;
		String	get_selected_path() const;
		String	get_selected_type() const;
		size_t	get_selected_size() const;

		/* * * * * * * * * * * * * * * * * * * * */

		inline String pathTo(const String & value) const 
		{ 
			return (m_path + "\\" + value);
		}

		inline const List<String> * get_list() const
		{
			Directory::const_iterator it;
			return (((it = m_dir.find(m_type)) != m_dir.end())
				? (&it->second)
				: (NULL));
		}

		inline const String * get_selected() const
		{
			const List<String> * list;
			return ((list = get_list())
				? ((m_index < list->size())
					? &(*list).at(m_index)
					: NULL)
				: NULL);
		}

		/* * * * * * * * * * * * * * * * * * * * */

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		String		m_path;		// Working Directory
		Directory	m_dir;		// Directory Contents
		char		m_type;		// Selected List
		size_t		m_index;	// Selected File
		String		m_preview;	// File Contents
		bool		m_isDouble;	// Has Double Click
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}
#endif // !_ML_BROWSER_GUI_HPP_