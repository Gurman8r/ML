#include <ML/Engine/SharedLibrary.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>

# ifdef ML_SYSTEM_WINDOWS
#	include <Windows.h>
#	define ML_FREE_LIBRARY(inst)		FreeLibrary(static_cast<HINSTANCE>(inst))
#	define ML_LOAD_LIBRARY(file)		LoadLibraryA(file)
#	define ML_LOAD_FUNCTION(inst, name) GetProcAddress(static_cast<HINSTANCE>(inst), name)
#else
#	define ML_LOAD_LIBRARY(file)		(0)
#	define ML_FREE_LIBRARY(inst)		(0)
#	define ML_LOAD_FUNCTION(inst, name) (0)
#endif

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	SharedLibrary::SharedLibrary()
		: m_instance(nullptr)
		, m_filename()
		, m_functions()
	{
	}

	SharedLibrary::SharedLibrary(const String & filename)
		: SharedLibrary()
	{
		this->loadFromFile(filename);
	}

	SharedLibrary::SharedLibrary(SharedLibrary && copy)
		: SharedLibrary()
	{
		std::swap(m_instance, copy.m_instance);
		std::swap(m_filename, copy.m_filename);
		std::swap(m_functions, copy.m_functions);
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
		return 
			(m_filename = filename) &&
			(ML_FS.fileExists(filename)) &&
			(m_instance = ML_LOAD_LIBRARY(m_filename.c_str()));
	}

	void * SharedLibrary::loadFunction(const String & name)
	{
		auto it { m_functions.find(name) };
		if (it != m_functions.end())
		{
			return it->second;
		}
		else if (m_instance)
		{
			void * location { ML_LOAD_FUNCTION(m_instance, name.c_str()) };
			if (!location)
			{
				Debug::logWarning("Function, \'{0}\', not found in \'{1}\'.", 
					name, 
					filename()
				);
			}
			return m_functions.insert({ name, location }).first->second;
		}
		return nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}