#ifndef _ML_FILE_BROWSER_HPP_
#define _ML_FILE_BROWSER_HPP_

#include <ML/Editor/Export.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/Matrix.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_EDITOR_API FileBrowser final
		: public Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : char
		{
			T_Reg = ' ',
			T_Dir = '/',
			T_Lnk = '@',
			T_Unk = '*',
		};

		using Directory = typename FileSystem::Directory;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		FileBrowser();
		~FileBrowser();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void update();
		void render();
		void render(String const & label, vec2 const & size = { 0 }, bool border = true, int32_t flags = 0);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void 	set_selected(char type, size_t index);
		String	get_selected_name() const;
		String	get_selected_path() const;
		String	get_selected_type() const;
		size_t	get_selected_size() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline String pathTo(String const & value) const
		{ 
			return (m_path + ML_PATH_DELIM + value);
		}

		inline std::vector<String> const * get_list() const
		{
			auto it { m_dir.find(m_type) };
			return ((it != m_dir.end()) ? &it->second : nullptr);
		}

		inline String const * get_selected() const
		{
			std::vector<String> const * list { get_list() };
			return ((list && (m_index < list->size())) ? (&(*list).at(m_index)) : nullptr);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		String		m_label;		// Label of Widget
		String		m_path;			// Working Directory
		Directory	m_dir;			// Directory Contents
		char		m_type;			// Selected List
		size_t		m_index;		// Selected File
		String		m_preview;		// File Contents
		bool		m_doubleClick;	// Has Double Click

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FILE_BROWSER_HPP_