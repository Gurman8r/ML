#ifndef _ML_CONTENT_HPP_
#define _ML_CONTENT_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Engine/Asset.hpp>
#include <ML/Core/List.hpp>
#include <ML/Core/Metadata.hpp>
#include <ML/Core/I_Disposable.hpp>
#include <ML/Core/I_Readable.hpp>

#define ML_Content _ML Content::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Monolithic bank of shared resources.
	// Anything can be stored in Content as long as it derrives I_Newable.
	struct ML_ENGINE_API Content final
		: public I_Disposable
		, public I_Readable
		, public I_Singleton<Content>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		using object_map = typename Map<String, AssetBase *>;
		using typeid_map = typename HashMap<size_t, object_map>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		bool dispose() override;
		bool loadFromFile(const String & filename) override;
		
		auto readLists(Istream & file) const -> List<Metadata *>;
		auto readMetadata(Istream & file, String & line) const -> Metadata *;
		
		auto parseLists(const List<Metadata *> & value) -> size_t;
		bool parseMetadata(const Metadata & data);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline object_map & at(size_t index)
		{
			typeid_map::iterator it;
			return (((it = m_data.find(index)) != m_data.end())
				? it->second
				: m_data.insert({ index, object_map() }).first->second
			);
		}

		inline const object_map & at(size_t index) const
		{
			typeid_map::const_iterator it;
			return (((it = m_data.find(index)) != m_data.cend())
				? it->second
				: m_data.insert({ index, object_map() }).first->second
			);
		}
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> inline object_map & data()
		{
			static object_map & temp { this->at(typeid(T).hash_code()) };
			return temp;
		}

		template <
			class T
		> inline const object_map & data() const
		{
			static const object_map & temp { this->at(typeid(T).hash_code()) };
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> inline Asset<T> * find(const String & name)
		{
			static object_map::iterator it;
			return (((it = this->data<T>().find(name)) != this->data<T>().end())
				? Asset<T>::cast(it->second)
				: nullptr
			);
		}

		template <
			class T
		> inline const Asset<T> * find(const String & name) const
		{
			static object_map::const_iterator it;
			return (((it = this->data<T>().find(name)) != this->data<T>().end())
				? Asset<T>::cast(it->second)
				: nullptr
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		template <
			class T, class ... Args
		> inline T * create(const String & name, Args && ... args)
		{
			return ((!this->find<T>(name))
				? this->insert(name, new T { std::forward<Args>(args)... })
				: nullptr
			);
		}

		template <
			class T
		> inline T * insert(const String & name, T * value)
		{
			return Asset<T>::cast(this->data<T>().insert({
				name, new Asset<T> { value, AssetBase::None }
			}).first->second)->get();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> inline T * get(const String & name)
		{
			static Asset<T> * temp;
			return ((temp = this->find<T>(name)) ? temp->get() : nullptr);
		}

		template <
			class T
		> inline const T * get(const String & name) const
		{
			static const Asset<T> * temp;
			return ((temp = this->find<T>(name)) ? temp->get() : nullptr);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> inline List<String> getKeys() const
		{
			List<String> temp;
			for (const auto & pair : this->data<T>())
			{
				temp.push_back(pair.first);
			}
			return temp;
		}

		template <
			class T
		> inline object_map::const_iterator getIterAt(int32_t index) const
		{
			if ((index >= 0) && ((size_t)index < this->data<T>().size()))
			{
				object_map::const_iterator it = this->data<T>().cbegin();
				for (int32_t i = 0; i < index; i++)
				{
					if ((++it) == this->data<T>().cend())
					{
						return it;
					}
				}
				return it;
			}
			return this->data<T>().cend();
		}

		template <
			class T
		> inline const T * getByIndex(int32_t index) const
		{
			object_map::const_iterator it;
			return (((it = this->getIterAt<T>(index)) != this->data<T>().end())
				? it->second->as<T>()
				: nullptr
			);
		}

		template <
			class T
		> inline int32_t getIndexOf(const T * value) const
		{
			int32_t index = 0;
			for (const auto & pair : this->data<T>())
			{
				if (pair.second->obj == value)
				{
					return index;
				}
				index++;
			}
			return (index = (-1));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend struct I_Singleton<Content>;

		Content() : m_data() {}
		~Content() { dispose(); }

		mutable typeid_map m_data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CONTENT_HPP_