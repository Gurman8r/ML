#ifndef _ML_RANDOM_HPP_
#define _ML_RANDOM_HPP_

#include <ML/Core/Export.hpp>
#include <ML/Core/Singleton.hpp>

#define ML_Random ::ml::Random::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct Random final : public Singleton<Random>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> static inline T roll()
		{
			return static_cast<T>(std::rand());
		}

		template <class T> static inline T roll(T max)
		{ 
			return (roll<T>() / max);
		}

		template <class T> static inline T roll(T min, T max)
		{
			return (min + (roll<T>(max) * (max - min)));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend struct Singleton<Random>;

		Random() { std::srand((uint32_t)std::time(nullptr)); }

		~Random() {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RANDOM_HPP_