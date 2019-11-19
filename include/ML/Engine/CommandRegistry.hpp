#ifndef _ML_COMMAND_REGISTRY_HPP_
#define _ML_COMMAND_REGISTRY_HPP_

#include <ML/Core/Disposable.hpp>
#include <ML/Core/StringUtility.hpp>
#include <ML/Engine/CommandExecutor.hpp>
#include <ML/Engine/CommandDescriptor.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API CommandRegistry final : public Trackable, public NonCopyable, public Disposable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename CommandDescriptor *;
		using command_list		= typename ArrayList<value_type>;
		using iterator			= typename command_list::iterator;
		using const_iterator	= typename command_list::const_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		CommandRegistry() : m_cmd() {}

		~CommandRegistry() {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool dispose() override
		{
			for (auto & elem : m_cmd)
			{
				if (elem) { delete elem; }
			}
			m_cmd.clear();
			return true;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool execute(String const & value)
		{
			auto args { util::tokenize(value, " ") };
			if (!args.empty())
			{
				if (auto cmd { this->find_by_name(args.front()) })
				{
					return cmd->execute(args);
				}
			}
			return false;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator find(String const & name)
		{
			return std::find_if(this->begin(), this->end(), [&](auto && e)
			{
				return e && (e->getName() == name);
			});
		}

		inline const_iterator find(String const & name) const
		{
			return std::find_if(this->cbegin(), this->cend(), [&](auto && e)
			{
				return e && (e->getName() == name);
			});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline value_type find_by_name(String const & name)
		{
			iterator it = this->find(name);
			return it != this->end() ? (*it) : nullptr;
		}

		inline const value_type find_by_name(String const & name) const
		{
			const_iterator it = this->find(name);
			return it != this->cend() ? (*it) : nullptr;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto commands()			-> command_list &		{ return m_cmd; }
		inline auto commands()	const	-> command_list const & { return m_cmd; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto begin()				-> iterator				{ return m_cmd.begin(); }
		inline auto begin()		const	-> const_iterator		{ return m_cmd.begin(); }
		inline auto cbegin()	const	-> const_iterator		{ return m_cmd.cbegin(); }
		inline auto cend()		const	-> const_iterator		{ return m_cmd.cend(); }
		inline auto end()				-> iterator				{ return m_cmd.end(); }
		inline auto end()		const	-> const_iterator		{ return m_cmd.end(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		command_list m_cmd;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_COMMAND_REGISTRY_HPP_