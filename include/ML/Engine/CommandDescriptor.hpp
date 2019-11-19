#ifndef _ML_COMMAND_DESCRIPTOR_HPP_
#define _ML_COMMAND_DESCRIPTOR_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/MemoryTracker.hpp>
#include <ML/Core/Input.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct CommandRegistry;

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API CommandDescriptor : public Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		CommandDescriptor();
		CommandDescriptor(String const & name);
		CommandDescriptor(String const & name, String const & desc, String const & usage);
		CommandDescriptor(CommandDescriptor const & copy);
		virtual ~CommandDescriptor();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual bool execute(const List<String> & args) const = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool install(CommandRegistry & reg);
		bool uninstall(CommandRegistry & reg);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		CommandDescriptor & setDesc(String const & value);
		CommandDescriptor & setName(String const & value);
		CommandDescriptor & setUsage(String const & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto getDesc()		const -> String const & { return m_desc; }
		inline auto getName()		const -> String const & { return m_name; }
		inline auto getUsage()		const -> String const & { return m_usage; }
		inline auto isRegistered()	const -> bool const &	{ return m_isRegistered; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: 
		String	m_desc;
		String	m_name;
		String	m_usage;
		bool	m_isRegistered;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	inline ML_SERIALIZE(std::ostream & out, CommandDescriptor const & value)
	{
		return out
			<< value.getName() << ": " << value.getUsage() << std::endl
			<< value.getDesc();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_COMMAND_DESCRIPTOR_HPP_