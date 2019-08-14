#ifndef _ML_COMMAND_IMPL_HPP_
#define _ML_COMMAND_IMPL_HPP_

#include <ML/Engine/CommandDescriptor.hpp>
#include <ML/Engine/CommandExecutor.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */
	
	struct ML_ENGINE_API CommandImpl : public CommandDescriptor
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		CommandImpl();
		CommandImpl(const String & name);
		CommandImpl(const String & name, const String & desc, const String & usage);
		CommandImpl(const String & name, const String & desc, const String & usage, CommandExecutor * executor);
		CommandImpl(const CommandImpl & copy);
		virtual ~CommandImpl();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool execute(const List<String> & args) const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		CommandImpl & setExecutor(CommandExecutor * value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto getExecutor() const -> const CommandExecutor * { return m_executor; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		CommandExecutor * m_executor;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_COMMAND_IMPL_HPP_