#ifndef _ML_TERMINAL_GUI_HPP_
#define _ML_TERMINAL_GUI_HPP_

#include <ML/Editor/EditorGui.hpp>
#include <ML/Core/List.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API Terminal final
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
		explicit Terminal(EventSystem & eventSystem);
		~Terminal();

	protected:
		/* * * * * * * * * * * * * * * * * * * * */
		bool drawGui(const GuiEvent & ev) override;

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		void    clear();
		void    execute(C_String value);
		void    printf(C_String value, ...);	// Print Format
		void	printl(const String & value);	// Print Line
		void	printss(SStream & value);		// Print SStream
		bool	redirect(Ostream & value);
		int32_t inputCallback(void * value);

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		inline const History & history() const { return m_history; }

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		const Ostream *	m_coutPtr; // cout redirect reference
		StreamBuf *		m_coutBuf; // cout redirect buffer
		SStream			m_coutStr; // cout redirect stream

		InputBuffer		m_inputBuf;
		List<String>	m_lines;
		bool			m_scrollBottom;
		History			m_history;
		int32_t			m_historyPos;
		List<C_String>	m_autoFill;
		bool			m_paused;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TERMINAL_GUI_HPP_