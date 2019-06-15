#ifndef _ML_GAME_RULE_HPP_
#define _ML_GAME_RULE_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/IObject.hpp>
#include <ML/Core/List.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	template <class T>
	class GameRule final
		: public IObject
	{
	public:
		using value_type = typename T;
		using name_type = typename String;
		using self_type = typename GameRule<value_type>;

	private:
		String		m_name;
		value_type	m_value;

	public:
		template <class ... Args>
		GameRule(const String & name, Args && ... args)
			: m_name(name)
			, m_value(std::forward<Args>(args)...)
		{
		}

		GameRule(const GameRule & copy)
			: GameRule(copy.name, copy.value)
		{
		}
		
		~GameRule() {}

	public:
		inline const String		& getName() const { return m_name; }
		inline const value_type & getValue() const { return m_value; }

	public:
		inline value_type	operator*() const	{ return m_value; }
		inline value_type * operator->()		{ return &m_value; }

	public:
		inline void serialize(OStream & out) const override {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_GAME_RULE_HPP_