#include <ML/Core/Random.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	void Random::seed()
	{
		return seed((uint32_t)time(NULL));
	}

	void Random::seed(const uint32_t value)
	{
		return srand(value);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}