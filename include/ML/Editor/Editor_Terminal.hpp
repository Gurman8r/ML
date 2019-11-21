#ifndef _ML_EDITOR_TERMINAL_HPP_
#define _ML_EDITOR_TERMINAL_HPP_

#include <ML/Editor/Editor_Widget.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API Editor_Terminal final : public Editor_Widget
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend class Editor;

		Editor_Terminal();

		~Editor_Terminal() {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void onEvent(Event const & value) override;

		bool draw() override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:
		Editor_Terminal & clear();
		
		Editor_Terminal & execute(C_String value);
		
		Editor_Terminal & printf(C_String value, ...);
		
		Editor_Terminal & printl(String const & value);
		
		Editor_Terminal & printss(SStream & value);
		
		int32_t inputCallback(void * value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto history() const -> ArrayList<char *> const & { return m_history; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		std::ostream const *m_coutPtr; // stream redirect reference
		std::streambuf *	m_coutBuf; // stream redirect buffer
		SStream				m_coutStr; // stream redirect stream
		Array<char, 256>	m_inputBuf;
		ArrayList<String>		m_lines;
		bool				m_scrollToBot;
		ArrayList<char *>		m_history;
		int32_t				m_historyPos;
		ArrayList<C_String>		m_autoFill;
		bool				m_paused;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_TERMINAL_HPP_