#include <ML/Script/AST_Block.hpp>
#include <ML/Script/AST_Expr.hpp>
#include <ML/Script/Interpreter.hpp>
#include <ML/Script/ScriptMacros.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	AST_Block::AST_Block()
		: AST_Node()
	{
		ML_Runtime.makeScope(getID());
	}

	AST_Block::AST_Block(const List<AST_Node *> & body)
		: AST_Node(body)
	{
		ML_Runtime.makeScope(getID());
	}

	AST_Block::~AST_Block()
	{
		ML_Runtime.clearScope(getID());
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Ostream & AST_Block::display(Ostream & out) const
	{
		auto tab = [](size_t n)
		{
			return (n == 0)
				? String()
				: String(n, '\t');
		};

		out << "{\n";

		for (AST_Node* n : (*this))
		{
			out << tab(n->getDepth()) << (*n) 
				<< endl << FMT();
		}

		out << tab(getDepth()) << "}";

		return out;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool	AST_Block::delVar(const String & name)
	{
		if (ML_Runtime.delVar(getID(), name))
		{
			return true;
		}
		else if (AST_Block* b = block())
		{
			return b->delVar(name);
		}
		else
		{
			return false;
		}
	}

	Var *	AST_Block::getVar(const String & name) const
	{
		if (Var * v = ML_Runtime.getVar(getID(), name))
		{
			return v;
		}
		else if (AST_Block* b = block())
		{
			return b->getVar(name);
		}
		else
		{
			return nullptr;
		}
	}

	Var *	AST_Block::newVar(const String & name, const Var & value)
	{
		if (Var * v = ML_Runtime.newVar(getID(), name, value))
		{
			return v;
		}
		else if (AST_Block* b = block())
		{
			return b->newVar(name, value);
		}
		else
		{
			return nullptr;
		}
	}

	Var *	AST_Block::setVar(const String & name, const Var & value)
	{
		if (Var * v = getVar(name))
		{
			(*v) = value;
			return v;
		}
		else if (Var * v = ML_Runtime.setVar(getID(), name, value))
		{
			return v;
		}
		else if (AST_Block* b = block())
		{
			return b->setVar(name, value);
		}
		else
		{
			return nullptr;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Var *	AST_Block::getRet() const
	{
		if (Var * v = ML_Runtime.getVar(getID(), ML_SCR_NAME_RETV))
		{
			return v;
		}
		return setRet(Var().voidValue());
	}

	Var *	AST_Block::setRet(const Var & value) const
	{
		return ML_Runtime.setVar(getID(), ML_SCR_NAME_RETV, value);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool AST_Block::addFunc(const String & name, AST_Func * value)
	{
		if (!getFunc(name))
		{
			if (Var * v = setVar(name, Var().funcValue({ { Token::Name, name } })))
			{
				m_funcs.insert({ name, value });

				return true;
			}
		}
		return false;
	}

	AST_Func * AST_Block::getFunc(const String & name)
	{
		if (AST_Block * b = block())
		{
			if (AST_Func * f = b->getFunc(name))
			{
				return f;
			}
		}

		FuncTable::iterator it;
		return ((it = m_funcs.find(name)) != m_funcs.end())
			? it->second
			: nullptr;
	}

	AST_Block::FuncTable AST_Block::getFuncs() const
	{
		return m_funcs;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool AST_Block::addStruct(const String & name, AST_Struct * value)
	{
		if (!getStruct(name))
		{
			if (Var * v = setVar(name, Var().structValue({ { Token::Name, name } })))
			{
				m_structs.insert({ name, value });

				return true;
			}
		}
		return false;
	}

	AST_Struct * AST_Block::getStruct(const String & name)
	{
		if (AST_Block * b = block())
		{
			if (AST_Struct * s = b->getStruct(name))
			{
				return s;
			}
		}

		StructTable::iterator it;
		return ((it = m_structs.find(name)) != m_structs.end())
			? it->second
			: nullptr;
	}

	AST_Block::StructTable AST_Block::getStructs() const
	{
		return m_structs;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	AST_Block *	AST_Block::block() const
	{
		AST_Node* p = getParent();
		while (p)
		{
			if (AST_Block* b = p->as<AST_Block>())
			{
				return b;
			}
			p = p->getParent();
		}
		return nullptr;
	}

	Var	 AST_Block::evaluate() const
	{
		return (*getRet());
	}
		 
	bool AST_Block::run()
	{
		// dont run block
		if (prevAs<AST_If>() ||
			prevAs<AST_Elif>() ||
			prevAs<AST_Else>() ||
			prevAs<AST_For>() ||
			prevAs<AST_While>() ||
			prevAs<AST_Func>() ||
			prevAs<AST_Struct>())
		{
			return runNext();
		}
		return runFirst();
	}
		 
	bool AST_Block::runFirst()
	{
		if (AST_Node* n = getChild(0))
		{
			return n->run();
		}
		return Debug::logError("Block is empty");
	}

	/* * * * * * * * * * * * * * * * * * * * */
}