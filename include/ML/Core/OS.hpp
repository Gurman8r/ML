#ifndef _ML_OS_HPP_
#define _ML_OS_HPP_

#include <ML/Core/Export.hpp>
#include <ML/Core/String.hpp>
#include <ML/Core/I_Singleton.hpp>

#define ML_OS ml::OS::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API OS final
		: public I_Singleton<OS>
	{
		friend class I_Singleton<OS>;

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
