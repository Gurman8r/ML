#include <ML/Engine/ManifestItem.hpp>
#include <ML/Core/StringUtility.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	ManifestItem::ManifestItem()
		: data()
	{
	}

	ManifestItem::ManifestItem(const map_type & data)
		: data(data)
	{
	}

	ManifestItem::ManifestItem(const init_type & init)
		: data()
	{
		for (auto it = init.begin(); it != init.end(); it++)
		{
			data.insert(*it);
		}
	}

	ManifestItem::ManifestItem(const ManifestItem & copy)
		: data(copy.data)
	{
	}

	ManifestItem::~ManifestItem() {}

	/* * * * * * * * * * * * * * * * * * * * */

	const String ManifestItem::getStr(const String & value, const String & dv) const
	{
		HashMap<String, String>::const_iterator it;
		return (((it = data.find(value)) != data.end()) ? it->second : dv);
	}

	const int32_t ManifestItem::getInt(const String & value, const int32_t dv) const
	{
		if (const String str = getStr(value))
		{
			int32_t temp;
			if (StringUtility::MakeInt(str, temp))
			{
				return temp;
			}
		}
		return dv;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}