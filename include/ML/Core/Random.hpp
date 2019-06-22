#ifndef _ML_RANDOM_HPP_
#define _ML_RANDOM_HPP_

#include <ML/Core/Export.hpp>
#include <ML/Core/StandardLib.hpp>
#include <ML/Core/I_Singleton.hpp>

#define ML_Random ml::Random::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API Random final
		: public I_Singleton<Random>
	{
		friend class I_Singleton<Random>;

	private:
		Random() { std::srand((uint32_t)std::time(nullptr)); }
		~Random() {}

	public:
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
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RANDOM_HPP_