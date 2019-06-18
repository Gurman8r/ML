#ifndef _ML_OS_HPP_
#define _ML_OS_HPP_

#include <ML/Core/Export.hpp>
#include <ML/Core/String.hpp>
#include <ML/Core/ISingleton.hpp>

#define ML_OS ml::OS::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API OS final
		: public ISingleton<OS>
	{
		friend class ISingleton<OS>;

	public:
		void * execute(const String & cmd) const;
		void * execute(const String & cmd, const String & file) const;
		void * execute(const String & cmd, const String & file, const String & args) const;
		void * execute(const String & cmd, const String & file, const String & args, const String & path) const;
		void * execute(const String & cmd, const String & file, const String & args, const String & path, const int32_t flags) const;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_OS_HPP_
