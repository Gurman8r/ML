#ifndef _ML_TERMINAL_HPP_
#define _ML_TERMINAL_HPP_

#include <ML/Editor/BaseWidget.hpp>
#include <ML/Core/List.hpp>

namespace ml
{
	// Command Line Interface
	class ML_EDITOR_API Terminal final
		: public BaseWidget
	{
		friend class Editor;

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		enum : size_t { BufferSize = 256 };

		using InputBuffer = typename char[256];

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		Terminal(Editor & editor, bool open);
		~Terminal();

	protected:
		/* * * * * * * * * * * * * * * * * * * * */
		bool drawGui(const DrawGuiEvent & ev) override;

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		void    clear();
		void    printf(CString value, ...);		// Print Format
		void	printl(const String & value);	// Print Line
		void	printss(SStream & value);		// Print Stream
		void    execute(CString value);
		int32_t inputCallback(void * value);

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		inline const List<char *> & history() const { return m_history; }

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		InputBuffer		m_inputBuf;
		List<String>	m_lines;
		bool			m_scrollBottom;
		List<char *>	m_history;
		int32_t			m_historyPos;
		List<CString>	m_autoFill;
	};

}

#endif // !_ML_TERMINAL_HPP_