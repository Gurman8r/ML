#ifndef _ML_MANIFEST_ITEM_HPP_
#define _ML_MANIFEST_ITEM_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/ITrackable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API ManifestItem final : public ITrackable
	{
		/* * * * * * * * * * * * * * * * * * * * */

		using map_type			= typename HashMap<String, String>;
		using pair_type			= typename Pair<String, String>;
		using init_type			= typename InitList<pair_type>;
		using iterator			= typename map_type::iterator;
		using const_iterator	= typename map_type::const_iterator;

		/* * * * * * * * * * * * * * * * * * * * */

		map_type data;

		/* * * * * * * * * * * * * * * * * * * * */

		ManifestItem();
		ManifestItem(const map_type & data);
		ManifestItem(const init_type & init);
		ManifestItem(const ManifestItem & copy);
		~ManifestItem();

		/* * * * * * * * * * * * * * * * * * * * */

		inline const String & operator[](const String & key) const 
		{
			return data.at(key); 
		}

		inline String & operator[](const String & key) 
		{
			return data[key]; 
		}

		/* * * * * * * * * * * * * * * * * * * * */

		bool loadValues(std::istream & file, String & line);

		/* * * * * * * * * * * * * * * * * * * * */

		const String getStr(const String & value, const String & dv = String()) const;

		const int32_t getInt(const String & value, const int32_t dv = 0) const;

		const bool getBool(const String & value, const bool dv = false) const;

		/* * * * * * * * * * * * * * * * * * * * */

		inline iterator			begin()			{ return data.begin();  }
		inline iterator			end()			{ return data.end();	}
		inline const_iterator	begin()  const	{ return data.begin();  }
		inline const_iterator	end()	 const	{ return data.end();	}
		inline const_iterator	cbegin() const	{ return data.cbegin(); }
		inline const_iterator	cend()	 const	{ return data.cend();	}

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MANIFEST_ITEM_HPP_