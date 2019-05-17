#ifndef _ML_STATE_MACHINE_HPP_
#define _ML_STATE_MACHINE_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/ITrackable.hpp>

#define ML_STATE_INVALID -1

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
		using fun_type		= typename Key(*)(Args...);
		using map_type		= typename HashMap<Key, fun_type>;
		using pair_type		= typename Pair<Key, fun_type>;
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
				if (it->second && (it->first != (static_cast<Key>(ML_STATE_INVALID))))
				{
					m_states[it->first] = it->second;
				}
			}
		}

		~StateMachine() {}

	public:
		inline fun_type get(const Key & key) const
		{
			const_iterator it;
			return ((key != (static_cast<Key>(ML_STATE_INVALID)))
				? ((((it = m_states.find(key)) != m_states.end())
					? (it->second)
					: (NULL)))
				: (NULL)
			);
		}

		inline Key run(const Key & key, Args && ... args)
		{
			fun_type fun;
			return ((key != static_cast<Key>(ML_STATE_INVALID))
				? (((fun = get(key))
					? (fun((args)...))
					: (static_cast<Key>(ML_STATE_INVALID))))
				: (static_cast<Key>(ML_STATE_INVALID))
			);
		}

	private:
		map_type m_states;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_STATE_MACHINE_HPP_
