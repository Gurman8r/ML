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

	CommandImpl::CommandImpl(const String & name)
		: CommandDescriptor(name)
		, m_executor(nullptr)
	{
	}

	CommandImpl::CommandImpl(const String & name, CommandExecutor * executor)
		: CommandDescriptor(name)
		, m_executor(executor)
	{
	}

	CommandImpl::CommandImpl(const String & name, const String & desc, const String & usage)
		: CommandDescriptor(name, desc, usage)
		, m_executor(nullptr)
	{
	}

	CommandImpl::CommandImpl(const String & name, const String & desc, const String & usage, CommandExecutor * executor)
		: CommandDescriptor(name, desc, usage)
		, m_executor(executor)
	{
	}

	CommandImpl::CommandImpl(const CommandImpl & copy)
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