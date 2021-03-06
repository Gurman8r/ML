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
	struct ML_ENGINE_API ContentManager final : public Trackable, public NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using AssetTable = typename std::map<String, Trackable *>;

		using Catagories = typename std::map<hash_t, AssetTable>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose()
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
		
		inline AssetTable & data(hash_t code)
		{
			if (auto it{ m_data.find(code) }; it != m_data.end())
			{
				return it->second;
			}
			return m_data.insert({ code, AssetTable{} }).first->second;
		}

		template <hash_t H> inline AssetTable & data()
		{
			static auto & temp { this->data(H) };
			return temp;
		}

		template <class T> inline AssetTable & data()
		{
			return this->data<typeof<T>::hash()>();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		inline AssetTable const & data(hash_t code) const
		{
			if (auto it{ m_data.find(code) }; it != m_data.cend())
			{
				return it->second;
			}
			return m_data.insert({ code, AssetTable{} }).first->second;
		}

		template <hash_t H> inline AssetTable const & data() const
		{
			static auto const & temp { this->data(H) };
			return temp;
		}

		template <class T> inline AssetTable const & data() const
		{
			return this->data<typeof<T>::hash()>();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline Trackable * insert(hash_t code, String const & name, void * value)
		{
			return this->data(code).insert({ name, (Trackable *)value }).first->second;
		}

		template <hash_t H> inline Trackable * insert(String const & name, void * value)
		{
			return this->insert(H, name, value);
		}

		template <class T> inline T * insert(String const & name, T * value)
		{
			return (T *)this->insert<typeof<T>::hash()>(name, value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T, class ... A> inline T * create(String const & name, A && ... args)
		{
			return (!this->exists<T>(name)
				? this->insert(name, new T { std::forward<A>(args)... })
				: nullptr
			);
		}

		inline Trackable * generate(String const & type, String const & name)
		{
			if (hash_t const * code{ ML_Registry.get_code(type) })
			{
				if (!this->exists(*code, name))
				{
					return this->insert(*code, name, ML_Registry.generate(*code));
				}
			}
			return nullptr;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool destroy(hash_t code, String const & name)
		{
			if (auto it{ this->data(code).find(name) }; it != this->data(code).end())
			{
				Trackable *& ptr{ it->second };
				delete ptr;
				this->data(code).erase(it);
				return true;
			}
			return false;
		}

		template <hash_t H> inline bool destroy(String const & name)
		{
			return this->destroy(H, name);
		}

		template <class T> inline bool destroy(String const & name)
		{
			return this->destroy<typeof<T>::hash()>(name);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool exists(hash_t code, String const & name) const
		{
			return (this->data(code).find(name) != this->data(code).cend());
		}

		template <hash_t H> inline bool exists(String const & name) const
		{
			return this->exists(H, name);
		}

		template <class T> inline bool exists(String const & name) const
		{
			return this->exists<typeof<T>::hash()>(name);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> inline T * get(String const & name)
		{
			if (auto it{ this->data<T>().find(name) }; it != this->end<T>())
			{
				return static_cast<T *>(it->second);
			}
			return nullptr;
		}

		template <class T> inline T const * get(String const & name) const
		{
			if (auto it{ this->data<T>().find(name) }; it != this->cend<T>())
			{
				return static_cast<T const *>(it->second);
			}
			return nullptr;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> inline std::vector<String> get_keys() const
		{
			std::vector<String> temp{};
			temp.reserve(this->size<T>());
			for (auto const & pair : this->data<T>())
			{
				temp.emplace_back(pair.first);
			}
			return temp;
		}

		template <class T> inline String get_name(T const * value) const
		{
			const int32_t i { this->get_index_of<T>(value) };
			return (i >= 0) ? this->get_keys<T>()[static_cast<hash_t>(i)] : String();
		}

		template <class T> inline AssetTable::const_iterator get_iter_at_index(int32_t index) const
		{
			if ((index >= 0) && (static_cast<hash_t>(index) < this->size<T>()))
			{
				auto it { this->cbegin<T>() };
				for (int32_t i = 0; i < index; i++)
				{
					if ((++it) == this->cend<T>()) { break; }
				}
				return it;
			}
			return this->cend<T>();
		}

		template <class T> inline T const * find_by_index(int32_t index) const
		{
			if (auto it{ this->get_iter_at_index<T>(index) }; it != this->cend<T>())
			{
				return static_cast<T const *>(it->second);
			}
			return nullptr;
		}

		template <class T> inline int32_t get_index_of(T const * value) const
		{
			if (value)
			{
				int32_t index{ 0 };
				for (auto const & pair : this->data<T>())
				{
					if (pair.second == value) { return index; }
					index++;
				}
			}
			return -1;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <hash_t H> inline auto begin()				{ return this->data<H>().begin(); }
		template <hash_t H> inline auto begin()		const	{ return this->data<H>().begin(); }
		template <class T>	inline auto begin()				{ return this->begin<typeof<T>::hash()>(); }
		template <class T>	inline auto begin()		const	{ return this->begin<typeof<T>::hash()>(); }
		
		template <hash_t H> inline auto end()				{ return this->data<H>().end(); }
		template <hash_t H> inline auto end()		const	{ return this->data<H>().end(); }
		template <class T>	inline auto end()				{ return this->end<typeof<T>::hash()>(); }
		template <class T>	inline auto end()		const	{ return this->end<typeof<T>::hash()>(); }
		
		template <hash_t H> inline auto cbegin()	const	{ return this->data<H>().cbegin(); }
		template <class T>	inline auto cbegin()	const	{ return this->cbegin<typeof<T>::hash()>(); }
		
		template <hash_t H> inline auto cend()		const	{ return this->data<H>().cend(); }
		template <class T>	inline auto cend()		const	{ return this->cend<typeof<T>::hash()>(); }
		
		template <hash_t H> inline auto size()		const	{ return this->data<H>().size(); }
		template <class T>	inline auto size()		const	{ return this->size<typeof<T>::hash()>(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend class Engine;

		ContentManager() : m_data{} {}

		~ContentManager() { this->dispose(); }

		mutable Catagories m_data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CONTENT_MANAGER_HPP_