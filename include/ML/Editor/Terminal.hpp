#ifndef _ML_TERMINAL_HPP_
#define _ML_TERMINAL_HPP_

#include <ML/Editor/EditorWindow.hpp>
#include <ML/Core/List.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API Terminal final
		: public EditorWindow
	{
		friend class Editor;

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		enum : size_t { BufferSize = 256 };

		using InputBuffer = typename char[256];

		using History = typename List<char *>;

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		explicit Terminal(EventSystem & eventSystem, bool startOpen);
		~Terminal();

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
		int32_t inputCallback(void * value);

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		inline const History & history() const { return m_history; }

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		InputBuffer		m_inputBuf;
		List<String>	m_lines;
		bool			m_scrollBottom;
		History			m_history;
		int32_t			m_historyPos;
		List<CString>	m_autoFill;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TERMINAL_HPP_