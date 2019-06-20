#ifndef _ML_TERMINAL_GUI_HPP_
#define _ML_TERMINAL_GUI_HPP_

#include <ML/Editor/EditorGui.hpp>
#include <ML/Core/List.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API TerminalGui final
		: public EditorGui
	{
		friend class Editor;

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		enum : size_t { BufferSize = 256 };

		using InputBuffer = typename char[256];

		using History = typename List<char *>;

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		explicit TerminalGui(EventSystem & eventSystem);
		~TerminalGui();

	protected:
		/* * * * * * * * * * * * * * * * * * * * */
		bool drawGui(const GuiEvent & ev) override;

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		void    clear();
		void    execute(CString value);
		void    printf(CString value, ...);		// Print Format
		void	printl(const String & value);	// Print Line
		void	printss(SStream & value);		// Print Stream
		bool	redirect(ostream & value);
		int32_t inputCallback(void * value);

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		inline const History & history() const { return m_history; }

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		const ostream *	m_coutPtr; // cout redirect reference
		streambuf *		m_coutBuf; // cout redirect buffer
		SStream			m_coutStr; // cout redirect stream

		InputBuffer		m_inputBuf;
		List<String>	m_lines;
		bool			m_scrollBottom;
		History			m_history;
		int32_t			m_historyPos;
		List<CString>	m_autoFill;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TERMINAL_GUI_HPP_