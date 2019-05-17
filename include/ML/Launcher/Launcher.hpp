#ifndef _ML_LAUNCHER_HPP_
#define _ML_LAUNCHER_HPP_

#include <ML/Engine/Application.hpp>

namespace ml
{
	class Launcher final
		: public ISingleton<Launcher>
	{
		friend class ISingleton<Launcher>;

	public:
		Launcher();
		~Launcher();

	private:

	};
}

#endif // !_ML_LAUNCHER_HPP_