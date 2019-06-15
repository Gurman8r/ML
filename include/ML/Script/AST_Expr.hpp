#ifndef _ML_AST_EXPR_HPP_
#define _ML_AST_EXPR_HPP_

#include <ML/Script/AST_Stmt.hpp>
#include <ML/Script/Operator.hpp>

namespace ml
{
	// Expressions
	/* * * * * * * * * * * * * * * * * * * * */
	class ML_SCRIPT_API AST_Expr : public AST_Stmt
	{
	public:
		enum : int32_t
		{
			EX_INVALID = -1,
			EX_Array,
			EX_Assign,
			EX_Bool,
			EX_Call,
			EX_Float,
			EX_Func,
			EX_Input,
			EX_Int,
			EX_Name,
			EX_BinOp,
			EX_String,
			EX_Struct,
			EX_Subscr,
			EX_Command,
			EX_SizeOf,
			EX_TypeID,
			EX_TypeName,
			EX_NodeID,
			EX_New,
			EX_Member,
			EX_System,
			MAX_EXPR_TYPE
		};

		int32_t exprType;

		AST_Expr(int32_t type);
		virtual ~AST_Expr() {}

		virtual OStream & display(OStream & out) const;

		virtual Var evaluate() const;

		virtual bool run();
	};

	// Array
	/* * * * * * * * * * * * * * * * * * * * */
	struct ML_SCRIPT_API AST_Array : public AST_Expr
	{
		List<AST_Expr *> values;

		AST_Array(const List<AST_Expr *> & values);

		OStream & display(OStream & out) const override;

		Var evaluate() const override;
	};

	// Bool
	/* * * * * * * * * * * * * * * * * * * * */
	struct ML_SCRIPT_API AST_Bool : public AST_Expr
	{
		bool value;

		AST_Bool(bool value);

		OStream & display(OStream & out) const override;

		Var evaluate() const override;
	};

	// Float
	/* * * * * * * * * * * * * * * * * * * * */
	struct ML_SCRIPT_API AST_Float : public AST_Expr
	{
		float value;

		AST_Float(float value);

		OStream & display(OStream & out) const override;

		Var evaluate() const override;
	};

	// Function
	/* * * * * * * * * * * * * * * * * * * * */
	struct ML_SCRIPT_API AST_Func : public AST_Expr
	{
		String name;
		List<AST_Expr *> args;

		AST_Func(const String & name, const List<AST_Expr *> & args);

		OStream & display(OStream & out) const override;

		Var evaluate() const override;

		bool run() override;
	};

	// Input
	/* * * * * * * * * * * * * * * * * * * * */
	struct ML_SCRIPT_API AST_Input : public AST_Expr
	{
		AST_Expr * prompt;

		AST_Input(AST_Expr * prompt);

		OStream & display(OStream & out) const override;

		Var evaluate() const override;
	};

	// Int
	/* * * * * * * * * * * * * * * * * * * * */
	struct ML_SCRIPT_API AST_Int : public AST_Expr
	{
		int32_t value;

		AST_Int(int32_t value);

		OStream & display(OStream & out) const override;

		Var evaluate() const override;
	};

	// Name
	/* * * * * * * * * * * * * * * * * * * * */
	struct ML_SCRIPT_API AST_Name : public AST_Expr
	{
		String value;

		AST_Name(const String & value);

		OStream & display(OStream & out) const override;

		Var evaluate() const override;
	};

	// Assign
	/* * * * * * * * * * * * * * * * * * * * */
	struct ML_SCRIPT_API AST_Assign : public AST_Expr
	{
		Operator	op;
		AST_Expr *	lhs;
		AST_Expr *	rhs;

		AST_Assign(Operator op, AST_Expr * lhs, AST_Expr * rhs);

		OStream & display(OStream & out) const override;

		Var evaluate() const override;

		bool run() override;
	};

	// Call
	/* * * * * * * * * * * * * * * * * * * * */
	struct ML_SCRIPT_API AST_Call : public AST_Expr
	{
		AST_Name * name;
		List<AST_Expr *> args;

		AST_Call(AST_Name * name, const List<AST_Expr *> & args);

		OStream & display(OStream & out) const override;

		Var			evaluate() const override;

		AST_Func*	getFunc() const;

