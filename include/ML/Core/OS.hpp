#ifndef _ML_OS_HPP_
#define _ML_OS_HPP_

#include <ML/Core/Export.hpp>
#include <ML/Core/String.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API OS final
	{
	public:
		static void * execute(const String & cmd);
		static void * execute(const String & cmd, const String & file);
		static void * execute(const String & cmd, const String & file, const String & args);
		static void * execute(const String & cmd, const String & file, const String & args, const String & path);
		static void * execute(const String & cmd, const String & file, const String & args, const String & path, const int32_t flags);
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_OS_HPP_
