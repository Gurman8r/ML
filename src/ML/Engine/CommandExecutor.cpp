#include <ML/Engine/CommandExecutor.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool FunctionExecutor::onCommand(const CommandDescriptor & cmd, const List<String> & args) const
	{
		return m_fun ? m_fun(cmd, args) : false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}