#ifndef _ML_RUNTIME_HPP_
#define _ML_RUNTIME_HPP_

#include <ML/Script/Operator.hpp>
#include <ML/Script/Var.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define ML_Runtime ml::Runtime::getInstance()

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_SCRIPT_API Runtime final
		: public ISingleton<Runtime>
	{
		friend class ISingleton<Runtime>;

	public:
		using VarMap	= Map<String, Var *>;
		using ScopeMap	= HashMap<int32_t, VarMap*>;

	private:
		Runtime();
		~Runtime();

	public:
		static ostream & display(ostream & out);

		static void	clean();

		static bool		delVar(int32_t index, const String & name);
		static Var *	getVar(int32_t index, const String & name);
		static Var *	newVar(int32_t index, const String & name, const Var & value);
		static Var *	setVar(int32_t index, const String & name, const Var & value);

		//static const VarMap *	values(int32_t index);
		static VarMap *	values(int32_t index);
		static VarMap *	makeScope(int32_t index);
		static bool		clearScope(int32_t index);

	private:
		static ScopeMap * m_values;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RUNTIME_HPP_