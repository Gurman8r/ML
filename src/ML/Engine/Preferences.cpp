#include <ML/Engine/Preferences.hpp>
#include <INIReader.h>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Preferences::Preferences()
		: m_ini(NULL)
	{
	}

	Preferences::~Preferences()
	{
		dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Preferences::dispose()
	{
		if (m_ini)
		{
			delete m_ini;
			m_ini = NULL;
		}
		return !(m_ini);
	}
	
	bool Preferences::loadFromFile(const String & filename)
	{
		return ((!(m_ini) && (m_ini = static_cast<INIReader *>(new INIReader(filename))))
			? (static_cast<INIReader *>(m_ini)->ParseError() == EXIT_SUCCESS)
			: (false)
		);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Preferences::GetBool(const String & section, const String & name, bool default_value)
	{
		return ((m_ini)
			? (static_cast<INIReader *>(m_ini)->GetBoolean(
				section,
				name,
				default_value
			))
			: ((default_value))
		);
	}

	float Preferences::GetFloat(const String & section, const String & name, float default_value)
	{
		return ((m_ini) 
			? (float)(static_cast<INIReader *>(m_ini)->GetReal(
				section,
				name,
				default_value
			))
			: (default_value)
		);
	}

	int32_t Preferences::GetInt(const String & section, const String & name, int32_t default_value)
	{
		return ((m_ini) 
			? (static_cast<INIReader *>(m_ini)->GetInteger(
				section, 
				name,
				default_value
			))
			: (default_value)
		);
	}

	uint32_t Preferences::GetUint(const String & section, const String & name, uint32_t default_value)
	{
		return (uint32_t)GetInt(section, name, (int32_t)default_value);
	}

	String Preferences::GetString(const String & section, const String & name, const String & default_value)
	{
		return ((m_ini)
			? (static_cast<INIReader *>(m_ini)->Get(
				section, 
				name, 
				(std::string)default_value
			))
			: ((std::string)default_value)
		);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}