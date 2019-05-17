#include <ML/Engine/SharedLibrary.hpp>
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

	SharedLibrary::SharedLibrary()
		: m_instance(NULL)
		, m_filename()
	{
	}

	SharedLibrary::~SharedLibrary()
	{
		dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool SharedLibrary::dispose()
	{
		return ML_FREE_LIBRARY(m_instance);
	}

	bool SharedLibrary::loadFromFile(const String & filename)
	{
		return (m_instance = ML_LOAD_LIBRARY(filename.c_str()));
	}

	void * SharedLibrary::loadFunction(const String & name)
	{
		Map<String, void *>::const_iterator it;
		if ((it = m_fun.find(name)) != m_fun.end())
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
			return m_fun.insert({ name, location }).first->second;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}