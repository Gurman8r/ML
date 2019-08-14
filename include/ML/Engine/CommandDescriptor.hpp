#ifndef _ML_COMMAND_DESCRIPTOR_HPP_
#define _ML_COMMAND_DESCRIPTOR_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/Input.hpp>
#include <ML/Core/List.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct CommandRegistry;


	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API CommandDescriptor : public I_Newable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		CommandDescriptor();
		CommandDescriptor(const String & name);
		CommandDescriptor(const String & name, const String & desc, const String & usage);
		CommandDescriptor(const CommandDescriptor & copy);
		virtual ~CommandDescriptor();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual bool execute(const List<String> & args) const = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool install(CommandRegistry & reg);
		bool uninstall(CommandRegistry & reg);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		CommandDescriptor & setDesc(const String & value);
		CommandDescriptor & setName(const String & value);
		CommandDescriptor & setUsage(const String & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto getDesc()		const -> const String & { return m_desc; }
		inline auto getName()		const -> const String & { return m_name; }
		inline auto getUsage()		const -> const String & { return m_usage; }
		inline bool isRegistered()	const					{ return m_isRegistered; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: 
		String	m_desc;
		String	m_name;
		String	m_usage;
		bool	m_isRegistered;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	inline ML_SERIALIZE(Ostream & out, const CommandDescriptor & value)
	{
		return out
			<< value.getDesc() << " "
			<< value.getName() << " "
			<< value.getUsage() << " ";
	}

	inline ML_DESERIALIZE(Istream & in, CommandDescriptor & value)
	{
		value.setDesc(input<String>()(in));
		value.setName(input<String>()(in));
		value.setUsage(input<String>()(in));
		return in;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_COMMAND_DESCRIPTOR_HPP_