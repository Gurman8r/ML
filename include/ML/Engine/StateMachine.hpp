#ifndef _ML_STATE_MACHINE_HPP_
#define _ML_STATE_MACHINE_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/ITrackable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class Key, class ... Args
	> class StateMachine final
		: public ITrackable
		, public INonCopyable
	{
	public: // Usings
		/* * * * * * * * * * * * * * * * * * * * */
		using key_type		= typename Key;
		using self_type		= typename StateMachine<Key, Args...>;
		using fun_type		= typename key_type(*)(Args...);
		using map_type		= typename HashMap<key_type, fun_type>;
		using pair_type		= typename Pair<key_type, fun_type>;
		using init_type		= typename Initializer<pair_type>;
		using const_iterator= typename map_type::const_iterator;

		static constexpr key_type NoState { static_cast<key_type>(-1) };

	public: // Constructors
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
		template <
			class T
		> inline fun_type operator[](const T & key)
		{
			const_iterator it;
			return ((key > self_type::NoState)
				? ((((it = m_states.find(static_cast<key_type>(key))) != m_states.end())
					? (it->second)
					: (NULL)))
				: (NULL)
			);
		}

	public: // Run State
		/* * * * * * * * * * * * * * * * * * * * */
		template <
			class T
		> inline key_type operator()(const T & key, Args ... args)
		{
			fun_type fun;
			return ((fun = (*this)[static_cast<key_type>(key)])
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
