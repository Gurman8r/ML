#ifndef _ML_RENDER_STATES_HPP_
#define _ML_RENDER_STATES_HPP_

#include <ML/Graphics/RenderVar.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API RenderStates final
		: public ITrackable
	{
	public:
		using pair_type			= typename Pair<uint32_t, RenderVar>;
		using init_type			= typename Initializer<pair_type>;
		using map_type			= typename HashMap<uint32_t, RenderVar>;
		using iterator			= typename map_type::iterator;
		using const_iterator	= typename map_type::const_iterator;

	public:
		RenderStates();
		RenderStates(const init_type & init);
		RenderStates(const map_type & flags);
		RenderStates(const RenderStates & copy);
		~RenderStates();

	public:
		void apply() const;
		bool apply(const uint32_t key, const RenderVar & value) const;
		
	public:
		inline iterator			begin()			{ return m_map.begin();	}
		inline const_iterator	begin()	const	{ return m_map.begin();	}
		inline const_iterator	cbegin()const	{ return m_map.cbegin();}
		inline iterator			end()			{ return m_map.end();	}
		inline const_iterator	end()	const	{ return m_map.end();	}
		inline const_iterator	cend()	const	{ return m_map.cend();	}

	public:
		inline const RenderVar	& operator[](const uint32_t key) const	{ return m_map.at(key); }
		inline RenderVar		& operator[](const uint32_t key)		{ return m_map[key];	}

	private:
		map_type m_map;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDER_STATES_HPP_