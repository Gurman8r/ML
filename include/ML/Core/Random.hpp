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

		template <
			class T
		> inline T roll() const
		{
			return static_cast<T>(std::rand());
		}

		template <
			class T
		> inline T roll(const T max) const
		{ 
			return (roll<T>() / max);
		}

		template <
			class T
		> inline T roll(const T min, const T max) const
		{
			return (min + (roll<T>(max) * (max - min)));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend struct Singleton<Random>;

		Random() { std::srand((uint32_t)std::time(nullptr)); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RANDOM_HPP_