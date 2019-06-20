#include <ML/Script/Runtime.hpp>
#include <ML/Script/Interpreter.hpp>
#include <ML/Script/ScriptMacros.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Runtime::ScopeMap * Runtime::m_values = nullptr;

	/* * * * * * * * * * * * * * * * * * * * */

	Runtime::Runtime()
	{
		m_values = new ScopeMap();
		makeScope(0);
	}

	Runtime::~Runtime()
	{
		clean();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	ostream & Runtime::display(ostream & out)
	{
		out << FG::White << "Memory:"
			<< ml::endl;

		ScopeMap::const_iterator scope;
		for (scope = m_values->begin(); scope != m_values->end(); scope++)
		{
			const int32_t& index = scope->first;

			out << FG::Yellow << "\tScope [" << index << "]"
				<< ml::endl;

			if (const VarMap* varMap = scope->second)
			{
				VarMap::const_iterator vp;
				for (vp = varMap->begin(); vp != varMap->end(); vp++)
				{
					out << "\t\t"
						<< FG::White << vp->first << " = " << *vp->second
						<< ml::endl;
				}
			}
		}

		return out;
	}


	void Runtime::clean()
	{
		for (auto scopePair : (*m_values))
		{
			for (auto varPair : (*scopePair.second))
			{
				if (varPair.second)
				{
					delete varPair.second;
				}
			}

			scopePair.second->clear();

			delete scopePair.second;
		}
	}


	bool Runtime::delVar(int32_t index, const String & name)
	{
		if (values(index))
		{
			VarMap::const_iterator it = values(index)->find(name);

			if (it != values(index)->end())
			{
				delete it->second;

				values(index)->erase(it);

				return true;
			}
		}
		return false;
	}

	Var * Runtime::getVar(int32_t index, const String & name)
	{
		if (values(index))
		{
			VarMap::const_iterator it = values(index)->find(name);
			if (it != values(index)->end())
			{
				return it->second;
			}
		}
		return nullptr;
	}

	Var * Runtime::newVar(int32_t index, const String & name, const Var & value)
	{
		if (values(index) || makeScope(index))
		{
			VarMap::const_iterator it = values(index)->find(name);

			if (it == values(index)->end())
			{
				Var * v = new Var(value);

				v->setScope(index);

				values(index)->insert({ name, v });

				return v;
			}
		}
		return nullptr;
	}

	Var * Runtime::setVar(int32_t index, const String & name, const Var & value)
	{
		if (Var * v = getVar(index, name))
		{
			v->Set(value);

			return v;
		}
		return newVar(index, name, value);
	}


	//const Runtime::VarMap * Runtime::values(int32_t index) const
	//{
	//	ScopeMap::const_iterator it = m_values->find(index);
	//	if (it != m_values->end())
	//	{
	//		return it->second;
	//	}
	//	return nullptr;
	//}

	Runtime::VarMap * Runtime::values(int32_t index)
	{
		ScopeMap::iterator it = m_values->find(index);
		if (it != m_values->end())
		{
			return it->second;
		}
		return nullptr;
	}


	Runtime::VarMap * Runtime::makeScope(int32_t index)
	{
		if (!values(index))
		{
			m_values->insert({ index, new VarMap() });

			setVar(index, ML_SCR_NAME_RETV, Var().voidValue());
		}
		return values(index);
	}

	bool Runtime::clearScope(int32_t index)
	{
		if (VarMap * vars = values(index))
		{
			for (auto & pair : (*vars))
			{
				delete pair.second;
			}

			m_values->erase(index);

			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}