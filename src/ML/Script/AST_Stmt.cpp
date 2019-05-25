#include <ML/Script/AST_Stmt.hpp>
#include <ML/Script/AST_Expr.hpp>
#include <ML/Script/Interpreter.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>

namespace ml
{
	// Statement
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Stmt::AST_Stmt(int32_t type)
		: AST_Node()
		, stmtType(type)
	{
	}

	std::ostream & AST_Stmt::display(std::ostream & out) const
	{
		return AST_Node::display(out);
	}

	AST_Block * AST_Stmt::block() const
	{
		AST_Node* p = getParent();
		while (p)
		{
			if (AST_Block * b = p->as<AST_Block>())
			{
				return b;
			}
			p = p->getParent();
		}
		return NULL;
	}


	// Elif
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Elif::AST_Elif(AST_Expr * expr)
		: AST_Stmt(ST_Elif)
		, expr(expr)
	{
		addChild(expr);
	}

	std::ostream & AST_Elif::display(std::ostream & out) const
	{
		return out << FG::Blue << "elif" << FMT() << "(" << (*expr) << FMT() << ")";
	}

	bool AST_Elif::run()
	{
		return runNext();
	}


	// Else
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Else::AST_Else()
		: AST_Stmt(ST_Else)
	{
	}

	std::ostream & AST_Else::display(std::ostream & out) const
	{
		return out << FG::Blue << "else" << FMT();
	}

	bool AST_Else::run()
	{
		return runNext();
	}


	// For
	/* * * * * * * * * * * * * * * * * * * * */
	AST_For::AST_For(AST_Expr * assn, AST_Expr * expr, AST_Expr * stmt)
		: AST_Stmt(ST_For)
		, assn(assn)
		, expr(expr)
		, stmt(stmt)
	{
		addChild(assn);
		addChild(expr);
		addChild(stmt);
	}

	std::ostream & AST_For::display(std::ostream & out) const
	{
		return out << FG::Blue << "for" << FMT() << "(" << (*assn) << "; " << (*expr) << "; " << (*stmt) << ")";
	}

	bool AST_For::run()
	{
		if (AST_Block * blck = nextAs<AST_Block>())
		{
			if (assn->run())
			{
				size_t count = 0;

				while (expr->evaluate().boolValue())
				{
					if (blck->runFirst())
					{
						if (!stmt->run())
						{
							return Debug::logError("AST_For : Statement Failure");
						}
					}
					else
					{
						return Debug::logError("AST_For : Block Failure");
					}
				}

				return blck->runNext();
			}
			else
			{
				return Debug::logError("For: Assignment Failure");
			}
		}
		return Debug::logError("For Failure");
	}


	// Free
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Delete::AST_Delete(AST_Name* name)
		: AST_Stmt(ST_Delete)
		, name(name)
	{
		addChild(name);
	}

	std::ostream & AST_Delete::display(std::ostream & out) const
	{
		return out << FG::Blue << "delete" << FMT() << "(" << (*name) << FMT() << ")";
	}

	bool AST_Delete::run()
	{
		if (block()->delVar(name->value))
		{
			return runNext();
		}
		return Debug::logError("AST_Delete : Failure {0}", name->value);
	}


	// If
	/* * * * * * * * * * * * * * * * * * * * */
	AST_If::AST_If(AST_Expr * expr)
		: AST_Stmt(ST_If)
		, expr(expr)
	{
		addChild(expr);
	}

	std::ostream & AST_If::display(std::ostream & out) const
	{
		return out << FG::Blue << "if" << FMT() << "(" << (*expr) << FMT() << ")";
	}

	bool AST_If::run()
	{
		if (AST_Block * ifBlock = nextAs<AST_Block>())
		{
			if (expr->evaluate().boolValue())
			{
				if (ifBlock->runFirst())
				{
					return ifBlock->runNext();
				}
				else
				{
					return Debug::logError("AST_If : Run Body Failed");
				}
			}

			AST_Node* next = ifBlock->getNext();

			while (AST_Elif* ei = next->as<AST_Elif>())
			{
				if (AST_Block * eiBlock = ei->nextAs<AST_Block>())
				{
					if (ei->expr->evaluate().boolValue())
					{
						if (eiBlock->runFirst())
						{
							return eiBlock->runNext();
						}
						else
						{
							return Debug::logError("AST_Elif : Run Body Failed");
						}
					}
					else
					{
						next = eiBlock->getNext();
					}
				}
				else
				{
					return Debug::logError("AST_Elif : Body Not Found");
				}
			}

			if (AST_Else * el = next->as<AST_Else>())
			{
				if (AST_Block * elBlock = el->nextAs<AST_Block>())
				{
					if (elBlock->runFirst())
					{
						return elBlock->runNext();
					}
					else
					{
						return Debug::logError("AST_Else : Run Body Failed");
					}
				}
				else
				{
					return Debug::logError("AST_Else : Body Not Found");
				}
			}

			return runNext();
		}
		else
		{
			return Debug::logError("AST_If : Body Not Found");
		}
	}


