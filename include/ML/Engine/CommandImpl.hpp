#ifndef _ML_COMMAND_IMPL_HPP_
#define _ML_COMMAND_IMPL_HPP_

#include <ML/Engine/CommandDescriptor.hpp>
#include <ML/Engine/CommandExecutor.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */
	
	struct ML_ENGINE_API CommandImpl : public CommandDescriptor
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		CommandImpl();
		CommandImpl(String const & name);
		CommandImpl(String const & name, CommandExecutor * executor);
		CommandImpl(String const & name, String const & desc, String const & usage);
		CommandImpl(String const & name, String const & desc, String const & usage, CommandExecutor * executor);
		CommandImpl(CommandImpl const & copy);
		virtual ~CommandImpl();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool execute(ArrayList<String> const & args) const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		CommandImpl & setExecutor(CommandExecutor * value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto getExecutor() const -> CommandExecutor const * { return m_executor; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: CommandExecutor * m_executor;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_COMMAND_IMPL_HPP_