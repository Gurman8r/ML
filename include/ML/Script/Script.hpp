#ifndef _ML_SCRIPT_HPP_
#define _ML_SCRIPT_HPP_

#include <ML/Script/AST_Expr.hpp>
#include <ML/Core/File.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_SCRIPT_API Script
		: public ITrackable
		, public IDisposable
		, public IReadable
	{
	public:
		Script();
		~Script();

	public:
		bool dispose() override;
		bool loadFromFile(const String & filename) override;
		bool loadFromMemory(const File & file);

	public:
		bool	build(const Args & args);
		bool	buildAndRun(const Args & args);
		bool	rebuild(const Args & args);
		bool	run();

	public:
		inline const File		& file() const { return m_file; }
		inline const String		& path() const { return m_path; }
		inline const AST_Block	* root() const { return m_root; }
		inline const Var		& retv() const { return m_retv;	}
		inline const TokenList	& toks() const { return m_toks; }

	private:
		File		m_file;
		String		m_path;
		Var			m_retv;
		AST_Block *	m_root;
		TokenList	m_toks;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SCRIPT_HPP_