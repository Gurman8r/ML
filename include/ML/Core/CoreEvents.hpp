#ifndef _ML_CORE_EVENTS_HPP_
#define _ML_CORE_EVENTS_HPP_

#include <ML/Core/IEvent.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API CoreEvent : public IEvent
	{
		enum : int32_t
		{
			MIN_CORE_EVENT = IEvent::EV_CORE + 1,
			
			EV_FS_ChangeDir,
			EV_OS_Execute,
			
			MAX_CORE_EVENT
		};

		CoreEvent(int32_t id)
			: IEvent(id)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API FS_ChangDirEvent final : public CoreEvent
	{
		const String dir;

		FS_ChangDirEvent(const String & dir)
			: CoreEvent(EV_FS_ChangeDir)
			, dir(dir)
		{
		}

		inline void serialize(std::ostream & out) const override
		{
			out << GetTypeName() << " " << dir;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API OS_ExecuteEvent final : public CoreEvent
	{
		const String	cmd;
		const String	file;
		const String	args;
		const String	path;
		const int32_t	flags;

		OS_ExecuteEvent(
			const String & cmd,
			const String & file	 = String(),
			const String & args	 = String(),
			const String & path	 = String(),
			const int32_t  flags = 5) // SW_SHOW
			: CoreEvent(EV_OS_Execute)
			, cmd	(cmd)
			, file	(file)
			, args	(args)
			, path	(path)
			, flags	(flags)
		{
		}

		inline void serialize(std::ostream & out) const override
		{
			out << GetTypeName() << " "
				<< cmd << " "
				<< file << " "
				<< args << " "
				<< path << " ";
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CORE_EVENTS_HPP_