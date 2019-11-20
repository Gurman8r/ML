#ifndef _ML_COMMAND_EXECUTOR_HPP_
#define _ML_COMMAND_EXECUTOR_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/MemoryTracker.hpp>
#include <ML/Core/StringUtility.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct CommandDescriptor;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API CommandExecutor : public Trackable
	{
		CommandExecutor() {}

		virtual ~CommandExecutor() {}

		virtual bool onCommand(CommandDescriptor const & cmd, ArrayList<String> const & args) const = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API FunctionExecutor final : public CommandExecutor
	{
		using fun_type = typename std::function<bool(CommandDescriptor const &, ArrayList<String> const &)>;

		FunctionExecutor(fun_type const & fun)
			: m_fun(fun)
		{
		}

		inline bool onCommand(CommandDescriptor const & cmd, ArrayList<String> const & args) const override
		{
			return m_fun ? m_fun(cmd, args) : false;
		}

	private: fun_type m_fun;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_COMMAND_EXECUTOR_HPP_