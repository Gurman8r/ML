#include <ML/Script/Script.hpp>
#include <ML/Script/Interpreter.hpp>
#include <ML/Script/ScriptMacros.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Script::Script()
		: m_file()
		, m_toks()
		, m_root(nullptr)
		, m_retv()
	{
	}

	Script::~Script()
	{
		dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Script::dispose()
	{
		if (m_root)
		{ 
			delete m_root;
			m_root = nullptr;
		}
		return (!m_root);
	}

	bool Script::loadFromFile(const String & filename)
	{
		return m_file.loadFromFile((m_path = filename));
	}

	bool Script::loadFromMemory(const File & file)
	{
		if (file)
		{
			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Script::build(const Arguments & args)
	{
		if (m_file)
		{
			// Generate Tokens
			m_toks = ML_Lexer.genTokenList(m_file.data());

			// Generate Tree
			if (m_root = ML_Parser.genFromList(m_toks))
			{
				// __ARGS__
				m_root->push_front(new AST_Assign(
					Operator::OP_SET,
					new AST_Name(ML_SCR_NAME_ARGS),
					ML_Parser.generate<AST_Array>(ML_Lexer.genArgsArray(args)))
				);

				// __FILE__
				m_root->push_front(new AST_Assign(
					Operator::OP_SET,
					new AST_Name(ML_SCR_NAME_FILE),
					new AST_String(m_path))
				);

				// __PATH__
				m_root->push_front(new AST_Assign(
					Operator::OP_SET,
					new AST_Name(ML_SCR_NAME_PATH),
					new AST_String(ML_FS.getPathTo("")))
				);

				return true;
			}
		}
		m_root = nullptr;
		return false;
	}

	bool Script::buildAndRun(const Arguments & args)
	{
		return build(args) && run();
	}

	bool Script::rebuild(const Arguments & args)
	{
		return loadFromFile(m_path) && build(args);
	}

	bool Script::run()
	{
		if (m_root)
		{
			if (m_root->run())
			{
				m_retv = m_root->getRet();

				return dispose();
			}
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}