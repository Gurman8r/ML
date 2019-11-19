#ifndef _ML_EDITOR_TERMINAL_HPP_
#define _ML_EDITOR_TERMINAL_HPP_

#include <ML/Editor/Editor_Base.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API Editor_Terminal final : public Editor_Base
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend class Editor;

		Editor_Terminal();

		void onEvent(Event const & value) override;

		bool draw() override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:
		void clear();
		
		void execute(C_String value);
		
		void printf(C_String value, ...);
		
		void printl(String const & value);
		
		void printss(SStream & value);
		
		int32_t inputCallback(void * value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto history() const -> const List<char *> & { return m_history; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		std::ostream const *m_coutPtr; // stream redirect reference
		std::streambuf *	m_coutBuf; // stream redirect buffer
		SStream				m_coutStr; // stream redirect stream
		Array<char, 256>	m_inputBuf;
		List<String>		m_lines;
		bool				m_scrollToBot;
		List<char *>		m_history;
		int32_t				m_historyPos;
		List<C_String>		m_autoFill;
		bool				m_paused;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_TERMINAL_HPP_