#ifndef _ML_CONTENT_MANAGER_HPP_
#define _ML_CONTENT_MANAGER_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Engine/Metadata.hpp>
#include <ML/Engine/Registry.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Bank of shared resources.
	// Anything can be stored in Content as long as it derives Trackable.
	struct ML_ENGINE_API ContentManager final : public Trackable, public NonCopyable, public Disposable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using AssetMap	= typename Map<String, ptr_t<Trackable>>;
		using TypeMap	= typename HashMap<hash_t, AssetMap>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		ContentManager() : m_data{} {}

		~ContentManager() { this->dispose(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override
		{
			for (auto & types : m_data)
			{
				for (auto & elem : types.second)
				{
					if (elem.second) { delete elem.second; }
				}
				types.second.clear();
			}
			m_data.clear();
			return m_data.empty();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		inline AssetMap & data(hash_t code)
		{
			auto it{ m_data.find(code) };
			return ((it != m_data.end())
				? it->second
				: m_data.insert({ code, {} }).first->second
			);
		}

		template <hash_t H> inline AssetMap & data()
		{
			static auto & temp { this->data(H) };
			return temp;
		}

		template <class T> inline AssetMap & data()
		{
			return this->data<typeof<T>::hash>();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		inline const AssetMap & data(hash_t code) const
		{
			auto it{ m_data.find(code) };
			return ((it != m_data.end())
				? it->second
				: m_data.insert({ code, {} }).first->second
			);
		}

		template <hash_t H> inline const AssetMap & data() const
		{
			static const auto & temp { this->data(H) };
			return temp;
		}

		template <class T> inline const AssetMap & data() const
		{
			return this->data<typeof<T>::hash>();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline ptr_t<Trackable> insert(hash_t code, const String & name, voidptr_t value)
		{
			return this->data(code).insert({ name, (ptr_t<Trackable>)value }).first->second;
		}

		template <hash_t H> inline ptr_t<Trackable> insert(const String & name, voidptr_t value)
		{
			return this->insert(H, name, value);
		}

		template <class T> inline ptr_t<T> insert(const String & name, ptr_t<T> value)
		{
			return (ptr_t<T>)this->insert<typeof<T>::hash>(name, value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T, class ... A> inline ptr_t<T> create(const String & name, A && ... args)
		{
			return (!this->exists<T>(name)
				? this->insert(name, new T { std::forward<A>(args)... })
				: nullptr
			);
		}

		inline ptr_t<Trackable> generate(const String & type, const String & name)
		{
			if (const hash_t * code{ ML_Registry.get_code(type) })
			{
				if (!this->exists(*code, name))
				{
					return this->insert(*code, name, ML_Registry.generate(*code));
				}
			}
			return nullptr;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool destroy(hash_t code, const String & name)
		{
			auto it{ this->data(code).find(name) };
			if (it != this->data(code).end())
			{
				ptr_t<Trackable> & ptr{ it->second };
				delete ptr;
				this->data(code).erase(it);
				return true;
			}
			return false;
		}

		template <hash_t H> inline bool destroy(const String & name)
		{
			return this->destroy(H, name);
		}

		template <class T> inline bool destroy(const String & name)
		{
			return this->destroy<typeof<T>::hash>(name);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool exists(hash_t code, const String & name) const
		{
			return (this->data(code).find(name) != this->data(code).cend());
		}

		template <hash_t H> inline bool exists(const String & name) const
		{
			return this->exists(H, name);
		}

		template <class T> inline bool exists(const String & name) const
		{
			return this->exists<typeof<T>::hash>(name);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> inline AssetMap::iterator find(const String & name)
		{
			return name ? this->data<T>().find(name) : this->end<T>();
		}

		template <class T> inline AssetMap::const_iterator find(const String & name) const
		{
			return name ? this->data<T>().find(name) : this->end<T>();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> inline ptr_t<T> get(const String & name)
		{
			auto it { this->find<T>(name) };
			return ((it != this->end<T>()) ? (ptr_t<T>)it->second : nullptr);
		}

		template <class T> inline const_ptr_t<T> get(const String & name) const
		{
			auto it { this->find<T>(name) };
			return ((it != this->end<T>()) ? (const_ptr_t<T>)it->second : nullptr);
		}

		template <class T> inline const_ptr_t<T> cget(const String & name) const
		{
			return this->get<T>(name);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> inline List<String> get_keys() const
		{
			List<String> temp{};
			temp.reserve(this->size<T>());
			for (const auto & pair : this->data<T>())
			{
				temp.push_back(pair.first);
			}
			return temp;
		}

		template <class T> inline String get_name(const_ptr_t<T> value) const
		{
			const int32_t i { this->get_index_of<T>(value) };
			return (i >= 0) ? this->get_keys<T>()[(hash_t)i] : String();
		}

		template <class T> inline AssetMap::const_iterator get_iter_at_index(int32_t index) const
		{
			if ((index >= 0) && ((hash_t)index < this->size<T>()))
			{
				auto it { this->begin<T>() };
				for (int32_t i = 0; i < index; i++)
				{
					if ((++it) == this->end<T>()) { break; }
				}
				return it;
			}
			return this->end<T>();
		}

		template <class T> inline const_ptr_t<T> find_by_index(int32_t index) const
		{
			auto it { this->get_iter_at_index<T>(index) };
			return ((it != this->end<T>()) ? (const_ptr_t<T>)it->second : nullptr);
		}

		template <class T> inline int32_t get_index_of(const_ptr_t<T> value) const
		{
			if (value)
			{
				int32_t index { 0 };
				for (const auto & [ name, ptr ] : this->data<T>())
				{
					if (ptr == value)
					{
						return index;
					}
					index++;
				}
			}
			return -1;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T>	inline auto begin()			{ return this->begin<typeof<T>::hash>(); }
		template <class T>	inline auto begin()	const	{ return this->begin<typeof<T>::hash>(); }
		template <hash_t H> inline auto begin()			{ return this->data(H).begin(); }
		template <hash_t H> inline auto begin()	const	{ return this->data(H).begin(); }
		template <class T>	inline auto end()			{ return this->end<typeof<T>::hash>(); }
		template <class T>	inline auto end()	const	{ return this->end<typeof<T>::hash>(); }
		template <hash_t H> inline auto end()			{ return this->data(H).end(); }
		template <hash_t H> inline auto end()	const	{ return this->data(H).end(); }
		template <class T>	inline auto size()	const	{ return this->size<typeof<T>::hash>(); }
		template <hash_t H> inline auto size()	const	{ return this->data(H).size(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: mutable TypeMap m_data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CONTENT_MANAGER_HPP_