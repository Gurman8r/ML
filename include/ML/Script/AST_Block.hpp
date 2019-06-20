#ifndef _ML_AST_BLOCK_HPP_
#define _ML_AST_BLOCK_HPP_

#include <ML/Script/AST_Node.hpp>
#include <ML/Script/Var.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct AST_Func;
	struct AST_Struct;

	// Block
	/* * * * * * * * * * * * * * * * * * * * */
	class ML_SCRIPT_API AST_Block : public AST_Node
	{
	public:
		using FuncTable = HashMap<String, AST_Func*>;
		using StructTable = HashMap<String, AST_Struct *>;

	public:
		AST_Block();
		AST_Block(const List<AST_Node *> & body);
		~AST_Block();

		virtual ostream & display(ostream & out) const;

		bool	delVar(const String & name);
		Var *	getVar(const String & name) const;
		Var *	newVar(const String & name, const Var & value);
		Var *	setVar(const String & name, const Var & value);

		Var *	getRet() const;
		Var *	setRet(const Var & value) const;

		bool		addFunc(const String & name, AST_Func * value);
		AST_Func *	getFunc(const String & name);
		FuncTable	getFuncs() const;

		bool		addStruct(const String & name, AST_Struct * value);
		AST_Struct *getStruct(const String & name);
		StructTable getStructs() const;

		AST_Block *	block() const;

		Var	 evaluate() const;
		bool run() override;
		bool runFirst();

	private:
		FuncTable	m_funcs;
		StructTable m_structs;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_AST_BLOCK_HPP_