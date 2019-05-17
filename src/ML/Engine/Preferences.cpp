#include <ML/Engine/Preferences.hpp>
#include <INIReader.h>

#define ML_INI(e) static_cast<INIReader *>(e)

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
		if (!(m_ini) && (m_ini = ML_INI(new INIReader(filename))))
		{
			return ML_INI(m_ini)->ParseError() == EXIT_SUCCESS;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	inline bool Preferences::GetBoolean(String section, String name, bool default_value)
	{
		return ((m_ini)
			? (ML_INI(m_ini)->GetBoolean(section, name, default_value))
			: ((default_value)));
	}

	inline float Preferences::GetFloat(String section, String name, float default_value)
	{
		return ((m_ini) 
			? (float)(ML_INI(m_ini)->GetReal(section, name, default_value)) 
			: (default_value));
	}

	inline long Preferences::GetInteger(String section, String name, long default_value)
	{
		return ((m_ini) 
			? (ML_INI(m_ini)->GetInteger(section, name, default_value)) 
			: (default_value));
	}

	inline String Preferences::GetString(String section, String name, String default_value)
	{
		return ((m_ini)
			? (ML_INI(m_ini)->Get(section, name, default_value)) 
			: (default_value));
	}

	/* * * * * * * * * * * * * * * * * * * * */
}