#include <ML/Engine/Preferences.hpp>
#include <ML/Core/Debug.hpp>
#include <inih/INIReader.h>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Preferences::Preferences()
		: m_ini { nullptr }
		, m_filename {}
	{
	}

	Preferences::Preferences(String const & filename)
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
	
	bool Preferences::loadFromFile(String const & filename)
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

	bool Preferences::saveToFile(String const & filename) const
	{
		if (std::ofstream file { filename })
		{
			for (String const & elem : sections())
			{
				// WIP
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
		if (auto ini { static_cast<INIReader const *>(m_ini) })
		{
			return reinterpret_cast<const std::set<String> &>(ini->Sections());
		}
		return std::set<String>();
	}

	Tree<String, String> Preferences::values() const
	{
		if (auto ini { static_cast<INIReader const *>(m_ini) })
		{
			return Tree<String, String> { ini->Values().begin(), ini->Values().end() };
		}
		return Tree<String, String>();
	}

	bool Preferences::set_string(String const & section, String const & name, String const & value)
	{
		if (auto ini { static_cast<INIReader *>(m_ini) })
		{
			ini->Set(section, name, value);

			return (get_string(section, name, "") == value);
		}
		return false;
	}

	bool Preferences::get_bool(String const & section, String const & name, bool dv) const
	{
		return ((m_ini)
			? static_cast<INIReader *>(m_ini)->GetBoolean(section, name, dv)
			: dv
		);
	}

	float64_t Preferences::get_double(String const & section, String const & name, float64_t dv) const
	{
		return ((m_ini)
			? static_cast<INIReader *>(m_ini)->GetReal(section, name, dv)
			: dv
		);
	}

	float_t Preferences::get_float(String const & section, String const & name, float_t dv) const
	{
		return (float_t)get_double(section, name, (float64_t)dv);
	}

	int32_t Preferences::get_int(String const & section, String const & name, int32_t dv) const
	{
		return ((m_ini) 
			? static_cast<INIReader *>(m_ini)->GetInteger(section, name, dv)
			: dv
		);
	}

	uint32_t Preferences::get_uint(String const & section, String const & name, uint32_t dv) const
	{
		return (uint32_t)get_int(section, name, (int32_t)dv);
	}

	String Preferences::get_string(String const & section, String const & name, String const & dv) const
	{
		return ((m_ini)
			? (String)static_cast<INIReader *>(m_ini)->Get(section, name, dv)
			: dv
		);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}