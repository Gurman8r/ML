#include <ML/Engine/CommandDescriptor.hpp>
#include <ML/Engine/CommandRegistry.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	CommandDescriptor::CommandDescriptor()
		: m_desc()
		, m_name()
		, m_usage()
		, m_isRegistered(false)
	{
	}

	CommandDescriptor::CommandDescriptor(String const & name)
		: m_desc	()
		, m_name	(name)
		, m_usage	()
		, m_isRegistered(false)
	{
	}

	CommandDescriptor::CommandDescriptor(String const & name, String const & desc, String const & usage)
		: m_desc	(desc)
		, m_name	(name)
		, m_usage	(usage)
		, m_isRegistered(false)
	{
	}

	CommandDescriptor::CommandDescriptor(CommandDescriptor const & copy)
		: m_desc	(copy.m_desc)
		, m_name	(copy.m_name)
		, m_usage	(copy.m_usage)
		, m_isRegistered(copy.m_isRegistered)
	{
	}

	CommandDescriptor::~CommandDescriptor()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool CommandDescriptor::install(CommandRegistry & reg)
	{
		if (!m_isRegistered)
		{
			auto it{ reg.find(m_name) };
			if (it == reg.end())
			{
				reg.commands().push_back(this);
				m_isRegistered = true;
				return true;
			}
		}
		return false;
	}

	bool CommandDescriptor::uninstall(CommandRegistry & reg)
	{
		if (m_isRegistered)
		{
			auto it{ reg.find(m_name) };
			if (it != reg.end())
			{
				reg.commands().erase(it);
				m_isRegistered = false;
				return true;
			}
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	CommandDescriptor & CommandDescriptor::setDesc(String const & value)
	{
		m_desc = value; return (*this);
	}

	CommandDescriptor & CommandDescriptor::setName(String const & value)
	{
		m_name = value; return (*this);
	}

	CommandDescriptor & CommandDescriptor::setUsage(String const & value)
	{
		m_usage = value; return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}