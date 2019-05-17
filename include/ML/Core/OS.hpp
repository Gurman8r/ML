#ifndef _ML_OS_HPP_
#define _ML_OS_HPP_

#include <ML/Core/ITrackable.hpp>
#include <ML/Core/IEventListener.hpp>

#define ML_OS ml::OS::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API OS
		: public ITrackable
		, public IEventListener
		, public ISingleton<OS>
	{
		friend class ISingleton<OS>;

	private:
		OS();
		~OS();

	public:
		void onEvent(const IEvent * value) override;

	public:
		void * execute(const String & cmd);
		void * execute(const String & cmd, const String & file);
		void * execute(const String & cmd, const String & file, const String & args);
		void * execute(const String & cmd, const String & file, const String & args, const String & path);
		void * execute(const String & cmd, const String & file, const String & args, const String & path, const int32_t flags);
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_OS_HPP_
