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
		: public ITrackable
		, public ISingleton<Parser>
	{
		friend class ISingleton<Parser>;

	public:
		using RuleMap = HashMap<const std::type_info *, Rule *>;

	private:
		Parser();
		~Parser();

	public:
		/* * * * * * * * * * * * * * * * * * * * */

		static bool	InfixToPostfix(const TokenList & ifx, TokenList & pfx, bool show);

		TokenList infixToPostfix(const TokenList & ifx, bool show) const;

		/* * * * * * * * * * * * * * * * * * * * */

		AST_Block *	genFromList(const TokenList & value) const;
		AST_Block *	genFromTree(const TokenTree & value) const;
		AST_Node *	genNode(AST_Node * root, const TokenList & toks) const;
		AST_Stmt *	genStatement(const TokenList & toks) const;
		AST_Expr *	genExpression(const TokenList & toks) const;
		AST_BinOp *	genNestedBinOp(const TokenList & toks) const;

		/* * * * * * * * * * * * * * * * * * * * */

		List<AST_Expr *> genArrayElems(const TokenList & toks) const;
		List<AST_Expr *> genCallParams(const TokenList & toks) const;
		List<AST_Expr *> genFuncParams(const TokenList & toks) const;

		/* * * * * * * * * * * * * * * * * * * * */

	private:
		void install_statements();
		void install_expressions();

	public:
		inline bool showToks(bool value) { return m_showToks = value; }
		inline bool showTree(bool value) { return m_showTree = value; }
		inline bool showItoP(bool value) { return m_showItoP = value; }

		inline bool showToks() const { return m_showToks; }
		inline bool showTree() const { return m_showTree; }
		inline bool showItoP() const { return m_showItoP; }

	public:
		template <class T>
		inline Rule * install(INodeMaker * maker)
		{
			RuleMap::iterator it;
			if ((it = m_rules.find(&typeid(T))) == m_rules.end())
			{
				return (m_rules[&typeid(T)] = new Rule(maker));
			}
			return NULL;
		}

		template <class T>
		inline T * generate(const TokenList & toks) const
		{
			RuleMap::const_iterator it;
			if ((it = m_rules.find(&typeid(T))) != m_rules.end())
			{
				return (T *)it->second->run(toks);
			}
			return NULL;
		}

	private:
		bool m_showToks;
		bool m_showTree;
		bool m_showItoP;

		RuleMap m_rules;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PARSER_HPP_