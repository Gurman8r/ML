#ifndef _ML_RULE_HPP_
#define _ML_RULE_HPP_

#include <ML/Script/TokenList.hpp>
#include <ML/Script/AST_Expr.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_SCRIPT_API INodeMaker : public INewable
	{
	public:
		virtual ~INodeMaker() {}

		virtual AST_Node * run(const TokenList & toks) const = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * */

	template <
		typename Ret,
		typename Arg  = const TokenList &,
		typename Fun = Ret * (*)(Arg)
	>
	class NodeMaker : public INodeMaker
	{
		Fun m_fun;

	public:
		NodeMaker()
			: m_fun(nullptr)
		{
		}
		NodeMaker(Fun fun)
			: m_fun(fun)
		{
		}
		~NodeMaker() {}

		inline Ret * run(Arg toks) const override 
		{ 
			return ((m_fun) ? (m_fun(toks)) : ((Ret *)nullptr));
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	class ML_SCRIPT_API Rule final : public INewable
	{
	public:
		Rule() 
			: m_maker(nullptr)
		{
		}
		Rule(INodeMaker * maker)
			: m_maker(maker)
		{
		}
		~Rule() 
		{
			if (m_maker)
			{
				delete m_maker;
				m_maker = nullptr;
			}
		}

		inline AST_Node * run(const TokenList & toks) const
		{
			return ((m_maker) ? (m_maker->run(toks)) : (nullptr));
		}

		inline AST_Node * operator()(const TokenList & toks) const
		{
			return run(toks);
		}

	private:
		INodeMaker * m_maker;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RULE_HPP_