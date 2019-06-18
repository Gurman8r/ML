#include <ML/Engine/Preferences.hpp>
#include <ML/Core/Debug.hpp>
#include <INIReader.h>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Prefs::Prefs()
		: m_ini(nullptr)
	{
	}

	Prefs::Prefs(const String & filename)
		: Prefs()
	{
		loadFromFile(filename);
	}

	Prefs::Prefs(Prefs && copy)
		: Prefs()
	{
		std::swap(m_ini, copy.m_ini);
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
			m_ini = nullptr;
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

	double Prefs::GetDouble(const String & section, const String & name, double default_value) const
	{
		return ((m_ini)
			? (static_cast<INIReader *>(m_ini)->GetReal(
				section,
				name,
				default_value
			))
			: (default_value)
		);
	}

	float Prefs::GetFloat(const String & section, const String & name, float default_value) const
	{
		return (float)GetDouble(section, name, (double)default_value);
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
				(String)default_value
			))
			: ((String)default_value)
		);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}