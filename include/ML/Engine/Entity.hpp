#ifndef _ML_ENTITY_HPP_
#define _ML_ENTITY_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/ITrackable.hpp>
#include <ML/Core/IDisposable.hpp>
#include <ML/Core/IReadable.hpp>
#include <ML/Core/IWritable.hpp>
#include <ML/Core/Preprocessor.hpp>

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API Entity final
		: public ITrackable
		, public IDisposable
		, public IReadable
		, public IWritable
		, public INonCopyable
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * */
		using value_type	= typename ITrackable *;
		using map_type		= typename HashMap<size_t, value_type>;
		using iterator		= typename map_type::iterator;
		using const_iterator= typename map_type::const_iterator;

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		Entity();
		~Entity();

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		bool dispose() override;
		bool loadFromFile(const String & filename) override;
		bool saveToFile(const String & filename) const override;

	public:
		// Find Component
		/* * * * * * * * * * * * * * * * * * * * */
		template <class Component> 
		inline iterator find()
		{
			ML_assert_is_base_of(ITrackable, Component);
			return (iterator)(m_map.find(ML_typeof(Component)));
		}

		template <class Component>
		inline const_iterator find() const
		{
			ML_assert_is_base_of(ITrackable, Component);
			return (const_iterator)(m_map.find(ML_typeof(Component)));
		}

		// Add Component
		/* * * * * * * * * * * * * * * * * * * * */
		template <class Component>
		inline Component * add()
		{
			ML_assert_is_base_of(ITrackable, Component);
			return ((this->find<Component>() == this->end())
				? (reinterpret_cast<Component *>(m_map.insert({ 
						ML_typeof(Component), 
						new Component() 
					}).first->second))
				: (NULL)
			);
		}

		template <class Component>
		inline Component * add(Component * value)
		{
			ML_assert_is_base_of(ITrackable, Component);
			return ((this->find<Component>() == this->end())
				? (reinterpret_cast<Component *>(m_map.insert({
						ML_typeof(Component),
						value
					}).first->second))
				: (NULL)
			);
		}

		//template <class Component>
		//inline Component * add(const Component & value)
		//{
		//	ML_assert_is_base_of(ITrackable, Component);
		//	return ((this->find<Component>() == this->end())
		//		? (reinterpret_cast<Component *>(m_map.insert({
		//				ML_typeof(Component),
		//				new Component(value)
		//			}).first->second))
		//		: (NULL)
		//	);
		//}

		template <
			class Component,
			class ... Args
		> inline Component * add(Args && ... args)
		{
			ML_assert_is_base_of(ITrackable, Component);
			return add<Component>(new Component(std::forward<Args>(args)...));
		}

		// Get Component
		/* * * * * * * * * * * * * * * * * * * * */
		template <class Component>
		inline Component * get()
		{
			ML_assert_is_base_of(ITrackable, Component);
			iterator it;
			return (((it = this->find<Component>()) != this->end())
				? (reinterpret_cast<Component *>(it->second))
				: (NULL)
			);
		}

		template <class Component>
		inline const Component * get() const
		{
			ML_assert_is_base_of(ITrackable, Component);
			const_iterator it;
			return (((it = this->find<Component>()) != this->cend())
				? (reinterpret_cast<const Component *>(it->second))
				: (NULL)
			);
		}

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		inline iterator			begin()				{ return m_map.begin();  }
		inline const_iterator	begin()		const	{ return m_map.begin();  }
		inline const_iterator	cbegin()	const	{ return m_map.cbegin(); }
		inline iterator			end()				{ return m_map.end();	 }
		inline const_iterator	end()		const	{ return m_map.end();	 }
		inline const_iterator	cend()		const	{ return m_map.cend();	 }

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		map_type m_map;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENTITY_HPP_