#include <ML/Script/Parser.hpp>
#include <ML/Core/StringUtility.hpp>
#include <ML/Script/Interpreter.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	using RuleMap = typename Parser::RuleMap;

	/* * * * * * * * * * * * * * * * * * * * */

	Parser::Parser()
	{
		install_statements();
		install_expressions();
	}

	Parser::~Parser()
	{
		for (auto & pair : m_rules)
		{
			delete pair.second;
			pair.second = NULL;
		}
		m_rules.clear();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Parser::InfixToPostfix(const TokenList & ifx, TokenList & pfx, bool show)
	{
		// just one operand
		if (ifx.size() == 1 && ifx.front().isOperand())
		{
			pfx.push_back(ifx.front());

			return true;
		}

		TokenList stack = { '(' };

		pfx = TokenList();

		bool func = false;
		for (auto it = ifx.cbegin(); it != ifx.cend(); it++)
		{
			const Token & arg = (*it);

			if (arg.isOperator())
			{
				size_t count = 0;

				while (!stack.empty() && (stack.front() <= arg))
				{
					pfx.push_back(stack.front());

					stack.erase(stack.begin());

					if (++count > 100)
					{
						return Debug::logError("Infix To Postfix Stack overflow");
					}
				}

				stack.insert(stack.begin(), arg);
			}
			else if (arg == '(')
			{
				TokenList::const_iterator prev = (it - 1);
				if (!func && ((prev >= ifx.begin()) && prev->type == 'n'))
				{
					pfx.push_back(arg);
					func = true;
				}
				else
				{
					stack.insert(stack.begin(), arg);
				}
			}
			else if (arg == ')')
			{
				size_t count = 0;
				while (!stack.empty())
				{
					if (stack.front() == '(')
					{
						break;
					}
					pfx.push_back(stack.front());
					stack.erase(stack.begin());
					count++;
				}

				if (func)
				{
					func = false;
					pfx.insert(pfx.end() - count, arg);
				}

				if (!stack.empty())
				{
					stack.erase(stack.begin());
				}

				if (stack.empty())
				{
					Debug::logError("Missing left parenthesis (1)\n");
					return false;
				}
			}
			else if (arg.isOperand())
			{
				pfx.push_back(arg);
			}

			if (show) cout << "P: " << pfx << ml::endl;
		}

		while (!stack.empty() && stack.front() != '(')
		{
			pfx.push_back(stack.front());
			stack.erase(stack.begin());
		}

		if (show) cout << "P: " << pfx << ml::endl;

		if (stack.empty())
		{
			return Debug::logError("Missing left parenthesis (2)\n");
		}

		stack.erase(stack.begin());

		// Final Error Checking
		size_t numOperators = 0;
		size_t numOperands = 0;

		for (size_t i = 0; i < pfx.size(); i++)
		{
			const Token & arg = pfx[i];

			if (arg.isOperator())
			{
				numOperators++;
			}
			else
			{
				numOperands++;
			}
		}

		if (numOperators == 0 && numOperands == 1)
		{
			return true;
		}
		else if (numOperands <= 1)
		{
			return Debug::logError("Parser : Not enough operands");
		}
		else if (numOperators >= numOperands)
		{
			return Debug::logError("Parser : Too many operators");
		}

		return true;
	}

	TokenList Parser::infixToPostfix(const TokenList & ifx, bool show)  const
	{
		TokenList post;
		return ((InfixToPostfix(ifx, post, show))
			? (post)
			: (TokenList())
		);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	AST_Block * Parser::genFromList(const TokenList & value) const
	{
		return genFromTree(ML_Lexer.genTokenTree(value));
	}

	AST_Block * Parser::genFromTree(const TokenTree & value) const
	{
		AST_Block * root = NULL;

		if (!value.empty())
		{
			root = new AST_Block({});

			TokenTree::const_iterator it;
			for (it = value.begin(); it != value.end(); it++)
			{
				const TokenList & toks = (*it);

				if (toks.front("for"))
				{
					if (AST_Expr * a = generate<AST_Assign>(TokenList(*it).after(2)))
					{
						if (AST_Expr * e = genExpression(TokenList((*++it))))
						{
							if (AST_Expr * s = genExpression(TokenList((*++it)).pop_back()))
							{
								if (root->addChild(new AST_For(a, e, s)))
								{
									continue;
								}
								delete s;
							}
							delete e;
						}
						delete a;
					}
					return NULL;
				}
				else if (AST_Node * n = genNode(root, (*it)))
				{
					if (n == root)
					{
						root = root->block();
					}
					else
					{
						root->addChild(n);

						if (AST_Block * b = n->as<AST_Block>())
						{
							root = b;
						}
					}
				}

				if (m_showToks)
				{
					cout << toks << ml::endl;
				}
			}

			if (m_showTree)
			{
				cout << (*root) << endl;
			}
		}
		return root;
	}

	AST_Node * Parser::genNode(AST_Node * root, const TokenList & toks) const
	{
		if (root)
		{
			for (TokenList::const_iterator it = toks.begin(); it != toks.end(); it++)
			{
				switch (it->type)
				{
				case '{':
					return new AST_Block({});

				case '}':
					if (root->getParent())
					{
						return root;
					}

				default:
					return genStatement(toks);
				}
			}
		}
		return NULL;
	}

	AST_Stmt * Parser::genStatement(const TokenList & toks) const
	{
		if (toks.empty())
		{
			return NULL;
		}
		else if (AST_If			* temp = generate<AST_If>(toks))		{ return temp; }
		else if (AST_Elif		* temp = generate<AST_Elif>(toks))		{ return temp; }
		else if (AST_Else		* temp = generate<AST_Else>(toks))		{ return temp; }
		else if (AST_Print		* temp = generate<AST_Print>(toks))		{ return temp; }
		else if (AST_Return		* temp = generate<AST_Return>(toks))	{ return temp; }
		else if (AST_While		* temp = generate<AST_While>(toks))		{ return temp; }
		else if (AST_Delete		* temp = generate<AST_Delete>(toks))	{ return temp; }
		else if (AST_Include	* temp = generate<AST_Include>(toks))	{ return temp; }
		else
		{
			return genExpression(toks);
		}
	}
	
	AST_Expr * Parser::genExpression(const TokenList & toks) const
	{
		if (toks.empty())
		{
			return NULL;
		}
		else if (toks.isWrap('(', ')'))
		{
			return genExpression(toks.unwrapped());
		}
		else if (AST_Float		* temp = generate<AST_Float>(toks))		{ return temp; }
		else if (AST_Int		* temp = generate<AST_Int>(toks))		{ return temp; }
		else if (AST_String		* temp = generate<AST_String>(toks))	{ return temp; }
		else if (AST_Bool		* temp = generate<AST_Bool>(toks))		{ return temp; }
		else if (AST_Name		* temp = generate<AST_Name>(toks))		{ return temp; }
		else if (AST_BinOp		* temp = generate<AST_BinOp>(toks))		{ return temp; }
		else if (AST_Member		* temp = generate<AST_Member>(toks))	{ return temp; }
		else if (AST_New		* temp = generate<AST_New>(toks))		{ return temp; }
		else if (AST_Struct		* temp = generate<AST_Struct>(toks))	{ return temp; }
		else if (AST_Command	* temp = generate<AST_Command>(toks))	{ return temp; }
		else if (AST_SizeOf		* temp = generate<AST_SizeOf>(toks))	{ return temp; }
		else if (AST_TypeID		* temp = generate<AST_TypeID>(toks))	{ return temp; }
		else if (AST_TypeName	* temp = generate<AST_TypeName>(toks))	{ return temp; }
		else if (AST_NodeID		* temp = generate<AST_NodeID>(toks))	{ return temp; }
		else if (AST_System		* temp = generate<AST_System>(toks))	{ return temp; }
		else if (AST_Input		* temp = generate<AST_Input>(toks))		{ return temp; }
		else if (AST_Func		* temp = generate<AST_Func>(toks))		{ return temp; }
		else if (AST_Assign		* temp = generate<AST_Assign>(toks))	{ return temp; }
		else if (AST_Call		* temp = generate<AST_Call>(toks))		{ return temp; }
		else if (AST_Subscr		* temp = generate<AST_Subscr>(toks))	{ return temp; }
		else if (AST_Array		* temp = generate<AST_Array>(toks))		{ return temp; }
		else
		{
			if (AST_BinOp * temp = genNestedBinOp(infixToPostfix(toks, m_showItoP)))
			{
				return temp;
			}
			else
			{
				return new AST_String(toks.str()); // Something Went Wrong
			}
		}
	}
	
	AST_BinOp *	Parser::genNestedBinOp(const TokenList & toks) const
	{
		AST_BinOp * temp;

		if (toks.empty())
		{
			return (temp = NULL);
		}

		std::stack<AST_Expr *> stack;
		
		for (TokenList::const_iterator it = toks.begin(); it != toks.end(); it++)
		{
			// Call
			if (((*(it + 0)) == 'n') && 
				((*(it + 1)) == '('))
			{
				TokenList params(*it);

				while ((*(it++)) != ')')
				{
					params.push_back(*it);
				}

				if (AST_Call * c = generate<AST_Call>(params))
				{
					stack.push(c);
				}
				else
				{
					return (temp = NULL);
				}
			}

			// BinOp
			if (Operator op = Operator(it->data))
			{
				if (stack.size() < 2)
				{
					return (temp = NULL);
				}

				AST_Expr * rhs = stack.top();
				stack.pop();

				AST_Expr * lhs = stack.top();
				stack.pop();

				stack.push(new AST_BinOp(op, lhs, rhs));
			}
			else
			{
				stack.push(genExpression(*it));
			}
		}

		if (AST_BinOp * binop = stack.top()->as<AST_BinOp>())
		{
			return (temp = binop);
		}
		else
		{
			return (temp = NULL);
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * */

	List<AST_Expr *> Parser::genArrayElems(const TokenList & toks) const
	{
		List<AST_Expr *> elems;

		TokenList	temp;
		int32_t		depth = 0;

		for (TokenList::const_iterator it = toks.begin(); it != toks.end(); it++)
		{
			if ((*it) == '(')
			{
				depth++;

				temp.push_back(*it);
			}
			else if ((*it) == ')')
			{
				if (--depth > 0)
				{
					temp.push_back(*it);
				}
			}
			else if ((*it) == ',' && !depth)
			{
				if (!temp.empty())
				{
					elems.push_back(genExpression(temp));

					temp.clear();

					continue;
				}

				temp.push_back(*it);
			}
			else
			{
				temp.push_back(*it);
			}
		}

		if (!temp.empty())
		{
			elems.push_back(genExpression(temp));
		}

		return elems;
	}

	List<AST_Expr *> Parser::genCallParams(const TokenList & toks) const
	{
		List<AST_Expr *> params;

		if (toks.match_type_str("()"))
		{
			return params;
		}

		TokenList	temp;
		int32_t		depth = 0;

		for (TokenList::const_iterator it = toks.begin() + 1; it != toks.end(); it++)
		{
			if ((*it) == '(')
			{
				depth++;

				temp.push_back(*it);
			}
			else if ((*it) == ')')
			{
				if (--depth > 0)
				{
					temp.push_back(*it);
				}
			}
			else if ((*it) == ',' && !depth)
			{
				if (!temp.empty())
				{
					params.push_back(genExpression(temp));
					temp.clear();
					continue;
				}

				temp.push_back(*it);
			}
			else
			{
				temp.push_back(*it);
			}
		}

		if (!temp.empty())
		{
			params.push_back(genExpression(temp));
		}

		return params;
	}

	List<AST_Expr *> Parser::genFuncParams(const TokenList & toks) const
	{
		List<AST_Expr *> params;

		if (toks.isWrap('(', ')'))
		{
			for (TokenList::const_iterator it = toks.begin() + 1; it != toks.end() - 1; it++)
			{
				if ((*it).type != ',')
				{
					if (AST_Name * name = generate<AST_Name>(*it))
					{
						params.push_back(name);
					}
				}
			}
		}

		return params;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Parser::install_statements()
	{
		install<AST_If>(new NodeMaker<AST_If>([](const TokenList & toks)
		{
			AST_If * temp;
			if (toks.match_type_str("n(") && toks.back(')'))
			{
				if (toks.front("if"))
				{
					if (AST_Expr * e = ML_Parser.genExpression(toks.after(1)))
					{
						return (temp = new AST_If(e));
					}
				}
			}
			return (temp = NULL);
		}));

		install<AST_Elif>(new NodeMaker<AST_Elif>([](const TokenList & toks)
		{
			AST_Elif * temp;
			if (toks.match_type_str("n(") && toks.back(')'))
			{
				if (toks.front("elif"))
				{
					if (AST_Expr * e = ML_Parser.genExpression(toks.after(1)))
					{
						return (temp = new AST_Elif(e));
					}
				}
			}
			return (temp = NULL);
		}));

		install<AST_Else>(new NodeMaker<AST_Else>([](const TokenList & toks)
		{
			AST_Else * temp;
			return
				((toks.size(1)) &&
				(toks.front("else")))
				? (temp = new AST_Else())
				: (temp = NULL);
		}));

		install<AST_Return>(new NodeMaker<AST_Return>([](const TokenList & toks)
		{
			AST_Return * temp;
			if (toks.front("return"))
			{
				if (AST_Expr * e = ML_Parser.genExpression(toks.after(1)))
				{
					return (temp = new AST_Return(e));
				}
				else
				{
					return (temp = new AST_Return(new AST_Int(0)));
				}
			}
			return (temp = NULL);
		}));

		install<AST_While>(new NodeMaker<AST_While>([](const TokenList & toks)
		{
			AST_While * temp;
			if (toks.match_type_str("n(") && toks.back(")"))
			{
				if (toks.front("while"))
				{
					if (AST_Expr * e = ML_Parser.genExpression(toks.after(1)))
					{
						return (temp = new AST_While(e));
					}
				}
			}
			return (temp = NULL);
		}));

		install<AST_Delete>(new NodeMaker<AST_Delete>([](const TokenList & toks)
		{
			AST_Delete * temp;
			if (toks.match_type_str("n(") && toks.back(")"))
			{
				if (toks.front("delete"))
				{
					if (AST_Expr * e = ML_Parser.genExpression(toks.after(1)))
					{
						if (AST_Name * n = e->as<AST_Name>())
						{
							return (temp = new AST_Delete(n));
						}
						else { delete e; }
					}
				}
			}
			return (temp = NULL);
		}));

		install<AST_Include>(new NodeMaker<AST_Include>([](const TokenList & toks)
		{
			AST_Include * temp;
			if (toks.match_type_str("n(") && toks.back(")"))
			{
				if (toks.front("include"))
				{
					if (AST_Expr * e = ML_Parser.genExpression(toks.after(1)))
					{
						if (AST_String * s = e->as<AST_String>())
						{
							return (temp = new AST_Include(s));
						}
						else { delete e; }
					}
				}
			}
			return (temp = NULL);
		}));

		install<AST_Print>(new NodeMaker<AST_Print>([](const TokenList & toks)
		{
			AST_Print * temp;
			if (toks.match_type_str("n(") && toks.back(")"))
			{
				// Printl
				if (toks.front("printl"))
				{
					if (AST_Expr * e = ML_Parser.genExpression(toks.after(1)))
					{
						return (temp = new AST_Print(e, true));
					}
					else
					{
						return (temp = new AST_Print(new AST_String(String()), true));
					}
				}
				// Print
				else if (toks.front("print"))
				{
					if (AST_Expr * e = ML_Parser.genExpression(toks.after(1)))
					{
						return (temp = new AST_Print(e, false));
					}
					else
					{
						return (temp = new AST_Print(new AST_String(String()), false));
					}
				}
			}
			return (temp = NULL);
		}));
	}

	void Parser::install_expressions()
	{
		install<AST_Bool>(new NodeMaker<AST_Bool>([](const TokenList & toks)
		{
			AST_Bool * temp;
			return (
				(toks.size(1)) &&
				(toks.front('n')) &&
				(StringUtility::IsBool(toks.front().data))
					? (temp = new AST_Bool(StringUtility::ToBool(toks.front().data)))
					: (temp = NULL)
				);
		}));

		install<AST_Int>(new NodeMaker<AST_Int>([](const TokenList & toks)
		{
			AST_Int * temp;
			return (
				(toks.size(1)) &&
				(toks.front('i')) &&
				(StringUtility::IsInt(toks.front().data))
					? (temp = new AST_Int(StringUtility::ToInt(toks.front().data)))
					: (temp = NULL)
				);
		}));

		install<AST_Float>(new NodeMaker<AST_Float>([](const TokenList & toks)
		{
			AST_Float * temp;
			return (
				(toks.size(1)) &&
				(toks.front('f')) &&
				(StringUtility::IsDecimal(toks.front().data))
					? (temp = new AST_Float(StringUtility::ToFloat(toks.front().data)))
					: (temp = NULL)
				);
		}));

		install<AST_Name>(new NodeMaker<AST_Name>([](const TokenList & toks)
		{
			AST_Name * temp;
			return (
				(toks.size(1)) &&
				(toks.front('n'))
					? (temp = new AST_Name(toks.front().data))
					: (temp = NULL)
				);
		}));

		install<AST_String>(new NodeMaker<AST_String>([](const TokenList & toks)
		{
			AST_String * temp;
			return (
				(toks.size(1)) &&
				(toks.front('s'))
					? (temp = new AST_String(toks.front().data))
					: (temp = NULL)
				);
		}));
	
		install<AST_Array>(new NodeMaker<AST_Array>([](const TokenList & toks)
		{
			AST_Array * temp;
			return ((toks.isWrap('[', ']'))
				? ((toks.match_type_str("[]"))
					? (temp = new AST_Array({}))
					: (temp = new AST_Array(ML_Parser.genArrayElems(toks.unwrapped()))))
				: (temp = NULL));
		}));

		install<AST_Struct>(new NodeMaker<AST_Struct>([](const TokenList & toks)
		{
			AST_Struct * temp;
			if (toks.match_type_str("n=$(") && toks.back(')'))
			{
				return (temp = new AST_Struct(
					toks.front().data,
					ML_Parser.genCallParams(toks.after(3))
				));
			}
			return (temp = NULL);
		}));

		install<AST_Call>(new NodeMaker<AST_Call>([](const TokenList & toks)
		{
			AST_Call * temp;
			if (toks.match_type_str("n(") && toks.back(")"))
			{
				return (temp = new AST_Call(
					new AST_Name(toks.front().data),
					ML_Parser.genCallParams(toks.after(1))
				));
			}
			return (temp = NULL);
		}));

		install<AST_Func>(new NodeMaker<AST_Func>([](const TokenList & toks)
		{
			AST_Func * temp;
			if (toks.match_type_str("n=[](") && toks.back(')'))
			{
				return (temp = new AST_Func(
					toks.front().data,
					ML_Parser.genFuncParams(toks.after(4))
				));
			}
			return (temp = NULL);
		}));
	
		install<AST_BinOp>(new NodeMaker<AST_BinOp>([](const TokenList & toks)
		{
			AST_BinOp * temp;
			if ((toks.size() >= 4) &&
				(toks.match_type_str("EOOA")))
			{
				if (Operator op = Operator(
					(toks.begin() + 1)->data,
					(toks.begin() + 2)->data))
				{
					return (temp = new AST_BinOp(
						op,
						ML_Parser.genExpression(toks.front()),
						ML_Parser.genExpression(toks.after(3))));
				}
			}
			return (temp = NULL);
		}));

		install<AST_Subscr>(new NodeMaker<AST_Subscr>([](const TokenList & toks)
		{
			AST_Subscr * temp;
			if (toks.match_type_str("n[E]"))
			{
				if (AST_Name * n = new AST_Name(toks.front().data))
				{
					if (AST_Expr * e = ML_Parser.genExpression(toks[2]))
					{
						return (temp = new AST_Subscr(n, e));
					}
					else { delete e; }
				}
				else { delete n; }
			}
			return (temp = NULL);
		}));

		install<AST_Member>(new NodeMaker<AST_Member>([](const TokenList & toks)
		{
			AST_Member * temp;
			if (toks.match_type_str("n.n"))
			{
				if (AST_Name * n = ML_Parser.generate<AST_Name>(toks.front()))
				{
					if (AST_Name * e = ML_Parser.generate<AST_Name>(toks.back()))
					{
						return (temp = new AST_Member(n, e));
					}
					else { delete e; }
				}
				else { delete n; }
			}
			return (temp = NULL);
		}));

		install<AST_Assign>(new NodeMaker<AST_Assign>([](const TokenList & toks)
		{
			AST_Assign * temp;
			if (toks.match_type_str("n[E]=A"))
			{
				TokenList list({
					*(toks.begin() + 0),
					*(toks.begin() + 1),
					*(toks.begin() + 2),
					*(toks.begin() + 3)
				});

				if (AST_Subscr* subscr = ML_Parser.generate<AST_Subscr>(list))
				{
					if (AST_Expr * value = ML_Parser.genExpression(toks.after(5)))
					{
						return (temp = new AST_Assign(Operator::OP_SET, subscr, value));
					}
				}
			}
			else if (toks.match_type_str("n[E]O=A"))
			{
				TokenList list({
					*(toks.begin() + 0),
					*(toks.begin() + 1),
					*(toks.begin() + 2),
					*(toks.begin() + 3)
				});

				if (AST_Subscr* subscr = ML_Parser.generate<AST_Subscr>(list))
				{
					if (Operator op = Operator(
						(toks.begin() + 4)->data,
						(toks.begin() + 5)->data))
					{
						if (AST_Expr * value = ML_Parser.genExpression(toks.after(6)))
						{
							return (temp = new AST_Assign(op, subscr, value));
						}
					}
				}
			}
			else if (toks.match_type_str("n.n=A"))
			{
				return (temp = new AST_Assign(
					Operator::OP_SET,
					new AST_Name(toks.begin()->data),
					ML_Parser.genExpression(toks.after(2))
				));
			}
			else if (toks.match_type_str("n.nO=A"))
			{
				if (Operator op = Operator(
					(toks.begin() + 1)->data,
					(toks.begin() + 2)->data))
				{
					return (temp = new AST_Assign(
						op,
						new AST_Name(toks.begin()->data),
						ML_Parser.genExpression(toks.after(3))
					));
				}
			}
			else if (toks.match_type_str("n=A"))
			{
				return (temp = new AST_Assign(
					Operator::OP_SET,
					new AST_Name(toks.begin()->data),
					ML_Parser.genExpression(toks.after(2))
				));
			}
			else if (toks.match_type_str("nO=A"))
			{
				if (Operator op = Operator(
					(toks.begin() + 1)->data,
					(toks.begin() + 2)->data))
				{
					return (temp = new AST_Assign(
						op,
						new AST_Name(toks.begin()->data),
						ML_Parser.genExpression(toks.after(3))
					));
				}
			}
			return (temp = NULL);
		}));

		install<AST_Input>(new NodeMaker<AST_Input>([](const TokenList & toks)
		{
			AST_Input * temp;
			if (toks.match_type_str("n(") && toks.back(')'))
			{
				if (toks.match_data(toks.begin(), { "input" }))
				{
					return (temp = new AST_Input(ML_Parser.genExpression(toks.after(1))));
				}
			}
			return (temp = NULL);
		}));

		install<AST_Command>(new NodeMaker<AST_Command>([](const TokenList & toks)
		{
			AST_Command * temp;
			if (toks.match_type_str("n(") && toks.back(")"))
			{
				if (toks.front("command"))
				{
					if (AST_Expr * e = ML_Parser.genExpression(toks.after(1)))
					{
						return (temp = new AST_Command(e));
					}
					else
					{
						return new AST_Command(new AST_String(String()));
					}
				}
			}
			return (temp = NULL);
		}));

		install<AST_New>(new NodeMaker<AST_New>([](const TokenList & toks)
		{
			AST_New * temp;
			if (toks.match_type_str("n(") && toks.back(")"))
			{
				if (toks.front("new"))
				{
					if (AST_Expr * e = ML_Parser.genExpression(toks.after(1)))
					{
						return (temp = new AST_New(e));
					}
					else
					{
						return new AST_New(new AST_Int(0));
					}
				}
			}
			return (temp = NULL);
		}));

		install<AST_NodeID>(new NodeMaker<AST_NodeID>([](const TokenList & toks)
		{
			AST_NodeID * temp;
			if (toks.match_type_str("n(") && toks.back(")"))
			{
				if (toks.front("nodeid"))
				{
					if (AST_Expr * e = ML_Parser.genExpression(toks.after(1)))
					{
						return (temp = new AST_NodeID(e));
					}
					else
					{
						return new AST_NodeID(new AST_Int(0));
					}
				}
			}
			return (temp = NULL);
		}));

		install<AST_SizeOf>(new NodeMaker<AST_SizeOf>([](const TokenList & toks)
		{
			AST_SizeOf * temp;
			if (toks.match_type_str("n(") && toks.back(")"))
			{
				if (toks.front("sizeof"))
				{
					if (AST_Expr * e = ML_Parser.genExpression(toks.after(1)))
					{
						return (temp = new AST_SizeOf(e));
					}
				}
			}
			return (temp = NULL);
		}));

		install<AST_TypeID>(new NodeMaker<AST_TypeID>([](const TokenList & toks)
		{
			AST_TypeID * temp;
			if (toks.match_type_str("n(") && toks.back(")"))
			{
				if (toks.front("typeid"))
				{
					if (AST_Expr * e = ML_Parser.genExpression(toks.after(1)))
					{
						return (temp = new AST_TypeID(e));
					}
				}
			}
			return (temp = NULL);
		}));

		install<AST_TypeName>(new NodeMaker<AST_TypeName>([](const TokenList & toks)
		{
			AST_TypeName * temp;
			if (toks.match_type_str("n(") && toks.back(")"))
			{
				if (toks.front("typename"))
				{
					if (AST_Expr * e = ML_Parser.genExpression(toks.after(1)))
					{
						return (temp = new AST_TypeName(e));
					}
				}
			}
			return (temp = NULL);
		}));

		install<AST_System>(new NodeMaker<AST_System>([](const TokenList & toks)
		{
			AST_System * temp;
			if (toks.match_type_str("n(") && toks.back(")"))
			{
				if (toks.front("system"))
				{
					return (temp = new AST_System(
						ML_Parser.genCallParams(toks.after(1))
					));
				}
			}
			return (temp = NULL);
		}));
	}

	/* * * * * * * * * * * * * * * * * * * * */
}