#ifndef _ML_COMMAND_REGISTRY_HPP_
#define _ML_COMMAND_REGISTRY_HPP_

#include <ML/Engine/CommandExecutor.hpp>
#include <ML/Engine/CommandDescriptor.hpp>
#include <ML/Core/StringUtility.hpp>

#define ML_CommandRegistry ::ml::CommandRegistry::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API CommandRegistry final : public I_Singleton<CommandRegistry>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename CommandDescriptor *;
		using command_list		= typename List<value_type>;
		using iterator			= typename command_list::iterator;
		using const_iterator	= typename command_list::const_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool execute(const String & value)
		{
			if (auto args { alg::tokenize(value, " ") })
			{
				if (auto cmd { this->find_by_name(args.front()) })
				{
					return cmd->execute(args);
				}
			}
			return false;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator find(const String & name)
		{
			return std::find_if(this->begin(), this->end(), [&](auto && e)
			{
				return e && (e->getName() == name);
			});
		}

		inline const_iterator find(const String & name) const
		{
			return std::find_if(this->cbegin(), this->cend(), [&](auto && e)
			{
				return e && (e->getName() == name);
			});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline value_type find_by_name(const String & name)
		{
			iterator it = this->find(name);
			return it != this->end() ? (*it) : nullptr;
		}

		inline const value_type find_by_name(const String & name) const
		{
			const_iterator it = this->find(name);
			return it != this->cend() ? (*it) : nullptr;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto commands()			-> command_list &		{ return m_cmd; }
		inline auto commands()	const	-> const command_list & { return m_cmd; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto begin()				-> iterator				{ return m_cmd.begin(); }
		inline auto begin()		const	-> const_iterator		{ return m_cmd.begin(); }
		inline auto cbegin()	const	-> const_iterator		{ return m_cmd.cbegin(); }
		inline auto cend()		const	-> const_iterator		{ return m_cmd.cend(); }
		inline auto end()				-> iterator				{ return m_cmd.end(); }
		inline auto end()		const	-> const_iterator		{ return m_cmd.end(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend struct I_Singleton<CommandRegistry>;

		CommandRegistry() : m_cmd() {}
		~CommandRegistry() {}

		command_list m_cmd;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_COMMAND_REGISTRY_HPP_