#include <ML/Engine/SharedLibrary.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	SharedLibrary::SharedLibrary()
		: m_instance(nullptr)
		, m_filename()
		, m_functions()
	{
	}

	SharedLibrary::SharedLibrary(String const & filename)
		: SharedLibrary()
	{
		this->loadFromFile(filename);
	}

	SharedLibrary::SharedLibrary(SharedLibrary && copy) noexcept
		: SharedLibrary()
	{
		std::swap(m_instance, copy.m_instance);
		std::swap(m_filename, copy.m_filename);
		std::swap(m_functions, copy.m_functions);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool SharedLibrary::dispose()
	{
#ifdef ML_SYSTEM_WINDOWS
		return FreeLibrary(static_cast<HINSTANCE>(m_instance));
#else
		// do the thing
#endif
	}

	bool SharedLibrary::loadFromFile(String const & filename)
	{
		if (!ML_FS.fileExists(m_filename = filename))
		{
			return false;
		}

#ifdef ML_SYSTEM_WINDOWS
			return (m_instance = LoadLibraryA(m_filename.c_str()));
#else
			// do the thing
#endif
	}

	void * SharedLibrary::loadFunction(String const & name)
	{
		if (auto it{ m_functions.find(name) }; it != m_functions.end())
		{
			return it->second;
		}
		else if (m_instance)
		{
			void * location { 
#ifdef ML_SYSTEM_WINDOWS
				GetProcAddress(static_cast<HINSTANCE>(m_instance), name.c_str())
#else
				// do the thing
#endif
			};
			if (!location)
			{
				Debug::logWarning("Function, \'{0}\', not found in \'{1}\'.", 
					name, this->filename()
				);
			}
			return m_functions.insert({ name, location }).first->second;
		}
		return nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}