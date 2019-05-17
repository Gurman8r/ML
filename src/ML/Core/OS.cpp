#include <ML/Core/OS.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/CoreEvents.hpp>
#include <ML/Core/Debug.hpp>

# ifdef ML_SYSTEM_WINDOWS
#	include <Windows.h>
#	include <shellapi.h>
# endif

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	OS::OS()
	{
		ML_EventSystem.addListener(CoreEvent::EV_OS_Execute, this);
	}

	OS::~OS() { }

	/* * * * * * * * * * * * * * * * * * * * */

	void OS::onEvent(const IEvent * value)
	{
		switch (*value)
		{
		case CoreEvent::EV_OS_Execute:
			if (const auto * ev = value->as<OS_ExecuteEvent>())
			{
				ML_OS.execute(
					ev->cmd,
					ev->file,
					ev->args,
					ev->path,
					ev->flags
				);
			}
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void * OS::execute(const String & cmd)
	{
		return execute(cmd, String());
	}

	void * OS::execute(const String & cmd, const String & file)
	{
		return execute(cmd, file, String());
	}

	void * OS::execute(const String & cmd, const String & file, const String & args)
	{
		return execute(cmd, file, args, String());
	}

	void * OS::execute(const String & cmd, const String & file, const String & args, const String & path)
	{
		return execute(cmd, file, args, path, 5); // SW_SHOW
	}

	void * OS::execute(const String & cmd, const String & file, const String & args, const String & path, const int32_t flags)
	{
#if defined(ML_SYSTEM_WINDOWS)
		return ShellExecuteA(GetDesktopWindow(), cmd.c_str(), file.c_str(), args.c_str(), path.c_str(), flags);
#else
		return NULL;
#endif // ML_SYSTEM_WINDOWS
	}

	/* * * * * * * * * * * * * * * * * * * * */
}