#ifndef _ML_STATE_MACHINE_HPP_
#define _ML_STATE_MACHINE_HPP_

#include <ML/Core/IObject.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class Key, class ... Args
	> class StateMachine final
		: public IObject
		, public INonCopyable
	{
	public: // Usings
		/* * * * * * * * * * * * * * * * * * * * */
		using key_type		= typename Key;
		using self_type		= typename StateMachine<Key, Args...>;
		using fun_type		= typename key_type(*)(Args...);
		using map_type		= typename HashMap<key_type, fun_type>;
		using pair_type		= typename Pair<key_type, fun_type>;
		using init_type		= typename InitList<pair_type>;
		using const_iterator= typename map_type::const_iterator;

		static constexpr key_type NoState { static_cast<key_type>(-1) };

	public: // Constructor
		/* * * * * * * * * * * * * * * * * * * * */
		StateMachine(init_type && init)
			: m_states()
		{
			for (auto it = init.begin(); it != init.end(); it++)
			{
				if (it->second && (it->first > self_type::NoState))
				{
					m_states.insert({ it->first, it->second });
				}
			}
		}

	public: // Get State
		/* * * * * * * * * * * * * * * * * * * * */
		inline fun_type operator[](const key_type & key)
		{
			const_iterator it;
			return ((key > self_type::NoState)
				? (((it = m_states.find(key)) != m_states.end())
					? it->second
					: nullptr)
				: (nullptr)
			);
		}

	public: // Run State
		/* * * * * * * * * * * * * * * * * * * * */
		inline key_type operator()(const key_type & key, Args ... args)
		{
			fun_type fun;
			return ((fun = (*this)[key])
				? fun((args)...)
				: self_type::NoState
			);
		}

	private: // Data
		/* * * * * * * * * * * * * * * * * * * * */
		map_type m_states;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_STATE_MACHINE_HPP_