	// Include
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Include::AST_Include(AST_Expr * expr)
		: AST_Stmt(ST_Include)
		, expr(expr)
	{
		addChild(expr);
	}

	std::ostream & AST_Include::display(std::ostream & out) const
	{
		return out << FG::Blue << "include" << FMT() << "(" << (*expr) << FMT() << ")";
	}

	bool AST_Include::run()
	{
		const String & filename = expr->evaluate().stringValue();

		static File file;
		if (file.loadFromFile(ML_FS.getPathTo(filename)))
		{
			if (AST_Block * root = ML_Parser.genFromList(ML_Lexer.genTokenList(file.data())))
			{
				if (!root->empty())
				{
					for (auto it = root->rbegin(); it != root->rend(); it++)
					{
						if (!block()->push_front(*it))
						{
							Debug::logError("AST_Include : Failed loading node");
						}
					}
				}
				else
				{
					Debug::logWarning("AST_Include : Nothing to load");
				}

				root->clearChildren();
				delete root;
			}
			return runNext();
		}
		return Debug::logError("AST_Include : File Not Found");
	}


	// Print
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Print::AST_Print(AST_Expr * expr, bool newl)
		: AST_Stmt(ST_Print)
		, expr(expr)
		, newl(newl)
	{
		addChild(expr);
	}

	std::ostream & AST_Print::display(std::ostream & out) const
	{
		return out << FG::Blue << (newl ? "printl" : "print") << FMT() << "(" << (*expr) << FMT() << ")";
	}

	bool AST_Print::run()
	{
		if (AST_String * str = expr->as<AST_String>())
		{
			String::const_iterator it;
			for (it = str->value.begin(); it != str->value.end(); it++)
			{
				if (*it == '{')
				{
					String name;
					while (++it != str->value.end())
					{
						if (*it == '}')
						{
							break;
						}
						name += (*it);
					}

					if (Var * v = block()->getVar(name))
					{
						cout << (*v).textValue();
					}
					else
					{
						cout << name;
					}
				}
				else
				{
					cout << (*it);
				}
			}
		}
		else
		{
			cout << expr->evaluate().textValue();
		}
		
		cout << FMT();
		
		if(newl) { cout << ml::endl; }
		
		return runNext();
	}


	// Return
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Return::AST_Return(AST_Expr * expr)
		: AST_Stmt(ST_Return)
		, expr(expr)
	{
		addChild(expr);
	}

	std::ostream & AST_Return::display(std::ostream & out) const
	{
		return out << FG::Blue << "return" << FMT() << "(" << (*expr) << FMT() << ")";
	}

	bool AST_Return::run()
	{
		if (block()->setRet(expr->evaluate()))
		{
			return true; // FIXME: Only halts containing block
		}
		return Debug::logError("AST_Return : Failed Setting Value");
	}


	// While
	/* * * * * * * * * * * * * * * * * * * * */
	AST_While::AST_While(AST_Expr * expr)
		: AST_Stmt(ST_While)
		, expr(expr)
	{
		addChild(expr);
	}

	std::ostream & AST_While::display(std::ostream & out) const
	{
		return out << FG::Blue << "while" << FMT() << "(" << (*expr) << FMT() << ")";
	}

	bool AST_While::run()
	{
		if (expr)
		{
			if (AST_Block * blck = nextAs<AST_Block>())
			{
				while (expr->evaluate().boolValue())
				{
					if (!blck->runFirst())
					{
						return Debug::logError("AST_While : Failed Running Block");
					}
				}
				return blck->runNext();
			}
			return Debug::logError("AST_While : No Block");
		}
		return Debug::logError("AST_While : No Expression");
	}
	
}