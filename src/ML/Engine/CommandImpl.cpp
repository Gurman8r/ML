#include <ML/Engine/CommandImpl.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	CommandImpl::CommandImpl()
		: CommandDescriptor()
		, m_executor(nullptr)
	{
	}

	CommandImpl::CommandImpl(String const & name)
		: CommandDescriptor(name)
		, m_executor(nullptr)
	{
	}

	CommandImpl::CommandImpl(String const & name, CommandExecutor * executor)
		: CommandDescriptor(name)
		, m_executor(executor)
	{
	}

	CommandImpl::CommandImpl(String const & name, String const & desc, String const & usage)
		: CommandDescriptor(name, desc, usage)
		, m_executor(nullptr)
	{
	}

	CommandImpl::CommandImpl(String const & name, String const & desc, String const & usage, CommandExecutor * executor)
		: CommandDescriptor(name, desc, usage)
		, m_executor(executor)
	{
	}

	CommandImpl::CommandImpl(CommandImpl const & copy)
		: CommandDescriptor(copy)
		, m_executor(copy.m_executor)
	{
	}

	CommandImpl::~CommandImpl()
	{
		if (m_executor) { delete m_executor; }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool CommandImpl::execute(const List<String>& args) const
	{
		return m_executor && m_executor->onCommand((*this), args);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	CommandImpl & CommandImpl::setExecutor(CommandExecutor * value)
	{
		if (!m_executor)
		{
			m_executor = value;
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}