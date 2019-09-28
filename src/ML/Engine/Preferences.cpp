#include <ML/Engine/Preferences.hpp>
#include <ML/Core/Debug.hpp>
#include <INIReader.h>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Preferences::Preferences()
		: m_ini { nullptr }
		, m_filename {}
	{
	}

	Preferences::Preferences(const Preferences & copy)
		: m_ini { nullptr }
		, m_filename { copy.m_filename }
	{
		if (!m_ini && copy.m_ini)
		{
			if (auto ini { static_cast<INIReader *>(m_ini = new INIReader())})
			{
				ini->Copy(*static_cast<const INIReader *>(copy.m_ini));
			}
		}
	}

	Preferences::Preferences(const String & filename)
		: Preferences()
	{
		if (!loadFromFile(filename))
		{
			Debug::logError("FAILED LOADING INI: \'{0}\'", filename);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Preferences::dispose()
	{
		if (m_ini)
		{
			delete m_ini;
			m_ini = nullptr;
		}
		return !(m_ini);
	}
	
	bool Preferences::loadFromFile(const String & filename)
	{
		if ((!m_ini) && (m_ini = new INIReader(m_filename = filename)))
		{
			if (static_cast<INIReader *>(m_ini)->ParseError() == EXIT_SUCCESS)
			{
				return true;
			}
		}
		return false;
	}

	bool Preferences::saveToFile(const String & filename) const
	{
		if (std::ofstream file { filename })
		{
			for (const String & elem : sections())
			{

			}
			
			file.close();
		}
		return false;
	}

	bool Preferences::saveToFile() const
	{
		return saveToFile(m_filename);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	std::set<String> Preferences::sections() const
	{
		if (auto ini { static_cast<const INIReader *>(m_ini) })
		{
			return reinterpret_cast<const std::set<String> &>(ini->Sections());
		}
		return std::set<String>();
	}

	Tree<String, String> Preferences::values() const
	{
		if (auto ini { static_cast<const INIReader *>(m_ini) })
		{
			return Tree<String, String> { ini->Values().begin(), ini->Values().end() };
		}
		return Tree<String, String>();
	}

	bool Preferences::set_string(const String & section, const String & name, const String & value)
	{
		if (auto ini { static_cast<INIReader *>(m_ini) })
		{
			ini->Set(section, name, value);

			return (get_string(section, name, "") == value);
		}
		return false;
	}

	bool Preferences::get_bool(const String & section, const String & name, bool dv) const
	{
		return ((m_ini)
			? static_cast<INIReader *>(m_ini)->GetBoolean(section, name, dv)
			: dv
		);
	}

	float64_t Preferences::get_double(const String & section, const String & name, float64_t dv) const
	{
		return ((m_ini)
			? static_cast<INIReader *>(m_ini)->GetReal(section, name, dv)
			: dv
		);
	}

	float_t Preferences::get_float(const String & section, const String & name, float_t dv) const
	{
		return (float_t)get_double(section, name, (float64_t)dv);
	}

	int32_t Preferences::get_int(const String & section, const String & name, int32_t dv) const
	{
		return ((m_ini) 
			? static_cast<INIReader *>(m_ini)->GetInteger(section, name, dv)
			: dv
		);
	}

	uint32_t Preferences::get_uint(const String & section, const String & name, uint32_t dv) const
	{
		return (uint32_t)get_int(section, name, (int32_t)dv);
	}

	String Preferences::get_string(const String & section, const String & name, const String & dv) const
	{
		return ((m_ini)
			? static_cast<INIReader *>(m_ini)->Get(section, name, (String)dv)
			: (String)dv
		);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}