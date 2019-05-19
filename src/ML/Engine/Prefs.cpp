#include <ML/Engine/Prefs.hpp>
#include <ML/Core/Debug.hpp>
#include <INIReader.h>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Prefs::Prefs()
		: m_ini(NULL)
	{
	}

	Prefs::~Prefs()
	{
		dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Prefs::dispose()
	{
		if (m_ini)
		{
			delete m_ini;
			m_ini = NULL;
		}
		return !(m_ini);
	}
	
	bool Prefs::loadFromFile(const String & filename)
	{
		return ((!(m_ini) && (m_ini = static_cast<INIReader *>(new INIReader(filename))))
			? (static_cast<INIReader *>(m_ini)->ParseError() == EXIT_SUCCESS)
			: (false)
		);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Prefs::GetBool(const String & section, const String & name, bool default_value) const
	{
		return ((m_ini)
			? (static_cast<INIReader *>(m_ini)->GetBoolean(
				section,
				name,
				default_value
			))
			: (default_value)
		);
	}

	float Prefs::GetFloat(const String & section, const String & name, float default_value) const
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

	int32_t Prefs::GetInt(const String & section, const String & name, int32_t default_value) const
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

	uint32_t Prefs::GetUint(const String & section, const String & name, uint32_t default_value) const
	{
		return (uint32_t)GetInt(section, name, (int32_t)default_value);
	}

	String Prefs::GetString(const String & section, const String & name, const String & default_value) const
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