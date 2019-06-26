#ifndef _ML_AST_STMT_HPP_
#define _ML_AST_STMT_HPP_

#include <ML/Script/AST_Block.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class  AST_Expr;
	struct AST_Assign;
	struct AST_Bool;
	struct AST_Call;
	struct AST_Float;
	struct AST_Func;
	struct AST_Int;
	struct AST_Name;
	struct AST_Call;
	struct AST_BinOp;
	struct AST_String;

	// Statements
	/* * * * * * * * * * * * * * * * * * * * */
	class ML_SCRIPT_API AST_Stmt : public AST_Node
	{
	public:
		enum : int32_t
		{
			ST_INVALID = -1,
			ST_Elif,
			ST_Else,
			ST_Expr,
			ST_For,
			ST_Delete,
			ST_If,
			ST_Include,
			ST_Print,
			ST_Return,
			ST_While,
			MAX_STMT_TYPE
		};

		int32_t stmtType;

	public:
		AST_Stmt(int32_t stmt);
		virtual ~AST_Stmt() {}

		virtual Ostream & display(Ostream & out) const;

		AST_Block *	block() const;
		virtual bool run() = 0;
	};

	// Elif
	/* * * * * * * * * * * * * * * * * * * * */
	struct ML_SCRIPT_API AST_Elif : public AST_Stmt
	{
		AST_Expr * expr;

		AST_Elif(AST_Expr * expr);
		~AST_Elif() {}

		Ostream & display(Ostream & out) const override;
		bool run() override;
	};

	// Else
	/* * * * * * * * * * * * * * * * * * * * */
	struct ML_SCRIPT_API AST_Else : public AST_Stmt
	{
		AST_Else();
		~AST_Else() {}

		Ostream & display(Ostream & out) const override;
		bool run() override;
	};

	// For
	/* * * * * * * * * * * * * * * * * * * * */
	struct ML_SCRIPT_API AST_For : public AST_Stmt
	{
		AST_Expr * assn;
		AST_Expr * expr;
		AST_Expr * stmt;

		AST_For(AST_Expr * assn, AST_Expr * expr, AST_Expr * stmt);
		~AST_For() {}

		Ostream & display(Ostream & out) const override;
		bool run() override;
	};

	// Free
	/* * * * * * * * * * * * * * * * * * * * */
	struct ML_SCRIPT_API AST_Delete : public AST_Stmt
	{
		AST_Name * name;

		AST_Delete(AST_Name * name);
		~AST_Delete() {}

		Ostream & display(Ostream & out) const override;
		bool run() override;
	};

	// If
	/* * * * * * * * * * * * * * * * * * * * */
	struct ML_SCRIPT_API AST_If : public AST_Stmt
	{
		AST_Expr * expr;

		AST_If(AST_Expr * expr);
		~AST_If() {}

		Ostream & display(Ostream & out) const override;
		bool run() override;
	};

	// Include
	/* * * * * * * * * * * * * * * * * * * * */
	struct ML_SCRIPT_API AST_Include : public AST_Stmt
	{
		AST_Expr * expr;
		
		AST_Include(AST_Expr * expr);
		~AST_Include() {}

		Ostream & display(Ostream & out) const override;
		bool run() override;
	};

	// Print
	/* * * * * * * * * * * * * * * * * * * * */
	struct ML_SCRIPT_API AST_Print : public AST_Stmt
	{
		AST_Expr * expr;
		bool newl;

		AST_Print(AST_Expr * expr, bool newl);
		~AST_Print() {}

		Ostream & display(Ostream & out) const override;
		bool run() override;
	};

	// Return
	/* * * * * * * * * * * * * * * * * * * * */
	struct ML_SCRIPT_API AST_Return : public AST_Stmt
	{
		AST_Expr * expr;

		AST_Return(AST_Expr * expr);
		~AST_Return() {}

		Ostream & display(Ostream & out) const override;
		bool run() override;
	};

	// While
	/* * * * * * * * * * * * * * * * * * * * */
	struct ML_SCRIPT_API AST_While : public AST_Stmt
	{
		AST_Expr * expr;

		AST_While(AST_Expr * expr);
		~AST_While() {}

		Ostream & display(Ostream & out) const override;
		bool run() override;
	};
}

#endif // !_ML_AST_STMT_HPP_