		bool		run() override;
	};

	// BinOp
	/* * * * * * * * * * * * * * * * * * * * */
	struct ML_SCRIPT_API AST_BinOp : public AST_Expr
	{
		Operator op;
		AST_Expr * lhs;
		AST_Expr * rhs;

		AST_BinOp(const Operator & op, AST_Expr * lhs, AST_Expr * rhs);

		OStream & display(OStream & out) const override;

		Var evaluate() const override;
	};

	// String
	/* * * * * * * * * * * * * * * * * * * * */
	struct ML_SCRIPT_API AST_String : public AST_Expr
	{
		String value;

		AST_String(const String & value);

		OStream & display(OStream & out) const override;

		Var evaluate() const override;
	};

	// Struct
	/* * * * * * * * * * * * * * * * * * * * */
	struct ML_SCRIPT_API AST_Struct : public AST_Expr
	{
		String name;
		List<AST_Expr *> args;

		AST_Struct(const String & name, const List<AST_Expr *> & args);

		AST_Block * getBody() const;

		OStream & display(OStream & out) const override;

		Var evaluate() const override;

		bool run() override;
	};

	// Subscript
	/* * * * * * * * * * * * * * * * * * * * */
	struct ML_SCRIPT_API AST_Subscr : public AST_Expr
	{
		AST_Name * name;
		AST_Expr * index;

		AST_Subscr(AST_Name * name, AST_Expr * index);

		OStream & display(OStream & out) const override;

		Var evaluate() const override;
	};

	// Command
	/* * * * * * * * * * * * * * * * * * * * */
	struct ML_SCRIPT_API AST_Command : public AST_Expr
	{
		AST_Expr * expr;

		AST_Command(AST_Expr * expr);

		OStream & display(OStream & out) const override;

		Var evaluate() const override;

		bool run() override;
	};

	// Size Of
	/* * * * * * * * * * * * * * * * * * * * */
	struct ML_SCRIPT_API AST_SizeOf : public AST_Expr
	{
		AST_Expr * expr;

		AST_SizeOf(AST_Expr * expr);

		OStream & display(OStream & out) const override;

		Var evaluate() const override;
	};

	// Type ID
	/* * * * * * * * * * * * * * * * * * * * */
	struct ML_SCRIPT_API AST_TypeID : public AST_Expr
	{
		AST_Expr * expr;
		
		AST_TypeID(AST_Expr * expr);
		
		OStream & display(OStream & out) const override;

		Var evaluate() const override;
	};

	// Type Name
	/* * * * * * * * * * * * * * * * * * * * */
	struct ML_SCRIPT_API AST_TypeName : public AST_Expr
	{
		AST_Expr * expr;

		AST_TypeName(AST_Expr * expr);

		OStream & display(OStream & out) const override;

		Var evaluate() const override;
	};

	// Node ID
	/* * * * * * * * * * * * * * * * * * * * */
	struct ML_SCRIPT_API AST_NodeID : public AST_Expr
	{
		AST_Expr * expr;

		AST_NodeID(AST_Expr * expr);

		OStream & display(OStream & out) const override;

		Var evaluate() const override;
	};

	// New
	/* * * * * * * * * * * * * * * * * * * * */
	struct ML_SCRIPT_API AST_New : public AST_Expr
	{
		AST_Expr * expr;

		AST_New(AST_Expr * expr);

		OStream & display(OStream & out) const override;

		Var evaluate() const override;
	};

	// Member
	/* * * * * * * * * * * * * * * * * * * * */
	struct ML_SCRIPT_API AST_Member : public AST_Expr
	{
		AST_Name * name;
		AST_Name * expr;

		AST_Member(AST_Name * name, AST_Name * expr);

		AST_Struct * getStruct() const;

		OStream & display(OStream & out) const override;

		Var evaluate() const override;
	};

	// System
	/* * * * * * * * * * * * * * * * * * * * */
	struct ML_SCRIPT_API AST_System : public AST_Expr
	{
		List<AST_Expr *> args;

		AST_System(const List<AST_Expr *> & args);

		OStream & display(OStream & out) const override;

		Var			evaluate() const override;

		bool		run() override;
	};
}

#endif // !_ML_AST_EXPR_HPP_