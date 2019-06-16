#include <ML/Script/AST_Expr.hpp>
#include <ML/Script/Interpreter.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	// Expression
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Expr::AST_Expr(int32_t type)
		: AST_Stmt(ST_Expr)
		, exprType(type)
	{
	}

	OStream & AST_Expr::display(OStream & out) const
	{
		return AST_Stmt::display(out);
	}

	Var AST_Expr::evaluate() const
	{
		return Var().voidValue();
	}

	bool AST_Expr::run()
	{
		const Var v = evaluate();
		if (v.isErrorType())
		{
			return Debug::logError("AST_Expr : Invalid Syntax: \'{0}\'", v.textValue());
		}
		return true;
	}

	
	// Array
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Array::AST_Array(const List<AST_Expr *> & values)
		: AST_Expr(EX_Array)
		, values(values)
	{
		for (auto v : values)
			addChild(v);
	}

	OStream & AST_Array::display(OStream & out) const
	{
		out << (FG::Black | BG::Yellow) << "[" << FMT() << " ";

		Var v;
		if ((v = evaluate()).isArrayType())
		{
			const List<Var> list = v.listValue();
			for (size_t i = 0, imax = list.size(); i < imax; i++)
			{
				out << list[i] << ((i < imax - 1) ? ", " : " ");
			}
		}
		return out << (FG::Black | BG::Yellow) << "]" << FMT();
	}

	Var AST_Array::evaluate() const
	{
		TokenList list;
		for (auto it = values.begin(); it != values.end(); it++)
		{
			list.push_back(Token((*it)->evaluate()));
		}
		return Var().arrayValue(list);
		//TokenList items;
		//for (auto it = values.begin(); it != values.end(); it++)
		//{
		//	items.push_back((*it)->evaluate().tokensValue());
		//}
		//return Var().arrayValue(items);
	}


	// Assign
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Assign::AST_Assign(Operator op, AST_Expr * lhs, AST_Expr * rhs)
		: AST_Expr(EX_Assign)
		, op(op)
		, lhs(lhs)
		, rhs(rhs)
	{
		addChild(lhs);
		addChild(rhs);
	}

	OStream & AST_Assign::display(OStream & out) const
	{
		return out << (*lhs) << " " << op << " " << (*rhs);
	}

	Var AST_Assign::evaluate() const
	{
		if (AST_Name * n = lhs->as<AST_Name>())
		{
			if (op == Operator::OP_SET)
			{
				if (Var * v = block()->setVar(n->value, rhs->evaluate()))
				{
					return (*v);
				}
				else
				{
					return Var().errorValue("AST_Assign : Failed Setting Block Var");
				}
			}
			else if (Var * v = block()->getVar(n->value))
			{
				switch (op.type)
				{
				case Operator::OP_ADD: return v->Add(rhs->evaluate());
				case Operator::OP_SUB: return v->Sub(rhs->evaluate());
				case Operator::OP_MUL: return v->Mul(rhs->evaluate());
				case Operator::OP_DIV: return v->Div(rhs->evaluate());
				case Operator::OP_POW: return v->Pow(rhs->evaluate());
				case Operator::OP_MOD: return v->Mod(rhs->evaluate());
				}
			}
		}
		return Var().errorValue("AST_Assign : Failed Setting Var");
	}

	bool AST_Assign::run()
	{
		if (evaluate().isErrorType())
		{
			// breaking here not always needed?
		}
		return runNext();
	}


	// Bool
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Bool::AST_Bool(bool value)
		: AST_Expr(EX_Bool)
		, value(value)
	{
	}

	OStream & AST_Bool::display(OStream & out) const
	{
		out << Var().boolValue(value);
		return out;
	}

	Var AST_Bool::evaluate() const
	{
		return Var().boolValue(value);
	}


	// Call
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Call::AST_Call(AST_Name * name, const List<AST_Expr *> & args)
		: AST_Expr(EX_Call)
		, name(name)
		, args(args)
	{
		addChild(name);
		for (auto p : args)
			addChild(p);
	}

	OStream & AST_Call::display(OStream & out) const
	{
		out << (FG::Green | BG::Black)
			<< name->value << "("
			<< FMT();
		List<AST_Expr *>::const_iterator it;
		for (it = args.begin(); it != args.end(); it++)
		{
			out << * ( * it) << ((it != args.end() - 1) ? ", " : "");
		}
		out << (FG::Green | BG::Black) << ")"
			<< FMT();
		return out;
	}

	AST_Func * AST_Call::getFunc() const
	{
		if (AST_Block * b = block())
		{
			if (Var * v = b->getVar(name->value))
			{
				if (v->isFuncType())
				{
					return b->getFunc(v->textValue());
				}
			}
		}
		return nullptr;
	}

	Var AST_Call::evaluate() const
	{
		if (AST_Func * func = getFunc())
		{
			if (AST_Block * body = func->nextAs<AST_Block>())
			{
				if (func->args.size() == args.size())
				{
					for (size_t i = 0; i < func->args.size(); i++)
					{
						if (AST_Name * n = func->args[i]->as<AST_Name>())
						{
							if (!body->setVar(n->value, args[i]->evaluate()))
							{
								return Var().errorValue(
									"AST_Call : {0} Set Arg Failed", n->value);
							}
						}
						else
						{
							return Var().errorValue(
								"AST_Call : Funcion argument did not evaluate to a Name");
						}
					}

					if (body->runFirst())
					{
						for (size_t i = 0; i < func->args.size(); i++)
						{
							if (AST_Name * n = func->args[i]->as<AST_Name>())
							{
								if (!body->delVar(n->value))
								{
									return Var().errorValue(
										"AST_Call : {0} Del Arg Failed", n->value);
								}
							}
							else
							{

							}
						}

						return body->evaluate();
					}
					else
					{
						return Var().errorValue(
							"AST_Call : {0} Failed Running Function Body",
							func->name);
					}
				}
				else
				{
					return Var().errorValue(
						"AST_Call : Expected {0} args, recieved {1}",
						func->args.size(), 
						args.size());
				}
			}
			else
			{
				return Var().errorValue(
					"AST_Call : {0} Function Body Not Found", 
					func->name);
			}
		}
		else
		{
			return Var().errorValue(
				"AST_Call : {0} Function Name Not Found",
				name->value);
		}
	}

	bool AST_Call::run()
	{
		Var v;
		if ((v = evaluate()).isErrorType())
		{
			return Debug::logError(v.textValue());
		}
		return runNext();
	}


	// Float
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Float::AST_Float(float value)
		: AST_Expr(EX_Float)
		, value(value)
	{
	}

	OStream & AST_Float::display(OStream & out) const
	{
		out << Var().floatValue(value);
		return out;
	}

	Var AST_Float::evaluate() const
	{
		return Var().floatValue(value);
	}


	// Function
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Func::AST_Func(const String & name, const List<AST_Expr *> & args)
		: AST_Expr(EX_Func)
		, name(name)
		, args(args)
	{
		for (auto p : args)
			addChild(p);
	}

	OStream & AST_Func::display(OStream & out) const
	{
		out << (FG::White | BG::DarkGray) << name << FMT() << " = [](";
		List<AST_Expr *>::const_iterator it;
		for (it = args.begin(); it != args.end(); it++)
		{
			out << * ( * it) << ((it != args.end() - 1) ? ", " : "");
		}
		out << ")";
		return out;
	}

	Var AST_Func::evaluate() const
	{
		return Var().pointerValue({ getID(), name });
	}

	bool AST_Func::run()
	{
		if (block()->addFunc(name, this))
		{
			return runNext();
		}
		return Debug::logError("AST_Func : Run Failed");
	}


	// Input
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Input::AST_Input(AST_Expr * prompt)
		: AST_Expr(EX_Input)
		, prompt(prompt)
	{
		addChild(prompt);
	}

	OStream & AST_Input::display(OStream & out) const
	{
		return out << FG::Cyan << "input" << FMT() << "(" << ")";
	}

	Var AST_Input::evaluate() const
	{
		if (prompt)
		{
			cout << (*prompt).evaluate().textValue();
		}
		String line;
		std::getline(cin, line);
		return Var().stringValue(line);
	}


	// Int
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Int::AST_Int(int32_t value)
		: AST_Expr(EX_Int)
		, value(value)
	{
	}

	OStream & AST_Int::display(OStream & out) const
	{
		out << Var().intValue(value);
		return out;
	}

	Var AST_Int::evaluate() const
	{
		return Var().intValue(value);
	}


	// Name
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Name::AST_Name(const String & value)
		: AST_Expr(EX_Name)
		, value(value)
	{
	}

	OStream & AST_Name::display(OStream & out) const
	{
		if (AST_Block * b = block())
		{
			if (Var * v = block()->getVar(value))
			{
				out << ( * v);
			}
			else
			{
				out << Var().pointerValue({ block()->getID(), value });
			}
		}
		else
		{
			out << Var().errorValue("AST_Name : Null Ref: {0}", value);
		}
		return out;
	}

	Var AST_Name::evaluate() const
	{
		if (AST_Block * b = block())
		{
			if (Var * v = block()->getVar(value))
			{
				return ( * v);
			}
			else
			{
				return * block()->setVar(value, Var().pointerValue({ block()->getID(), value }));
			}
		}

		return Var().errorValue("AST_Name : Null reference");
	}


	// BinOp
	/* * * * * * * * * * * * * * * * * * * * */
	AST_BinOp::AST_BinOp(const Operator & op, AST_Expr * lhs, AST_Expr * rhs)
		: AST_Expr(EX_BinOp)
		, op(op)
		, lhs(lhs)
		, rhs(rhs)
	{
		addChild(lhs);
		addChild(rhs);
	}

	OStream & AST_BinOp::display(OStream & out) const
	{
		out << "(" << * lhs << " " << op << " " << * rhs << ")";
		return out;
	}

	Var AST_BinOp::evaluate() const
	{
		if (lhs && rhs)
		{
			switch (op.type)
			{
			case Operator::OP_ADD: return lhs->evaluate() + rhs->evaluate();
			case Operator::OP_SUB: return lhs->evaluate() - rhs->evaluate();
			case Operator::OP_MUL: return lhs->evaluate() * rhs->evaluate();
			case Operator::OP_DIV: return lhs->evaluate() / rhs->evaluate();
			case Operator::OP_POW: return lhs->evaluate() ^ rhs->evaluate();
			case Operator::OP_MOD: return lhs->evaluate() % rhs->evaluate();

			case Operator::OP_EQU: return Var().boolValue(lhs->evaluate() == rhs->evaluate());
			case Operator::OP_NEQ: return Var().boolValue(lhs->evaluate() != rhs->evaluate());
			case Operator::OP_LT:  return Var().boolValue(lhs->evaluate()  < rhs->evaluate());
			case Operator::OP_GT:  return Var().boolValue(lhs->evaluate()  > rhs->evaluate());
			case Operator::OP_LTE: return Var().boolValue(lhs->evaluate() <= rhs->evaluate());
			case Operator::OP_GTE: return Var().boolValue(lhs->evaluate() >= rhs->evaluate());

			case Operator::OP_AND: return Var().boolValue(lhs->evaluate() && rhs->evaluate());
			case Operator::OP_OR:  return Var().boolValue(lhs->evaluate() || rhs->evaluate());
			}
		}
		return Var().errorValue("AST_BinOp : Invalid Operation {0} {1} {2}", 
			(*lhs), op, (*rhs));
	}


	// String 
	/* * * * * * * * * * * * * * * * * * * * */
	AST_String::AST_String(const String & value)
		: AST_Expr(EX_String)
		, value(value)
	{
	}

	OStream & AST_String::display(OStream & out) const
	{
		return out << Var().stringValue(value);
	}

	Var AST_String::evaluate() const
	{
		return Var().stringValue(value);
	}


	// Struct
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Struct::AST_Struct(const String & name, const List<AST_Expr *> & args)
		: AST_Expr(EX_Struct)
		, name(name)
		, args(args)
	{
		for (auto c : args) addChild(c);
	}

	AST_Block * AST_Struct::getBody() const
	{
		return nextAs<AST_Block>();
	}

	OStream & AST_Struct::display(OStream & out) const
	{
		out << (FG::White | BG::DarkBlue)
			<< name << FMT() << " = $(";
		
		List<AST_Expr *>::const_iterator it;
		for (it = args.begin(); it != args.end(); it++)
		{
			out << *(*it) << ((it != args.end() - 1) ? ", " : "");
		}
		
		return out << FMT() << ")";
	}

	Var AST_Struct::evaluate() const
	{
		return Var().intValue(getID());
		return Var().pointerValue({ getID(), name });
	}

	bool AST_Struct::run()
	{
		if (AST_Block * body = getBody())
		{
			if (block()->addStruct(name, this))
			{
				if (body->runFirst())
				{
					return runNext();
				}
				return Debug::logError("AST_Struct : Failed initializing body : {0}", name);
			}
			return Debug::logError("AST_Struct : Struct redefinition : {0}", name);
		}
		return Debug::logError("AST_Struct : Body not found : {0}", name);
	}


	// Subscript
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Subscr::AST_Subscr(AST_Name * name, AST_Expr * index)
		: AST_Expr(EX_Subscr)
		, name(name)
		, index(index)
	{
		addChild(name);
		addChild(index);
	}

	OStream & AST_Subscr::display(OStream & out) const
	{
		return out << "" << name->value << "[" << * index << "]";
	}

	Var AST_Subscr::evaluate() const
	{
		if (AST_Block * b = block())
		{
			if (Var * a = b->getVar(name->value))
			{
				Var v = index->evaluate();
				if (v.isIntType())
				{
					return a->elemValue(v.intValue());
				}
				else
				{
					return Var().errorValue("AST_Subscr : Could not evaluate expression to index: \'{0}\'", v.textValue());
				}
			}
			return Var().errorValue("AST_Subscr : Null reference {0}", name->value);
		}
		return Var().errorValue("AST_Subscr : Null Block {0}", name->value);
	}


	// Command
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Command::AST_Command(AST_Expr * expr)
		: AST_Expr(EX_Command)
		, expr(expr)
	{
		addChild(expr);
	}

	OStream & AST_Command::display(OStream & out) const
	{
		return out << FG::Cyan << "command" << FMT() << "(" << (*expr) << FMT() << ")";
	}

	Var AST_Command::evaluate() const
	{
		return ML_Interpreter.execCommand(expr->evaluate().textValue());
	}

	bool AST_Command::run()
	{
		if (evaluate().isValid())
		{
			return runNext();
		}
		return Debug::logError("AST_Sys : Call Failed");
	}


	// Size Of
	/* * * * * * * * * * * * * * * * * * * * */
	AST_SizeOf::AST_SizeOf(AST_Expr * expr)
		: AST_Expr(EX_SizeOf)
		, expr(expr)
	{
		addChild(expr);
	}

	OStream & AST_SizeOf::display(OStream & out) const
	{
		return out << FG::Cyan << "sizeof" << FMT() << "(" << (*expr) << FMT() << ")";
	}

	Var AST_SizeOf::evaluate() const
	{
		return Var().intValue(expr->evaluate().sizeOfValue());
	}


	// Type ID
	/* * * * * * * * * * * * * * * * * * * * */
	AST_TypeID::AST_TypeID(AST_Expr * expr)
		: AST_Expr(EX_TypeID)
		, expr(expr)
	{
		addChild(expr);
	}

	OStream & AST_TypeID::display(OStream & out) const
	{
		return out << FG::Cyan << "typeid" << FMT() << "(" << (*expr) << FMT() << ")";
	}

	Var AST_TypeID::evaluate() const
	{
		return Var().intValue(expr->evaluate().getTypeID());
	}


	// Type Name
	/* * * * * * * * * * * * * * * * * * * * */
	AST_TypeName::AST_TypeName(AST_Expr * expr)
		: AST_Expr(EX_TypeName)
		, expr(expr)
	{
		addChild(expr);
	}

	OStream & AST_TypeName::display(OStream & out) const
	{
		return out << FG::Cyan << "typename" << FMT() << "(" << (*expr) << FMT() << ")";
	}

	Var AST_TypeName::evaluate() const
	{
		return Var().stringValue(expr->evaluate().getTypeName());
	}


	// Node ID
	/* * * * * * * * * * * * * * * * * * * * */
	AST_NodeID::AST_NodeID(AST_Expr * expr)
		: AST_Expr(EX_NodeID)
		, expr(expr)
	{
		addChild(expr);
	}
	
	OStream & AST_NodeID::display(OStream & out) const
	{
		return out << FG::Cyan << "nodeid" << FMT() << "(" << (*expr) << FMT() << ")";
	}
	
	Var AST_NodeID::evaluate() const
	{
		return Var().intValue(expr->block()->getID());
	}


	// New
	/* * * * * * * * * * * * * * * * * * * * */
	AST_New::AST_New(AST_Expr * expr)
		: AST_Expr(EX_New)
		, expr(expr)
	{
		addChild(expr);
	}
	
	OStream & AST_New::display(OStream & out) const
	{
		return out << FG::Cyan << "new" << FMT() << "(" << (*expr) << FMT() << ")";
	}
	
	Var AST_New::evaluate() const
	{
		if (expr)
		{
			if (AST_Name * name = expr->as<AST_Name>())
			{
				if (Var * v = block()->getVar(name->value))
				{
					if (v->isStructType())
					{
						if (AST_Struct * s = block()->getStruct(v->textValue()))
						{
							if (AST_Block * body = s->getBody())
							{
								return s->evaluate();
							}
						}
					}
				}
			}

			return expr->evaluate();
		}
		return Var().errorValue("AST_New : No Expression");
	}
	

	// Member
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Member::AST_Member(AST_Name * name, AST_Name * expr)
		: AST_Expr(EX_Member)
		, name(name)
		, expr(expr)
	{
		addChild(name);
		addChild(expr);
	}
	
	AST_Struct * AST_Member::getStruct() const
	{
		if (name)
		{
			if (AST_Block * b = block())
			{
				if (Var * v = block()->getVar(name->value))
				{
					if (v->isStructType())
					{
						return b->getStruct(v->textValue());
					}
				}
			}
		}
		return nullptr;
	}
	
	OStream & AST_Member::display(OStream & out) const
	{
		return out << (*name) << FG::Cyan << "." << FMT() << (*expr) << FMT();
	}
	
	Var AST_Member::evaluate() const
	{
		if (name)
		{
			if (expr)
			{
				if (AST_Struct * str = getStruct())
				{
					if (AST_Block * body = str->getBody())
					{
						if (Var * v = body->getVar(expr->value))
						{
							return Var().pointerValue({ 
								v->getScope(), 
								v->textValue() 
							});
						}
						return Var().errorValue("AST_Member : No Value");
					}
					return Var().errorValue("AST_Member : No Block");
				}
				return Var().errorValue("AST_Member : No Struct");
			}
			return Var().errorValue("AST_Member : No Expression");
		}
		return Var().errorValue("AST_Member : No Name");
	}

	/* * * * * * * * * * * * * * * * * * * * */
	
	AST_System::AST_System(const List<AST_Expr *> & args)
		: AST_Expr(EX_System)
		, args(args)
	{
		for (auto a : args)
			addChild(a);
	}

	OStream & AST_System::display(OStream & out) const
	{
		out << FG::Cyan << "system" << FMT() << "(";
		for (auto it = args.begin(); it != args.end(); it++)
		{
			out << *(*it) << ((it != args.end() - 1) ? ", " : "");
		}
		return out << FMT() << ")";
	}

	Var AST_System::evaluate() const
	{
		TokenList list;
		for (auto it = args.begin(); it != args.end(); it++)
		{
			list.push_back(Token((*it)->evaluate()));
		}
		return ML_Interpreter.execCommand(list.str());
	}

	bool AST_System::run()
	{
		Var v;
		if ((v = evaluate()).isErrorType())
		{
			return Debug::logError("AST_System : {0}", v.textValue());
		}
		return runNext();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}