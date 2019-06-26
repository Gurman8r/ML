#ifndef _ML_SCRIPT_HPP_
#define _ML_SCRIPT_HPP_

#include <ML/Script/AST_Expr.hpp>
#include <ML/Core/File.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_SCRIPT_API Script
		: public I_Newable
		, public I_Disposable
		, public I_Readable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Script();
		~Script();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;
		bool loadFromFile(const String & filename) override;
		bool loadFromMemory(const File & file);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool	build(const Arguments & args);
		bool	buildAndRun(const Arguments & args);
		bool	rebuild(const Arguments & args);
		bool	run();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto file() const -> const File &		{ return m_file; }
		inline auto path() const -> const String &		{ return m_path; }
		inline auto root() const -> const AST_Block	*	{ return m_root; }
		inline auto retv() const -> const Var &			{ return m_retv; }
		inline auto toks() const -> const TokenList	&	{ return m_toks; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		File		m_file;
		String		m_path;
		Var			m_retv;
		AST_Block *	m_root;
		TokenList	m_toks;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SCRIPT_HPP_