#include <ML/Engine/Plugin.hpp>
#include <ML/Core/Debug.hpp>

# ifdef ML_SYSTEM_WINDOWS
#	include <Windows.h>
#	define ML_FREE_LIBRARY(inst)		(FreeLibrary((HINSTANCE)inst))
#	define ML_LOAD_LIBRARY(file)		(LoadLibraryA(file))
#	define ML_LOAD_FUNCTION(inst, name) (GetProcAddress((HINSTANCE)inst, name))
# else
#	define ML_LOAD_LIBRARY(file)		(0)
#	define ML_FREE_LIBRARY(inst)		(0)
#	define ML_LOAD_FUNCTION(inst, name) (0)
# endif

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Plugin::Plugin()
		: m_instance(NULL)
		, m_filename()
		, m_functions()
	{
	}

	Plugin::Plugin(const String & filename)
		: Plugin()
	{
		loadFromFile(filename);
	}

	Plugin::Plugin(Plugin && copy)
		: Plugin()
	{
		this->Plugin::operator=((Plugin &&)copy);
	}

	Plugin::~Plugin()
	{
		dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Plugin & Plugin::operator=(Plugin && copy)
	{
		m_instance = copy.m_instance;
		m_filename = copy.m_filename;
		m_functions = copy.m_functions;
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Plugin::dispose()
	{
		return ML_FREE_LIBRARY(m_instance);
	}

	bool Plugin::loadFromFile(const String & filename)
	{
		return (m_instance = ML_LOAD_LIBRARY((m_filename = filename).c_str()));
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void * Plugin::loadFunction(const String & name)
	{
		Map<String, void *>::const_iterator it;
		if ((it = m_functions.find(name)) != m_functions.end())
		{
			return it->second;
		}
		else
		{
			void * location;
			if (!(location = ML_LOAD_FUNCTION(m_instance, name.c_str())))
			{
				Debug::logWarning("Uniform Not Found: \"{0}\"", name);

			}
			return m_functions.insert({ name, location }).first->second;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}