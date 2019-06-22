#ifndef _ML_PARSER_HPP_
#define _ML_PARSER_HPP_

#include <ML/Script/Rule.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define ML_Parser ml::Parser::getInstance()

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_SCRIPT_API Parser final
		: public I_Singleton<Parser>
	{
		friend struct I_Singleton<Parser>;

	public:
		using rule_map = HashMap<const std::type_info *, Rule *>;

	private:
		Parser();
		~Parser();

	public:
		/* * * * * * * * * * * * * * * * * * * * */

		static bool	infixToPostfix(const TokenList & ifx, TokenList & pfx);
		static TokenList infixToList(const TokenList & ifx);

		/* * * * * * * * * * * * * * * * * * * * */

		static AST_Block *	genFromList(const TokenList & value);
		static AST_Block *	genFromTree(const TokenTree & value);
		static AST_Node *	genNode(AST_Node * root, const TokenList & toks);
		static AST_Stmt *	genStatement(const TokenList & toks);
		static AST_Expr *	genExpression(const TokenList & toks);
		static AST_BinOp *	genNestedBinOp(const TokenList & toks);

		/* * * * * * * * * * * * * * * * * * * * */

		static List<AST_Expr *> genArrayElems(const TokenList & toks);
		static List<AST_Expr *> genCallParams(const TokenList & toks);
		static List<AST_Expr *> genFuncParams(const TokenList & toks);

		/* * * * * * * * * * * * * * * * * * * * */

	public:
		template <class T>
		inline Rule * install(INodeMaker * maker)
		{
			rule_map::iterator it;
			if ((it = m_rules.find(&typeid(T))) == m_rules.end())
			{
				return (m_rules[&typeid(T)] = new Rule(maker));
			}
			return nullptr;
		}

		template <class T>
		inline T * generate(const TokenList & toks) const
		{
			rule_map::const_iterator it;
			if ((it = m_rules.find(&typeid(T))) != m_rules.end())
			{
				return (T *)it->second->run(toks);
			}
			return nullptr;
		}

	private:
		rule_map m_rules;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PARSER_HPP_