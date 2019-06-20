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

	bool ManifestItem::loadValues(istream & file, String & line)
	{
		if (line.empty() || (line.trim().front() == '#'))
		{
			return false;
		}

		if (line.find("<import>") != String::npos)
		{
			while (std::getline(file, line))
			{
				line.replaceAll("$(Configuration)", ML_CONFIGURATION);
				line.replaceAll("$(PlatformTarget)", ML_PLATFORM_TARGET);

				if (line.find("</import>") != String::npos)
				{
					return true;
				}
				else
				{
					size_t i;
					if ((i = line.find_first_of("=")) != String::npos)
					{
						if (const String key = String(line.substr(0, i)).trim())
						{
							if (const String val = String(
								line.substr((i + 1), (line.size() - i - 2))).trim())
							{
								(*this)[key] = val;
							}
						}
					}
				}
			}
		}
		return false;
	}

	String ManifestItem::getStr(const String & value, const String & dv) const
	{
		HashMap<String, String>::const_iterator it;
		return (((it = data.find(value)) != data.end()) ? it->second : dv);
	}

	int32_t ManifestItem::getInt(const String & value, const int32_t dv) const
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

	uint32_t ManifestItem::getUint(const String & value, const uint32_t dv) const
	{
		return (uint32_t)getInt(value, (int32_t)dv);
	}

	bool ManifestItem::getBool(const String & value, const bool dv) const
	{
		if (const String str = StringUtility::ToLower(getStr(value)))
		{
			return (str == "1" || str == "true");
		}
		return dv;
	}

	int32_t ManifestItem::getEnum(const String & value, const int32_t dv, const Map<String, int32_t>& args) const
	{
		if (!args.empty())
		{
			if (const String str = StringUtility::ToLower(getStr(value)))
			{
				Map<String, int32_t>::const_iterator it;
				if ((it = args.find(str)) != args.end())
				{
					return it->second;
				}
			}
		}
		return dv;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}