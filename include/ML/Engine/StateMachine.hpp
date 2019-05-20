#ifndef _ML_STATE_MACHINE_HPP_
#define _ML_STATE_MACHINE_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/ITrackable.hpp>

#define ML_STATE_NONE -1

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class Key, class ... Args
	> class StateMachine final
		: public ITrackable
		, public INonCopyable
	{
	public:
		using key_type		= typename Key;
		using fun_type		= typename key_type(*)(Args...);
		using map_type		= typename HashMap<key_type, fun_type>;
		using pair_type		= typename Pair<key_type, fun_type>;
		using init_type		= typename Initializer<pair_type>;
		using iterator		= typename map_type::iterator;
		using const_iterator= typename map_type::const_iterator;

	public:
		StateMachine(const map_type & states)
			: m_states(states)
		{
		}

		StateMachine(const init_type & init)
			: m_states()
		{
			for (auto it = init.begin(); it != init.end(); it++)
			{
				if (it->second && (it->first != (static_cast<key_type>(ML_STATE_NONE))))
				{
					m_states[it->first] = it->second;
				}
			}
		}

	public:
		template <
			class T
		> inline fun_type operator[](const T & key)
		{
			const_iterator it;
			return ((key != (static_cast<key_type>(ML_STATE_NONE)))
				? ((((it = m_states.find(static_cast<key_type>(key))) != m_states.end())
					? (it->second)
					: (NULL)))
				: (NULL)
			);
		}

		template <
			class T
		> inline key_type operator()(const T & key, Args ... args)
		{
			static fun_type fun;
			return ((((fun = (*this)[static_cast<key_type>(key)])
				? (fun((args)...))
				: (static_cast<key_type>(ML_STATE_NONE))))
			);
		}

	private:
		map_type m_states;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_STATE_MACHINE_HPP_
