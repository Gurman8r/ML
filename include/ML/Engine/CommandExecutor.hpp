#ifndef _ML_COMMAND_EXECUTOR_HPP_
#define _ML_COMMAND_EXECUTOR_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/Newable.hpp>
#include <ML/Core/StringUtility.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct CommandDescriptor;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API CommandExecutor : public Newable
	{
		CommandExecutor() {}

		virtual ~CommandExecutor() {}

		virtual bool onCommand(const CommandDescriptor & cmd, const List<String> & args) const = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API FunctionExecutor final : public CommandExecutor
	{
		using fun_type = typename bool(*)(const CommandDescriptor &, const List<String> &);

		FunctionExecutor(fun_type && fun) : m_fun(fun) {}

		bool onCommand(const CommandDescriptor & cmd, const List<String> & args) const override;

	private: fun_type m_fun;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_COMMAND_EXECUTOR_HPP_